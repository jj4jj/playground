#include "base/StringUtil.h"
#include "proto/gen/res/include.h"
#include "proto/gen/db/include.h"
#include "proto/gen/cs/include.h"
#include "proto/gen/ss/include.h"
#include "proto/gen/conf/include.h"
#include "utility/Coroutine.h"
#include "AgentApp.h"

#if 1
/*
default addr arrangement:

agent
    51XXX
    channel listen
        51010
        51011
        51012
gate
    52XXX  
    channel listen
        52010
    tcp server listen
        52100
        52101
        52102        
console
    XX000
        XX000
account
    56XXX
platform
    58XXX

*/



void    AgentContext::OnGenerateDefaultConfig()
{
    //default
    static const char * kv[][2] = {
    //gate server
    {"agent:gate:num","1"},
    {"agent:gate#1:channel","1"},

    {"agent:db:num","1"},
    {"agent:db#1:ip","127.0.0.1"},
    {"agent:db#1:port","3306"},
    {"agent:db#1:user","root"},
    {"agent:db#1:passwd","123456"},
    {"agent:db#1:dbname","rgtd"},
        
    {"agent:cache:num","1"},
    {"agent:cache#1:ip","127.0.0.1"},
    {"agent:cache#1:port","6379"},
    {"agent:cache#1:dbidx","0"},
            
    {"agent:area:num","5"},
    {"agent:area:ids","1 2 3 4 5"},


    //default channel
    {"console:port","51000"},
    {"channel:num","1"},
    {"channel:local:addr","tcp://127.0.0.1:51010"},
    {"channel:local:name","agent#1"},
    {"channel:info#1:addr","-"},
    {"channel:info#1:name","gate#1"},
    /////////////add default config above////////////////
    {NULL,NULL}};
    int i = 0;

    ConfigValue v;
    while(kv[i][0])
    {
        v.key  = kv[i][0];
        v.value  = kv[i][1];
        parser.CreateConfig(v);
        ++i;
    }    
}
int     AgentContext::OnInit()
{
    //get some config value   
    /*
    {"agent:gate:num","1"},
    {"agent:gate#1:channel","1"},

    {"agent:db:num","1"},
    {"agent:db#1:ip","127.0.0.1"},
    {"agent:db#1:port",""},
    {"agent:db#1:user","root"},
    {"agent:db#1:passwd","123456"},
    {"agent:db#1:dbname","mtest"},
        
    {"agent:cache:num","1"},
    {"agent:cache#1:ip","127.0.0.1"},
    {"agent:cache#1:port",""},
    {"agent:cache#1:dbidx","0"},
            
    {"agent:area:num","2"},
    {"agent:area:ids","1 2 "},
    */
    int n = parser.GetConfigInt("agent:gate:num");
    MysqlProxyOption    mpo;
    RedisAddr           redis;
    char    szConfigBuffer[64];
    for(int i = 0;i < n; ++i)
    {
        snprintf(szConfigBuffer,sizeof(szConfigBuffer),"agent:db#%d:ip",i+1);
        mpo.ip = parser.GetConfigString(szConfigBuffer);
        snprintf(szConfigBuffer,sizeof(szConfigBuffer),"agent:db#%d:port",i+1);
        mpo.port = parser.GetConfigInt(szConfigBuffer);
        snprintf(szConfigBuffer,sizeof(szConfigBuffer),"agent:db#%d:user",i+1);
        mpo.uname = parser.GetConfigString(szConfigBuffer);
        snprintf(szConfigBuffer,sizeof(szConfigBuffer),"agent:db#%d:passwd",i+1);
        mpo.passwd = parser.GetConfigString(szConfigBuffer);
        snprintf(szConfigBuffer,sizeof(szConfigBuffer),"agent:db#%d:dbname",i+1);
        mpo.dbname = parser.GetConfigString(szConfigBuffer);
        dbs.push_back(mpo);
    }
    n = parser.GetConfigInt("agent:cache:num");
    for(int i = 0;i < n; ++i)
    {
        snprintf(szConfigBuffer,sizeof(szConfigBuffer),"agent:cache#%d:ip",i+1);
        redis.ip = parser.GetConfigString(szConfigBuffer);
        snprintf(szConfigBuffer,sizeof(szConfigBuffer),"agent:cache#%d:port",i+1);
        redis.port = parser.GetConfigInt(szConfigBuffer);
        snprintf(szConfigBuffer,sizeof(szConfigBuffer),"agent:cache#%d:dbidx",i+1);
        redis.dbidx = parser.GetConfigInt(szConfigBuffer);
        dbs.push_back(mpo);
    }
    int     area_num[128];
    n = parser.GetConfigInt("agent:area:num");
    string sValue = parser.GetConfigString("agent:area:ids");
    int real_num =  StringUtil::SplitNum(sValue.c_str(),area_num,128," ");
    if(real_num != n)
    {
        LOG_WARN("agent area num = %d but real num = %d ",n,real_num);
    }
    areas.clear();
    for(int i = 0;i < n; ++i)
    {
        areas.insert(area_num[i]);
    }
    if(areas.empty())
    {
        LOG_FATAL("area ids is empty !");
        return -1;
    }

    n = parser.GetConfigInt("agent:gate:num");
    //{"agent:gate:num","1"},
    //{"agent:gate#1:channel","1"},
    
    gates.clear();
    for(int i = 0 ; i < n ; ++i)
    {
        snprintf(szConfigBuffer,sizeof(szConfigBuffer),"agent:gate#%d:channel",i+1);
        int chid = parser.GetConfigInt( szConfigBuffer);
        if(chid == 0)
        {
            LOG_FATAL("get config = %s = 0",szConfigBuffer);
            return -1;
        }
        gates.insert(chid);
    }
    
    ////////////////////////////////////////////////////////////////////////////////


    
    return 0;
}
#endif


#if 1
int     AgentApp::StartDB()
{
    db::init_proto_dummy();

    DBAgentOption   cao;
    //AgentContext    * agent_ctx = (AgentContext*)GetContext();
    /////////////////////////////////////////////
    //db - register db tables
    MysqlProxyOption    mpo;
    // for testing , it should read from config
    mpo.dbname = "rgtd";
    mpo.ip = "127.0.0.1";
    mpo.port = 3306;
    mpo.uname = "root";
    mpo.passwd = "123456";
    mpo.unisock = "";    
    cao.addrList.push_back(mpo);
    
    return db.Init(cao,&meta);
}
int     AgentApp::StartCache()
{

    db::init_proto_dummy();

    
    CacheAgentOption    cao;

    //just for test , it should read from config
    RedisAddr   ra;
    ra.dbidx = 0;
    ra.ip = "127.0.0.1";
    ra.port = 6379;
   
    cao.addrList.push_back(ra);
    return cache.Init(cao,&meta);
}
int     AgentApp::StartShmCenter()
{
    //todo - init with config path
    return shmCenter.Init("agent.shm");
}
int     AgentApp::InitZoneMgr()
{
    return zoneMgr.Init();
}
int     AgentApp::InitScript()
{
    //todo

    return 0;
}
int     AgentApp::InitResTable()
{
    res::init_proto_dummy();
    //"../res" is temporary . should read from config
    return resTable.Init("../res",&meta);
}

#endif


#if 1
//return 0 is ok , otherwise exit prcess
int     AgentApp::OnInit()
{
    int ret = 0;
    //read config    
    //meta
    //default co size
    //160MB - 20480
    //1024 - 8M
    ret = CoroutineMgr::Instance().Init(1024);
    if(ret)
    {
        LOG_ERROR("coroutine init error !");
        return -1;
    }
    LOG_INFO("coroutine init success !");


    ret = meta.Init();
    if(ret)
    {
        LOG_ERROR("meta init error !");
        return -1;
    }

    //3rd platform service (sns,lbs)
    //rank
    
    //table
    ret = InitResTable();
    if(ret)
    {
        LOG_ERROR("table res init error !");
        return -1;
    }
        
    //script(listen msg and tick)
    ret = InitScript();
    if(ret)
    {
        LOG_ERROR("script init error !");
        return -1;
    }    

    //zone mgr
    ret = InitZoneMgr();
    if(ret)
    {
        LOG_ERROR("zone mgr init error !");
        return -1;
    }
    
    return 0;        
}
int     AgentApp::OnStart()
{
    int ret = 0;

    //share memory
    ret = StartShmCenter();
    if(ret)
    {
        LOG_ERROR("start init error !");
        return -1;
    }
    
    //db (mysql)
    ret = StartDB();
    if(ret)
    {
        LOG_ERROR("db start error !");
        return -1;
    }
    ret = StartCache();
    if(ret)
    {
        LOG_ERROR("cache start error !");
        return -1;
    }    
    return 0;
}

//control command process
string  AgentApp::OnCtrl(const std::vector<string> & cmdLine)
{
    string recvCMDS;
    for(uint i = 0;i < cmdLine.size(); ++i)
    {
        if(i == 1)
        {
            recvCMDS += " : ";
        }
        else if(i > 1)
        {
            recvCMDS += " & ";
        }
        recvCMDS += cmdLine[i];
    }
    LOG_INFO("recv cmd [%s]",recvCMDS.c_str());
    return string("todo");
}
//tick 
int     AgentApp::OnTick(int64_t lElapseTime)
{
    //sth need tick
    return 0;
}
//poll system
int     AgentApp::OnPoll(int iRecommendPollNum )
{
    db.Polling(iRecommendPollNum);
    cache.Polling(iRecommendPollNum);
    //wait for 2 ms
    zoneMgr.Polling(2);    
    //channel    
    return 0;
}
//system will close for closing Reason
int     AgentApp::OnClosing(int closingReason)
{
    LOG_INFO("agent is closing for reason = %d ...",closingReason);
    int iRet = 0;
    switch(closingReason)
    {
        case APP_CTRL_CLOSING_RESTART:
            //bakup mem
            break;
        case APP_CTRL_CLOSING_STOP:
            //stop sth
            break;
        case APP_CTRL_CLOSING_COREDUMP:
            //core dump
            break;
        case APP_CTRL_CLOSING_EXCEPTION:
        default:
            return 0;
    }
    return iRet;
}
//destroy sth uninitializing
int     AgentApp::OnDestroy()
{
    zoneMgr.Destroy();
    //sth need destroy .
    db.Destroy();
    cache.Destroy();
    return 0;
}    
#endif

