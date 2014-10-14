#pragma once

#include "app/App.hpp"
#include "base/Log.h"
#include "datacenter/DBAgent.h"
#include "datacenter/CacheAgent.h"
#include "datacenter/ShareMemoryCenter.h"
#include "datacenter/MetaSerializer.h"
#include "datacenter/ResTableAgent.h"
#include "ZoneAgentMgr.h"


struct  AgentContext : public AppContext
{
public:    
    virtual void    OnGenerateDefaultConfig();
    virtual int     OnInit();
};

//----------------------------------------------------------------------------------

class AgentApp : public App
{        
public:
    //return 0 is ok , otherwise exit prcess
    virtual int     OnInit();    
    //control command process
    virtual string  OnCtrl(const std::vector<string> & cmdLine);    
    //tick 
    virtual int     OnTick(int64_t lElapseTime);
    //poll system
    virtual int     OnPoll(int iRecommendPollNum = 1);
    //system will close for closing Reason
    virtual int     OnClosing(int closingReason);
    //destroy sth uninitializing
    virtual int     OnDestroy();

public:
    //helper
    int     InitDB();
    int     InitCache();
    int     InitShmCenter();
    int     InitZoneMgr();
    int     InitScript();
    int     InitResTable();
    
private:
    DBAgent                     db;
    CacheAgent                  cache;
    MetaSerializer              meta;
    ShareMemoryCenter           shmCenter;
    ResTableAgent               resTable;
    ZoneAgentMgr                zoneMgr;
private:
    private:
    AgentApp(){}
    ~AgentApp(){}
    DeclareSingltonSupport(AgentApp)
};
