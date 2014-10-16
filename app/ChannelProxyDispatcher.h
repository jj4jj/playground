#pragma once
#include "base/stdinc.h"
#include "base/Log.h"
#include "channel/ChannelMessageDispatcher.h"

class ChannelMsgProxy ;
class  ChannelProxyMessageDispatcher : public  ChannelMessageDispatcher
{
public:
    ChannelProxyMessageDispatcher(ChannelMsgProxy * p):proxy(p){}
public:    
    virtual   int DispatchMessage(ChannelAgent & agent , const ChannelMessage & msg);
private:
    ChannelMsgProxy    * proxy;
};

