#pragma once
#include "base/stdinc.h"
#include "base/Singleton.hpp"

class ChannelMessageHandler;
class ChannelAgent;
class ChannelAgentMgr : public Singleton<ChannelAgentMgr>
{
public:
    typedef shared_ptr<ChannelAgent>     ChannelAgentPtr;
    typedef unordered_map<int,ChannelAgentPtr >     ChannelAgentMap;
    typedef ChannelAgentMap::iterator               ChannelAgentMapItr;
public:
    int             Init();
    int             AddChannel(int id,bool bRemote,const char * pszName,const char* pszAddr,ChannelMessageHandler* pHandler);
    ChannelAgent*   GetChannel(int id);
    int             RemoveChannel(int id);
    int             Polling();
private:
    void*           zmq_context;
    ChannelAgentMap m_mpChannelAgent;  
};



