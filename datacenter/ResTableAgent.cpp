#include "base/Log.h"
#include "base/File.h"
#include "base/StringUtil.h"
#include "ResTableAgent.h"


#if 1
uint64_t  ResTableAgent::GetResTableEntryKey(const void*   pObj)
{    
    const MetaSerializer::MetaObject * pMetaObj = (MetaSerializer::MetaObject*)pObj;
    //////////////////////////////////////////////////////////////////////////
    //get primary key -- get value    
    vector<string>  & pks = m_metaSeri->GetTypePrimaryKey(pMetaObj->GetDescriptor());
    uint64_t    k = 0L;
    uint32_t  val = 0;
    if(pks.size() > 2)
    {
        LOG_FATAL("res table agent not support too much primary keys great than 2 !");
        assert(false);
        return 0;
    }
    for(uint i = 0;i < pks.size(); ++i)
    {
        m_metaSeri->GetObjectFieldU32(pMetaObj,pks[i],val);
        k <<= 32;
        k |= val;
    }
    return k;
}

int       ResTableAgent::Init(const char* pszResDataDir,MetaSerializer * meta)
{
    m_metaSeri = meta;
    m_strResDataDir = pszResDataDir;   
    return ReLoad();
}
int       ResTableAgent::ReLoad()
{
    vector < string >  files;
    int ret =  File::ListFiles(files,m_strResDataDir.c_str());
    if(ret)
    {
        LOG_ERROR("list files error !");
        return -1;
    }    
    ///////////////////////////////////////////////
    m_mpAllNameTables.clear();
    m_iLoadResNum = 0;
    m_mpNameCacheTable.clear();
    m_mpRawMetaResTable.clear();

    //////////////////////////////////////////////
    string metaNameSpace = "res";
    //12MB
    Buffer  unpackBuffer;
    #define MAX_TABLE_FILE_SIZE     (12*1024*1024)
    ret = unpackBuffer.Create(MAX_TABLE_FILE_SIZE);
    if(ret)
    {
        LOG_ERROR("create buffer error !");
        return -1;
    }
    for(uint i = 0;i < files.size(); ++i)
    {
        string filePath = m_strResDataDir + "/" + files[i];
        int     buffLen = File::GetFileSize(filePath.c_str());
        if(buffLen <= 0)
        {
            LOG_ERROR("get file = %s size error !",filePath.c_str());
            unpackBuffer.Destroy();
            return -1;
        }
        unpackBuffer.iUsed = buffLen;
        ////////////////////////////////////
        string  strResTableType =   StringUtil::SplitString(files[i],".")[0];
        File    file(filePath.c_str(),"r");
        file.Read((char*)unpackBuffer.pBuffer,buffLen);
        string strObjType = metaNameSpace+"."+strResTableType;
        void * pObjTable = NULL;
        ret = m_metaSeri->UnPack(strObjType.c_str(),unpackBuffer,&pObjTable);
        file.Close();
        if(ret)
        {
            unpackBuffer.Destroy();        
            LOG_ERROR("return obj type = %s error !",strObjType.c_str());
            return -1;
        }
        MetaSerializer::MetaObjectPtr  ptr((MetaSerializer::MetaObject*)pObjTable);
        m_mpRawMetaResTable[strResTableType] =  ptr;
        ///////////////////////////////////////////////////////////////////////////////////
        MetaSerializer::MetaObject* pMetaObjTable = (MetaSerializer::MetaObject*)pObjTable;
        string listFieldName = "list";
        const FieldDescriptor * pListFieldDesc = pMetaObjTable->GetDescriptor()->FindFieldByName(listFieldName);    
        int   nObjTableEntryNum = pMetaObjTable->GetReflection()->FieldSize(*pMetaObjTable,pListFieldDesc);
        if(nObjTableEntryNum <= 0)
        {
            LOG_ERROR("obj table = %s list obj num = %d",strObjType.c_str(),nObjTableEntryNum);            
        }
        MapResTable & resTable = m_mpAllNameTables[strResTableType];
        for(int j = 0; j < nObjTableEntryNum; ++j)
        {
            const MetaSerializer::MetaObject* pTableListEntryObj = &(pMetaObjTable->GetReflection()->GetRepeatedMessage(*pMetaObjTable,pListFieldDesc,j));
            ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            //get key
            uint64_t    ulKey = GetResTableEntryKey(pTableListEntryObj);
            if(m_mpAllNameTables[strResTableType].find(ulKey) != m_mpAllNameTables[strResTableType].end())
            {
                LOG_FATAL("table = %s primary key1 = %u key2 = %u is repeatly !!!",
                          strObjType.c_str(),(uint32_t)(ulKey>>32),(uint32_t)ulKey);
                unpackBuffer.Destroy();      
                return -1;
            }
            resTable[ulKey]  = pTableListEntryObj;
            ++m_iLoadResNum;
        }
    }
    unpackBuffer.Destroy();      
    return 0;
}
#endif


#if 1
const void*     ResTableAgent::Find(const char* pszName,void*  obj)
{
    string name = pszName;
    MapNameResTableItr  itr = m_mpAllNameTables.find(name);
    if(itr != m_mpAllNameTables.end())
    {
        uint64_t   ulKey = GetResTableEntryKey(obj);
        MapResTableItr tblItr = itr->second.find(ulKey);
        if( tblItr != itr->second.end())
        {
            return tblItr->second;
        }        
    }    
    return NULL;
}
const void*     ResTableAgent::LowerBound(const char* pszName,void*    obj)
{
    string name = pszName;
    MapNameResTableItr  itr = m_mpAllNameTables.find(name);
    if(itr != m_mpAllNameTables.end())
    {
        uint64_t   ulKey = GetResTableEntryKey(obj);
        MapResTableItr tblItr = itr->second.lower_bound(ulKey);
        if( tblItr != itr->second.end())
        {
            return tblItr->second;
        }        
    } 
    return NULL;
}
const void*     ResTableAgent::UpperBound(const char* pszName,void*    obj)
{
    string name = pszName;
    MapNameResTableItr  itr = m_mpAllNameTables.find(name);
    if(itr != m_mpAllNameTables.end())
    {
        uint64_t   ulKey = GetResTableEntryKey(obj);
        MapResTableItr tblItr = itr->second.upper_bound(ulKey);
        if( tblItr != itr->second.end())
        {
            return tblItr->second;
        }        
    } 
    return NULL;
}
const void*     ResTableAgent::Random(const char* pszName)
{
    vector<const void*>   vec = GetAll(pszName);
    if(!vec.empty())
    {
        return vec[rand()%vec.size()];
    }    
    return NULL;
}
vector<const void*> &  ResTableAgent::GetAll(const char* pszName)
{
    //cahce
    string name = pszName;
    MapNameCacheTableItr vecCacheItr = m_mpNameCacheTable.find(name);
    if(vecCacheItr != m_mpNameCacheTable.end())
    {
        return vecCacheItr->second;
    }    
    MapNameResTableItr  itr = m_mpAllNameTables.find(name);
    vector<const void*>  & vecObj = m_mpNameCacheTable[name];
    vecObj.clear();
    if(itr != m_mpAllNameTables.end())
    {        
        MapResTableItr tblItr = itr->second.begin();
        while(tblItr != itr->second.end())
        {
            vecObj.push_back(tblItr->second);
            ++tblItr;
        }
    } 
    return vecObj;
}
#endif

