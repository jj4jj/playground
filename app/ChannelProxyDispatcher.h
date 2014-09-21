#pragma once
#include "base/stdinc.h"
#include "base/Log.h"
#include "channel/ChannelMessageDispatcher.h"

class ChannelProxy ;
class  ChannelProxyMessageDispatcher : public  ChannelMessageDispatcher
{
public:
    ChannelProxyMessageDispatcher(ChannelProxy * p):proxy(p){}
public:    
    virtual   int DispatchMessage(ChannelAgent & agent , const ChannelMessage & msg);
private:
    ChannelProxy    * proxy;
};

