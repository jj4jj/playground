#pragma once
#include "base/stdinc.h"
#include "base/Buffer.h"
#include "DataListener.h"
#include "RedisAgent.h"
#include "MemSerializer.h"



struct CacheAgentOption
{
    vector<RedisAddr>   addrList;
};
enum    CacheResult
{
    CACHE_TIME_OUT      =  -1,
    CACHE_OK            =   0,
    CACHE_NO_EXISTS         ,
    CACHE_ALREADY_EXISTS    ,
    CACHE_REDIS_ERROR       ,
    CACHE_UNPACK_ERROR      ,
    CACHE_INTERNAL_ERR      ,
};


class MemSerializer;

class CacheAgent
{
public:
    int  Init(const CacheAgentOption  & cao,MemSerializer * seri);
    int  Polling(int    iProcPerTick);
    int  Destroy();
    int  AddListener(string typeName,DataListenerPtr ptr);
public:
    int  Get(void * obj,const Buffer & cb);
    int  Remove(void * obj,const Buffer & cb);
    int  Insert(void * obj,const Buffer &  cb);       
    int  Update(void * obj,const Buffer &  cb);   
    int  Exists(void*  obj,const Buffer &  cb);
public:
    int  DispatchResult(string & cmd,string & type,string & key,
                    redisReply* reply,Buffer & cb,bool timeout); 
    MemSerializer::MetaObject*    FindObject(const string & key);
    void           FreeObject(const string & key);
protected:
    int  GetKey(void* obj,string & key);
    MemSerializer::MetaObject*    FindObject(MemSerializer::MetaObject * obj);
public:
    CacheAgent():serializer(NULL){}
private:
    MemSerializer   *   serializer;
    std::map<string,DataListenerPtr>  m_mpListener;
    RedisAgent  *       redis;
    unordered_map<string,MemSerializer::MetaObjectPtr>  m_mpGetObjects;
};
