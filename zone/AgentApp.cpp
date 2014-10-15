#include "AgentApp.h"


#if 1
void    AgentContext::OnGenerateDefaultConfig()
{
}
int     AgentContext::OnInit()
{
    return 0;
}
#endif


#if 1
int     AgentApp::StartDB()
{
    DBAgentOption   cao;
    //AgentContext    * agent_ctx = (AgentContext*)GetContext();
    /////////////////////////////////////////////
    //db - register db tables
    MysqlProxyOption    mpo;
    // for testing , it should read from config
    mpo.dbname = "mtest";
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
    //sth need destroy .
    return 0;
}    
#endif

