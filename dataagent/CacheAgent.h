#pragma once
#include "base/stdinc.h"
#include "base/Buffer.h"
#include "DataListener.h"

struct CacheAgentOption
{


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
protected:
    int  GetKey(void* obj,string & key);
public:
    CacheAgent():serializer(NULL){}
private:
    MemSerializer   *   serializer;
    std::map<string,DataListenerPtr>  m_mpCacheListener;
    //redis batch client
    
    
};
