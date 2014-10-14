#include "datacenter/DBAgent.h"
#include "proto/gen/test/test.pb.h"
#include "base/Log.h"


class TestDBListener : public DataListener
{
public:    
    int   OnGet(int ret,void* obj,Buffer & cb)
    {
        LOG_DEBUG("db Test");
        if(ret == DATA_OK)
        {
            MetaSerializer::MetaObject*  mobj = (MetaSerializer::MetaObject*)obj;
            LOG_DEBUG(mobj->DebugString().c_str());
        }
        else if(ret == DATA_NO_EXISTS)
        {
            LOG_DEBUG("not exist !");
        }
        else if(ret == DATA_ALREADY_EXISTS)
        {
            LOG_DEBUG("already exist !");
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
    DBAgent db;
    DBAgentOption   cao;


    MysqlProxyOption option;
    option.cliflag = 0;
    option.dbname = "mtest";
    option.ip = "127.0.0.1";
    option.port = 3306;
    option.uname = "root";
    option.passwd = "123456";
    option.unisock = "";

    cao.addrList.push_back(option);
    cao.tableTypes.push_back("Cache");
    
    MetaSerializer   ser;
    if(ser.Init())
    {
        return -1;
    }
    db.AddListener(string("Cache"),DataListenerPtr(new TestDBListener()));
    if(db.Init(cao,&ser))
    {
        return -1;
    }
    db.CreateTable("Cache");
    
    test::Cache ch;

    test::Cache cache;
    cache.set_name(string("seanpeng"));
    cache.set_area(20);
    cache.set_gid(234526);

    LOG_DEBUG("TYPE NAME = %s",cache.GetTypeName().c_str());
    
    vector < string >  fields;
    Buffer cb;
    int x = 0;
    while(true)
    {
        db.Polling(10);
        usleep(10000);
        std::cin>>x;
        string dbs;
        ser.Visual((void*)&cache,dbs);
        LOG_DEBUG("dbg view:%s",dbs.c_str());
        if(x == 0)
        {
            db.Remove(&cache,cb);
        }
        else if(x == 1)
        {
            db.Insert(&cache,cb);
        }
        else if(x == 2)
        {            
            cache.set_gid(2222+rand()%10);
            db.Update(&cache,fields,cb);
        }
        else if(x == 4)
        {            
            fields.push_back("name");
            db.Get(&cache,fields,cb);
        }        
        else if(x == 5)
        {            
            fields.clear();
            fields.push_back("area");
            fields.push_back("gid");
            fields.push_back("name");
            db.Get(&cache,fields,cb);
        }  
        else if(x == 6)
        {            
            fields.clear();
            fields.push_back("gid");
            db.Get(&cache,fields,cb);
        }  
    }
    return 0;
};

