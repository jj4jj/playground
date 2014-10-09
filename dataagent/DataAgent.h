#pragma once

#include "base/stdinc.h"
#include "base/Buffer.h"
#include "algorithm/HashTable.h"
#include "MetaSerializer.h"
#include "DataListener.h"
#include "RecoveryShareMem.h"
#include "CacheAgent.h"
#include "DBAgent.h"

enum
{
    DATA_TYPE_PURE_LOCAL_CACHE = 1,
    DATA_TYPE_REMOTE_CACHE ,
    DATA_TYPE_REMOTE_CACHE_AND_DB ,
    DATA_TYPE_PURE_DB ,
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
    int         setType;
    string      metaName;
    int         iMaxNum;//max number
    PFNHashCode     hashCode;
    PFNHashCompare  hashCmp;

    //remote cache
    bool     bNeedRemoteCache;
    //persistence
    bool     bNeedPersistence;    
    //sync policy
    int      iSyncPeriod;
    int      iSyncPeriodNum;

};



//entry is protobuffer type
class DataSet
{
public:
    enum
    {
        RINPROGRESS = 0x7F,
    };
public:
    DataSet();
    virtual ~DataSet();
public:
    //return EINPROGRESS  if insert to remote
    //return 0 is insert ok ( pure local cache)
    //otherwise return error code
    int     Insert(void* entry,const Buffer & cb);
    int     Remove(void* entry,const Buffer & cb);
    int     Update(void* entry ,const Buffer & cb,vector<string> * pFields = NULL);
    
    //if local cache (pure)
    //if finded return addr
    //if remote return EINPROGRESS
    //if not found , return NULL
    void*   Find(void* entry , const Buffer & cb,bool justFindLocal = false);
private:
    LocalCacheRegParam  option;
    //
    //meta void * 
    //local cache
    HashTable       ht;//size_t(u32) -> entry[void* p,etc]
    PFNHashCode     hashCode;
    PFNHashCompare  hashCmp;
    //remote cache
    MetaSerializer * memSerializer;
    CacheAgent*     cacheAgent;
    DBAgent*        dbAgent;
    //
    int             updatesPeriod;
    uint32_t        dwLastUpdateTime;
    int             iPeriodTime;
    int             iLastUpdateIdx;
};



struct CacheOption;
struct DBOption;
struct RecoveryShareMemOption;
    
class DataAgent
{
public:
    int     LoadMeta();
    int     Init(const char* pszMetaFileName, const CacheOption & co , const DBOption & dbo , const RecoveryShareMemOption & ro);
    //entry meta name
    //set type
    //register meta set     
    DataSet &  RegisterLocalCache(LocalCacheRegParam & param,DataListener * listener = NULL);
public:
    static const char* GetErrorString(int err);
    
public:    
    std::map<string,DataSet>    datasets;
    MetaSerializer               memSerializer;
    RecoveryShareMem            recoveryShareMem;
    CacheAgent                  cacheAgent;
    DBAgent                     dbAgent;    
};




