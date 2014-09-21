#pragma once
#include "base/stdinc.h"
#include "app/AppContext.h"
#include "net/TcpServer.h"


struct GateServerContext : AppContext
{
    TcpServer            gateServer;
    ////////////////////////////////////////
public:
    virtual void    OnGenerateDefaultConfig();
    virtual int     OnInit();
};


