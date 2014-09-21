#pragma once

class ChannelMessage;
class ChannelAgent;

class ChannelMessageDispatcher
{    
public:    
    virtual   int DispatchMessage(ChannelAgent & agent , const ChannelMessage & msg);
    virtual   ~ChannelMessageDispatcher(){}
};
typedef shared_ptr<ChannelMessageDispatcher>   ChannelMessageDispatcherPtr;












