#pragma once
#include "base/stdinc.h"
#include "base/Buffer.h"
#include "DataListener.h"
#include "RedisAgent.h"

struct CacheAgentOption
{
    vector<RedisAddr>   addrList;
};

class MemSerializer;

class CacheAgent
{
public:
    int  Init(const CacheAgentOption  & cao,MemSerializer * seri);
    int  Polling(int    iProcPerTick);
    int  Destroy();
    int  AddCacheListener(string typeName,DataListenerPtr ptr);
public:
    int  Get(void * obj,const Buffer & cb);
    int  Remove(void * obj,const Buffer & cb);
    int  Insert(void * obj,const Buffer &  cb);       
    int  Update(void * obj,const Buffer &  cb);   
    int  Exists(void*  obj,const Buffer &  cb);
public:
    int  DispatchResult(string & cmd,string & type,string & key,
                    redisReply* reply,Buffer & cb,bool timeout); 
protected:
    int  GetKey(void* obj,string & key);
public:
    CacheAgent():serializer(NULL){}
private:
    MemSerializer   *   serializer;
    std::map<string,DataListenerPtr>  m_mpCacheListener;
    RedisAgent  *       redis;
};
