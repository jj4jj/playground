#include "base/Log.h"
#include "base/Buffer.h"
#include "dataagent/MetaSerializer.h"
#include "dataagent/CacheAgent.h"
#include "proto/meta/meta.pb.h"
#include "proto/meta/test.pb.h"

class TestCacheListener : public DataListener
{
public:    
    int   OnGet(int ret,void* obj,Buffer & cb)
    {
        LOG_DEBUG("Cache Test");
        if(ret == CACHE_OK)
        {
            MetaSerializer::MetaObject*  mobj = (MetaSerializer::MetaObject*)obj;
            LOG_DEBUG(mobj->DebugString().c_str());
        }
        else if(ret == CACHE_NO_EXISTS)
        {
            LOG_DEBUG("NOT EXIST !");
        }
        else
        {
            LOG_DEBUG("get error = %d!",ret);
        }
            
        return 0;
    }
    int   OnRemove(int ret,Buffer & cb)
    {
        LOG_DEBUG("remove ret = %d",ret);
        return 0;
    }
    int   OnUpdate(int ret,Buffer & cb)
    {
        LOG_DEBUG("update ret = %d",ret);
        return 0;
    }
};

int main(int argc,char* argv[])
{
    MetaSerializer   seri;
    
    if(seri.Init("meta"))
    {
        LOG_ERROR("meta proto init error !");
        return -1;
    }

    meta::Cache cache;
    cache.set_name(string("seanpeng"));
    cache.set_area(20);
    cache.set_gid(234526);
    CacheAgent  ca;
    CacheAgentOption    cao;
    RedisAddr addr;
    addr.ip = "127.0.0.1";
    addr.port = 6379;
    cao.addrList.push_back(addr);
    cao.addrList.push_back(addr);    
    ca.AddListener(string("Cache"), DataListenerPtr(new TestCacheListener()));
    if(ca.Init(cao,&seri))
    {
        LOG_ERROR("cache agent init error !");
        return -1;
    }    
    ca.Get(&cache,Buffer("my get"));
    //ca.Insert(&cache,Buffer("my insert"));
    ca.Update(&cache,Buffer("my update"));
    ca.Remove(&cache,Buffer("my remove"));
    while(true)
    {
        ca.Polling(10);
    };

    return 0;
};
