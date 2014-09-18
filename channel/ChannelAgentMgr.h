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
    ChannelAgentMgr();
    ~ChannelAgentMgr();
public:
    int             Init(int iMaxChannel = 5);
    void            Destroy();
    int             AddChannel(int id,bool bRemote,const char * pszName,const char* pszAddr,ChannelMessageHandler* pHandler);
    ChannelAgent*   GetChannel(int id);
    int             Polling(int timeout = 10);
private:
    int             RemoveChannel(int id);
private:
    void*           zmq_context;
    ChannelAgentMap m_mpChannelAgent;  
    zmq_pollitem_t* zpollitems;
    int             nzpollitem;
};



