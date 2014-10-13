#pragma once

#if 0
#include "base/stdinc.h"
#include "base/Buffer.h"
#include "algorithm/HashTable.h"
#include "MetaSerializer.h"
#include "DataListener.h"
#include "ShareMemoryCenter.h"
#include "CacheAgent.h"
#include "DBAgent.h"



enum
{
    DATA_TYPE_RAW  = 0,
    DATA_TYPE_HASH = 1,
    DATA_TYPE_MEMPOOL ,
    DATA_TYPE_MAX ,
};

struct DataSetEntry
{
    void *   pData;
    ///////////////////////////
    int      iDirty;
    uint32_t tLastModifiedTime;
    uint32_t tLastUpdateTime;
    ///////////////////////////
public:
    void construct()
    {
        bzero(this,sizeof(*this));
    }
};


struct  LocalCacheRegParam
{
    string  setName;

    //set meta(entry)
    int             setType;
    string          metaName;
    int             iMaxNum;//max number
    PFNHashCode     hashCode;
    PFNHashCompare  hashCmp;

    DataListenerPtr ptrListener;
    //remote cache
    bool     bNeedRemoteCache;
    //persistence
    bool     bNeedPersistence;    
    //sync policy
    int      iSyncPeriod;
    int      iSyncPeriodNum;
public:
    void    construct()
    {
        setName = metaName = "";
        setType = DATA_TYPE_HASH;
        iMaxNum = iSyncPeriod = iSyncPeriodNum =  0;
        hashCode = NULL;
        hashCmp = NULL;
        bNeedRemoteCache = bNeedPersistence = false;
        
    }
};

struct CacheOption;
struct DBOption;
struct ShareMemCenterOption;

class DataCenter;
class DataCenterShmCenterNotifier: public ShareMemoryModuleNotifier
{
public:
    void    SetAgent(DataCenter* _p){agent = _p;}
    int     OnAttach(const char* pszName,char* pData,size_t size,char chInited);
private:
    DataCenter*  agent;
};

/*
usage (order):
DataCenter   da;
    da.InitMeta();
    da.Register(); //---- many sets
    da.Init();
    da.Polling();
    da.Destroy();
*/
    
class DataCenter
{        
public:
    int     InitMeta()
    {
        /*
        metaSerializer.Init(const char * pszNameSpace, const char * pszMetaFileName = NULL);
        */
        return 0;
    }
    int     Init(const CacheOption & co , const DBOption & dbo , const ShareMemCenterOption & ro)
    {
        /*        
        dbAgent.Init(const DBAgentOption & cao, MetaSerializer * seri);
        
        cacheAgent.Init(const CacheAgentOption & cao, MetaSerializer * seri);

        shmCenter.Init(const char * pszShmKeyPath);

        */
        dataAgentMemCenterNotifier.SetAgent(this);
        return 0;
    }
    int     Polling(int iProcPerTick)
    {
        cacheAgent.Polling(iProcPerTick);
        dbAgent.Polling(iProcPerTick);
        AutoWriteBack();
        return 0;
    }
    void    Destroy()
    {
        //todo
    }
    
    void    AutoWriteBack()
    {
        //todo
    }

    //entry meta name
    //set type
    //register meta set     
    int     RegisterLocalCache(LocalCacheRegParam & param)
    {
        if(setRegParamName.find(param.setName) != setRegParamName.end())
        {
            LOG_FATAL("reg param name = %s repeatly !",param.setName);
            return -1;
        }
        ShareMemoryModuleReg    reg;
        reg.name = param.setName;
        size_t zMetaEntrySize = metaSerializer.GetMaxMetaObjSize(param.metaName.c_str());
        if(0 == zMetaEntrySize)
        {
            LOG_ERROR("register meta name = %s get max obj size error !",param.metaName.c_str());
            return -1;
        }
        size_t  zTotalSize = 0;
        switch(param.setType)
        {
            case DATA_TYPE_RAW:
                zTotalSize = param.iMaxNum*zMetaEntrySize;
                break;
            case DATA_TYPE_HASH:
                zTotalSize =  HashTable::GetBufferSize(zMetaEntrySize,param.iMaxNum);
                break;
            case DATA_TYPE_MEMPOOL:
                zTotalSize =  MemPool::GetBufferSize(zMetaEntrySize,param.iMaxNum);
                break;
            default:
                LOG_ERROR("not suupport type = %d",param.setType);
                return -1;
        }
        reg.zSize = zTotalSize;
        reg.notifier = &dataAgentMemCenterNotifier;
        int ret = shmCenter.Register(reg);
        if(ret)
        {
            return -1;
        }
        vecRegParam.push_back(param);
        return 0;
    }
public:
    static const char* GetErrorString(int err);

public:    
    vector<LocalCacheRegParam>  vecRegParam;
    std::set<string>            setRegParamName;
     MetaSerializer              metaSerializer;
    ShareMemoryCenter           shmCenter;
    CacheAgent                  cacheAgent;
    DBAgent                     dbAgent;    
    DataCenterShmCenterNotifier dataAgentMemCenterNotifier;

private:
    DeclareDefaultPrivateConstructor(DataCenter)
    DeclareSingltonSupport(DataCenter)
};

#endif

