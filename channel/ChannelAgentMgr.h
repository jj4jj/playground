#pragma once
#include "base/stdinc.h"
#include "base/Singleton.hpp"
#include "ChannelMessageDispatcher.h"
#include "ChannelAgent.h"


class ChannelAgentMgr : public Singleton<ChannelAgentMgr>
{
public:
    typedef shared_ptr<ChannelAgent>     ChannelAgentPtr;
    typedef unordered_map<int,ChannelAgentPtr >     ChannelAgentMap;
    typedef ChannelAgentMap::iterator               ChannelAgentMapItr;
private :
    DeclareSingltonSupport(ChannelAgentMgr)
    ChannelAgentMgr();
    ~ChannelAgentMgr();
public:
    int             Init(int iMaxChannel ,ChannelMessageDispatcher*  pDispatcher);
    void            Destroy();
    int             AddChannel(int id,bool bRemote,const char * pszName,const char* pszAddr);
    ChannelAgent*   GetChannel(int id);
    int             Polling(int timeout = 10);
    ChannelMessageDispatcher *  GetDispatcher(){return pDispatcher;}
private:
    int             RemoveChannel(int id);
private:
    void*           zmq_context;
    ChannelAgentMap m_mpChannelAgent;  
    zmq_pollitem_t* zpollitems;
    int             nzpollitem;
    ChannelMessageDispatcher * pDispatcher;
};


