#pragma once
#include "base/stdinc.h"
#include "app/AppContext.h"
#include "net/TcpServer.h"


typedef struct unordered_map<uint32_t,uint32_t>     MapAgentAreaIDGateChannelID;


struct GateServerContext : AppContext
{
    TcpServer            gateServer;
    TcpServerHandlerPtr  ptrHandler;
    ////////////////////////////////////////
    int             iTestMode;
    int             iNeedAuth;    
    MapAgentAreaIDGateChannelID    mpAreaID2ChannelID;
    vector<int>                    vecAgentIDS;
public:
    virtual void    OnGenerateDefaultConfig();
    virtual int     OnInit();
};


