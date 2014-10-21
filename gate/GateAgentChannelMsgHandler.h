#pragma once
#include "app/App.hpp"
#include "channel/ChannelMessageDispatcher.h"

class GateServer;
class ChannelAgent;

class   GateAgentChannelMsgHandler : public ChannelMessageDispatcher
{
public:
    GateAgentChannelMsgHandler(GateServerHandler * hdr);
    virtual   int DispatchMessage(ChannelAgent & agent , const ChannelMessage & msg);
private:
    GateServer *    gateServer;
    GateServerHandler * gateHandler;    
};



