#pragma once
#include "base/stdinc.h"
#include "app/AppContext.h"
#include "net/TcpServer.h"



typedef struct unordered_map<uint32_t,uint32_t>     MapAgentAreaIDGateChannelID;


class   GateServerHandler;
typedef shared_ptr<GateServerHandler>   GateServerHandlerPtr;

struct GateServerContext : AppContext
{
    TcpServer               gateServer;
    GateServerHandlerPtr    ptrHandler;
    ////////////////////////////////////////
    int             iTestMode;
    int             iNeedAuth;    
    MapAgentAreaIDGateChannelID    mpAreaID2ChannelID;
    vector<int>                    vecAgentIDS;
    int             iIdleCheckPeriod;
public:
    virtual void    OnGenerateDefaultConfig();
    virtual int     OnInit();
};


