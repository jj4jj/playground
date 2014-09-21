#pragma once
#include "base/stdinc.h"
#include "app/AppContext.h"
#include "GateChannelProxy.h"
#include "net/TcpServer.h"

struct GateChannel
{
    //id = 0 is local listen channel , otherwise connecting channel
    int     id;
    string  channelAddr;
    bool    listener;
};

struct GateServerContext : AppContext
{
    TcpServer            gateServer;
    GateChannelProxy     proxy;
    ////////////////////////////////////////
    std::vector<GateChannel> channels;
public:
    virtual void    OnGenerateDefaultConfig();
    virtual int     OnInit();
};


