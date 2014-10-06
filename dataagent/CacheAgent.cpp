#include "base/Log.h"
#include "MemSerializer.h"
#include "CacheAgent.h"
using namespace google::protobuf;

int  CacheAgent::Init(const CacheAgentOption  & cao,MemSerializer * seri)
{
        

    serializer = seri;
    return 0;
}
int  Polling(int    iProcPerTick)
{
    //polling redis client

    
    return 0;
}
int  Destroy()
{
    return 0;
}

int  CacheAgent::AddCacheListener(string typeName,DataListenerPtr ptr)
{
    if(m_mpCacheListener[typeName])
    {
        LOG_FATAL("repeat add cache listener = %s",typeName.c_str());
        return -1;
    }
    m_mpCacheListener[typeName] = ptr;
    return 0;
}













int  CacheAgent::GetKey(void* obj,string & key)
{
    Message * msg = (Message*) obj;
    const Descriptor* desc = msg->GetDescriptor();
    std::vector<string> &   pks = serializer->GetTypePrimaryKey(desc);
    if(pks.empty())
    {
        return -1;
    }
    //key:type#key&key
    key = desc->name()+":";
    for(uint i = 0; i < pks.size(); ++i)
    {
        if(i > 0)
        {
            key += "&";
        }        
        key += pks[i];
    }
    return 0;
}
int  CacheAgent::Get(void * obj,const Buffer & cb)
{
    //gen key
    string key ;
    if(GetKey(obj,key))
    {
        return -1;
    }
    string cmd = "GET "+key;
    LOG_INFO("cache exec cmd = %s",cmd.c_str());
    //todo
    //redis client
    return 0;
}
int  CacheAgent::Remove(void * obj,const Buffer & cb)
{
    //gen key
    string key ;
    if(GetKey(obj,key))
    {
        return -1;
    }
    string cmd = "REMOVE "+key;
    LOG_INFO("cache exec cmd = %s",cmd.c_str());
    //todo
    //redis client
    return 0;
}
int  CacheAgent::Insert(void * obj,const Buffer &  cb)
{
    //gen key
    string key ;
    if(GetKey(obj,key))
    {
        return -1;
    }
    string cmd = "INSERT "+key;
    //buffer to string.
        

    LOG_INFO("cache exec cmd = %s",cmd.c_str());
    //todo
    //redis client
    return 0;
}
int  CacheAgent::Update(void * obj,const Buffer &  cb)
{
    //gen key
    string key ;
    if(GetKey(obj,key))
    {
        return -1;
    }
    string cmd = "SET "+key;
    //buffer to string.
    
    LOG_INFO("cache exec cmd = %s",cmd.c_str());
    //todo
    //redis client
    return 0;
}

