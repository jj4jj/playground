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
int     AgentApp::InitDB()
{
    DBAgentOption   cao;
    //AgentContext    * agent_ctx = (AgentContext*)GetContext();
    /////////////////////////////////////////////
    //db - register db tables
    cao.tableTypes.push_back("Role");
    cao.tableTypes.push_back("AccountR");
    cao.tableTypes.push_back("AccountL");
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
int     AgentApp::InitCache()
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
int     AgentApp::InitShmCenter()
{
    //todo
    return 0;
}
int     AgentApp::InitZoneMgr()
{
    //todo
    return 0;
}
int     AgentApp::InitScript()
{
    //todo

    return 0;
}
int     AgentApp::InitResTable()
{
    //todo
    


    return 0;
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

    //db (mysql)
    ret = InitDB();
    if(ret)
    {
        LOG_ERROR("db init error !");
        return -1;
    }
    ret = InitCache();
    if(ret)
    {
        LOG_ERROR("cache init error !");
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
    

    //share memory
    ret = InitShmCenter();
    if(ret)
    {
        LOG_ERROR("shm init error !");
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
    //datagent

    
    
    return 0;
}
//system will close for closing Reason
int     AgentApp::OnClosing(int closingReason)
{
    LOG_INFO("agent is closing for reason = %d ...",closingReason);
    if(closingReason == 1)
    {
        //restart 
        //backup memory 
        return 0;
    }
    else
    {
        return 0;
    }
}
//destroy sth uninitializing
int     AgentApp::OnDestroy()
{
    //sth need close .
    return 0;
}    
#endif

