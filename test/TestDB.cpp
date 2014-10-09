#include "dataagent/DBAgent.h"
#include "proto/meta/test.pb.h"
int main(int argc,char* argv[])
{
    DBAgent db;
    DBAgentOption   cao;
    MemSerializer   ser;
    ser.Init("meta");
    db.Init(cao,&ser);
    meta::Cache ch;

    meta::Cache cache;
    cache.set_name(string("seanpeng"));
    cache.set_area(20);
    cache.set_gid(234526);

    vector < string >  fields;
    Buffer cb;
    db.Get(&cache,fields,cb);

    return 0;
};

