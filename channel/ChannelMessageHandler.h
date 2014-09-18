#pragma once

class ChannelMessage;
class ChannelAgent;

class ChannelMessageHandler
{    
public:    
    virtual   int OnRecvMessage(const ChannelMessage & );
    virtual   ~ChannelMessageHandler();
public:
    void    SetAgent(ChannelAgent* p){m_pAgent = p;}
public:
    ChannelAgent*   m_pAgent;        
};

