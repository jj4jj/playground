#include "base/Log.h"
#include "base/Buffer.h"
#include "dataagent/MemSerializer.h"
#include "dataagent/CacheAgent.h"
#include "proto/meta/meta.pb.h"
#include "proto/meta/test.pb.h"
class TestCacheListener : public DataListener
{
public:    
    int   OnGet(redisReply* reply,Buffer & cb,bool timeout)
    {
        DataListener::OnGet(reply,cb,timeout);
        LOG_DEBUG("Cache Test");
        return 0;
    }
    int   OnInsert(redisReply* reply,Buffer & cb,bool timeout)
    {
        DataListener::OnGet(reply,cb,timeout);
        LOG_DEBUG("Cache Test");
        return 0;
    }
    int   OnRemove(redisReply* reply,Buffer & cb,bool timeout)
    {
        DataListener::OnGet(reply,cb,timeout);
        LOG_DEBUG("Cache Test");
        return 0;
    }
    int   OnUpdate(redisReply* reply,Buffer & cb,bool timeout)
    {
        DataListener::OnGet(reply,cb,timeout);
        LOG_DEBUG("Cache Test");
        return 0;
    }
};

int main(int argc,char* argv[])
{
    MemSerializer   seri;
    
    if(seri.Init())
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
    ca.AddCacheListener(string("Cache"), DataListenerPtr(new TestCacheListener()));
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
