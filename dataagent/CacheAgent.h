#pragma once
#include "base/stdinc.h"
#include "base/Buffer.h"
#include "DataListener.h"
#include "RedisAgent.h"
#include "MetaSerializer.h"
#include "DataCommon.h"



struct CacheAgentOption
{
    vector<RedisAddr>   addrList;
};

class MetaSerializer;

class CacheAgent
{
public:
    int  Init(const CacheAgentOption  & cao,MetaSerializer * seri);
    int  Polling(int    iProcPerTick);
    int  Destroy();
    int  AddListener(string typeName,DataListenerPtr ptr);
public:
    //key value
    
    int  Get(void * obj,const Buffer & cb);
    int  Remove(void * obj,const Buffer & cb);
    int  Insert(void * obj,const Buffer &  cb);       
    int  Update(void * obj,const Buffer &  cb);   

    //////////////////////////////////////////////////////////////////////////////////////

    // extend key
    int  Exists(void*  obj,const Buffer &  cb);
    int  Expire(void*  key,int  secdons,const Buffer cb);
    int  MGet(void ** obj,int count,const Buffer & cb);
    int  MSet(void ** obj,int count,const Buffer & cb);

    //////////////////////////////////////////////////////////////////////////////////////

    //key hash
    int  HGet(string & strHashName,void* obj,Buffer & cb);
    int  HSet(string & strHashName,void* obj,Buffer & cb);
    int  HDel(string & strHashName,void* obj,Buffer & cb);
    int  HInsert(string & strHashName,void* obj,Buffer & cb);

    int  HMSet(string & strHashName,void** obj,int count,Buffer & cb);
    int  HMGet(string & strHashName,void** obj,int count,Buffer & cb);
    int  HMInsert(string & strHashName,void** obj,int count,Buffer & cb);


    //////////////////////////////////////////////////////////////////////////////////////

    //key zset
    int  ZAdd(string & strZSetName,int64_t score,void* obj,Buffer & cb);
    int  ZDel(string & strZSetName,void * obj,Buffer & cb);
    //redis > 2.4
    int  ZMDel(string & strZSetName,void ** obj,int count,Buffer & cb);
    //[start,stop]
    int  ZRangeDelWithRank(string & strZSetName,int start,int stop,Buffer & cb);
    //[min,max
    int  ZRangeDelWithScore(string & strZSetName,int64_t minSocre,int64_t maxSocre,Buffer & cb);
    int  ZCard(string & strZSetName,Buffer & cb);
    int  ZScore(string & strZSetName,void* obj,Buffer & cb);
    //[min,max] 
    int  ZCount(string & strZSetName,int64_t minScore,int64_t maxScore,Buffer & cb);
    //[score += change]
    int  ZChange(string & strZSetName,int64_t change,void* obj,Buffer & cb);
    // zrange - index (sort by score)
    int  ZRangeWithRank(string & strZSetName,int start,int stop,int count,Buffer & cb);
    // ZRANGEBYSCORE 
    // referrence to http://www.redisdoc.com/en/latest/sorted_set/zrangebyscore.html
    int  ZRangeWithScore(string & strZSetName,
        int64_t minScore,int64_t maxScore,
        int limit,int offset,int count,Buffer & cb);
    int  ZRank(string & strZSetName,void * obj,Buffer & cb,bool bOrderByDesc = true);
    

    //////////////////////////////////////////////////////////////////////////////////////
    //key list
    
    //0 1 2 3 ... N-1
    //-N+1 -N+2 ... -1
    int  LGet(string & strListName,int index,Buffer & cb);
    int  LSet(string & strListName,int index,void* obj,Buffer & cb);    
    int  LDel(string & strListName,void* obj,int count,Buffer & cb);
    //push object to list back - rpushx
    int  LPush(string & strListName,void * obj,Buffer & cb);
    int  LMPush(string & strListName,void ** obj,int count,Buffer & cb);
    //
    int  LPop(string & strListName,Buffer & cb);
    int  LLen(string & strListName,Buffer & cb);
    //[start,end] , close segments
    int  LRange(string & strListName,int start,int end,Buffer & cb);
    


public:
    int  DispatchResult(const CacheAgentCallBack & mcb,
                    redisReply* reply,Buffer & ucb,bool timeout); 
    MetaSerializer::MetaObject*    FindObject(const string & key);
    void           FreeObject(const string & key);
    int     CreateCallBack(Buffer & resultCB,CacheCollectionType type,
            CacheCallBackTypeCmd cmd,
            const string & typeKey,
            const Buffer * pCustomCB = NULL,const char * pszObjKey  = NULL );
    void    FreeCallBack(Buffer & cb);    
protected:
    int  GetObjectKey(void* obj,string & key);
    int  GetTypeKey(void* obj,string & key);
    MetaSerializer::MetaObject*    FindObject(MetaSerializer::MetaObject * obj);
public:
    CacheAgent():serializer(NULL){}
private:
    MetaSerializer   *   serializer;
    std::map<string,DataListenerPtr>  m_mpListener;
    RedisAgent  *       redis;
    unordered_map<string,MetaSerializer::MetaObjectPtr>  m_mpGetObjects;
};
