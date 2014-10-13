#include "base/Log.h"
#include "base/CommonMacro.h"
#include "ShareMemoryCenter.h"

#define SHM_MAGIC   ("RSHM")
#if 1
int     ShareMemoryCenter::Register(const ShareMemoryModuleReg & reg)
{
    if(FindReg(reg.name))
    {
        LOG_FATAL("register repeatly ! name = %s",reg.name.c_str());
        return -1 ;
    }
    m_vecModules.push_back(reg);
    m_zTotalSize += reg.zSize;
    LOG_INFO("register sbm sub module name = %s size = %zu",reg.name.c_str(),reg.zSize);
    return 0;
}    
void    ShareMemoryCenter::DeleteSHM()
{
    LOG_WARN("recovery share mem delete shm !");
    m_shm.Destroy(m_shm.GetShmID());
}
int     ShareMemoryCenter::Init(const char* pszShmKeyPath)
{
    //if get a valid old size
    //check valid
    //notify them rebuild it(Attach)
    //else old shm is valid 
    //then recover it
    //else there is no old shm
    //init it
    m_zTotalSize += sizeof(ShareMemoryCenterDataFmt);
    int shmKey = ShareMemory::PathToKey(pszShmKeyPath);
    int ret = m_shm.Attach(shmKey,m_zTotalSize);

    m_pDataBase = (char*)m_shm.GetData();
    m_pFmtBase = (ShareMemoryCenterDataFmt*)m_pDataBase;

    if(ret)
    {
        if(1 == ret)
        {
            LOG_ERROR("there is no old shm found ! it will create a new memory block !");
            if(m_shm.Create(shmKey,m_zTotalSize))
            {
                LOG_ERROR("shm create error !");
                return -1;
            }
            //
            m_pDataBase = (char*)m_shm.GetData();
            m_pFmtBase = (ShareMemoryCenterDataFmt*)m_pDataBase;
            InitRegModules();
        }
        else
        {
            LOG_FATAL("attach shm error = %d ! for = %s",ret,strerror(errno));
            return -1;
        }
    }
    else
    {        
        if(Check())
        {
            LOG_FATAL("attach memory but mem magic chck error !");
            return -1;
        }
        size_t zOldShmSize = GetOldShmSize();
        if(zOldShmSize != m_zTotalSize)
        {
            LOG_ERROR("old shm size = %zu old mod = %u but new shm size = %zu mod = %u . will extend old shm (todo pre alloc big size)",
                     zOldShmSize,m_pFmtBase->modulesCount,m_zTotalSize,m_vecModules.size());
            ////////////////////////////////////////////////////////////////////////////////
            AppendRegModules();//append no reg module
        }
    }  
    return AttachAllModules();
}
void     ShareMemoryCenter::InitRegModules()
{
    bzero(m_pFmtBase,m_zTotalSize);
    memcpy(m_pFmtBase->magic,SHM_MAGIC,strlen(SHM_MAGIC));
    m_pFmtBase->dataOffset = sizeof(ShareMemoryCenterDataFmt);
    AppendRegModules();
}
void     ShareMemoryCenter::AppendRegModules()
{
    for(uint i = 0;i < m_vecModules.size(); ++i)
    {
        if(FindModule(m_vecModules[i].name.c_str()))
        {
            continue;
        }
        ////////////////////////////////////////
        AddModule(m_vecModules[i].name.c_str());
    }
}
size_t  ShareMemoryCenter::GetOldShmSize()
{
    return  m_pFmtBase->dataLength + sizeof(ShareMemoryCenterDataFmt);
}
int     ShareMemoryCenter::AttachAllModules()
{
    for(uint i = 0;i < m_pFmtBase->modulesCount; ++i)
    {        
        ShareMemoryModuleReg*   reg = FindReg(string(m_pFmtBase->modules[i].szName));
        if(!reg)
        {
            LOG_ERROR("not find reg name = %s ",m_pFmtBase->modules[i].szName);
            return -1;
        }
        char* pData = m_pDataBase + m_pFmtBase->modules[i].dataOffset;
        if(reg->notifier && 
           reg->notifier->OnAttach(pData,m_pFmtBase->modules[i].zSize,m_pFmtBase->modules[i].chInited))
        {
            LOG_FATAL("reg name = %s attach init = %d error !",
                m_pFmtBase->modules[i].szName,m_pFmtBase->modules[i].chInited);
            return -1;
        }
        m_pFmtBase->modules[i].chInited = 1;
    }
    return 0;
}
int     ShareMemoryCenter::Check()
{
    return memcmp(m_pFmtBase->magic,SHM_MAGIC,strlen(SHM_MAGIC));
}
ShareMemoryModule*  ShareMemoryCenter::FindModule(const char* pszName)
{
    for(uint i = 0 ; i < m_pFmtBase->modulesCount; ++i)
    {
        if(strcmp(m_pFmtBase->modules[i].szName , pszName) == 0)
        {
            return &(m_pFmtBase->modules[i]);
        }
    }
    return NULL;
}
Buffer       ShareMemoryCenter::GetModleBuffer(const char* pszName)
{
    Buffer buffer;
    ShareMemoryModule* mod = FindModule( pszName);
    if(!mod)
    {
        return buffer;
    }
    buffer.pBuffer = (uint8_t*)m_pDataBase + mod->dataOffset;
    buffer.iUsed = buffer.iCap = mod->zSize;
    return buffer;
}
int         ShareMemoryCenter::AddModule(const char* pszName)
{
    ShareMemoryModuleReg*   reg = FindReg(string(pszName));
    if(!reg)
    {
        return -1;
    }
    if(reg->zSize + m_pFmtBase->dataLength + sizeof(ShareMemoryCenterDataFmt) > m_zTotalSize)
    {
        LOG_FATAL("shm buffer is not enough !!");
        return -1;
    }
    if(m_pFmtBase->modulesCount >= RECOVERYSHM_MAX_MOUDLES)
    {
        LOG_FATAL("moudules is too much !");
        return -1;
    }

    ShareMemoryModule & mod = m_pFmtBase->modules[m_pFmtBase->modulesCount];
    mod.dataOffset = sizeof(ShareMemoryCenterDataFmt) + m_pFmtBase->dataLength;
    mod.zSize      = reg->zSize;
    bzero((char*)m_pFmtBase + mod.dataOffset,mod.zSize);
    STRNCPY(mod.szName,sizeof(mod.szName),reg->name.c_str());
    ///////////////////////////////////////////////////////////////    
    m_pFmtBase->dataLength += reg->zSize;
    ++m_pFmtBase->modulesCount;
    return 0;
}
ShareMemoryModuleReg * ShareMemoryCenter::FindReg(const string & name)
{
    for(uint i = 0;i < m_vecModules.size(); ++i)
    {
        if(m_vecModules[i].name == name)
        {
            return &(m_vecModules[i]);
        }
    }
    return NULL;
}

#endif    
    
