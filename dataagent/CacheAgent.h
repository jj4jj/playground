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
    int  Get(void * obj,const Buffer & cb);
    int  Remove(void * obj,const Buffer & cb);
    int  Insert(void * obj,const Buffer &  cb);       
    int  Update(void * obj,const Buffer &  cb);   
    int  Exists(void*  obj,const Buffer &  cb);
public:
    int  DispatchResult(string & cmd,string & type,string & key,
                    redisReply* reply,Buffer & cb,bool timeout); 
    MetaSerializer::MetaObject*    FindObject(const string & key);
    void           FreeObject(const string & key);
protected:
    int  GetKey(void* obj,string & key);
    MetaSerializer::MetaObject*    FindObject(MetaSerializer::MetaObject * obj);
public:
    CacheAgent():serializer(NULL){}
private:
    MetaSerializer   *   serializer;
    std::map<string,DataListenerPtr>  m_mpListener;
    RedisAgent  *       redis;
    unordered_map<string,MetaSerializer::MetaObjectPtr>  m_mpGetObjects;
};
