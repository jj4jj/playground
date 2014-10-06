#include "base/Log.h"
#include "base/Buffer.h"
#include "dataagent/MemSerializer.h"
#include "dataagent/CacheAgent.h"
#include "proto/meta/meta.pb.h"
#include "proto/meta/test.pb.h"


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

    CacheAgent  ca;
    CacheAgentOption    cao;
        
    if(ca.Init(cao,&seri))
    {

        LOG_ERROR("cache agent init error !");
        return -1;
    }    
    ca.Get(&cache,Buffer());
    return 0;
};
