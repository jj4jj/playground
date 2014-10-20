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
    int             Init(const char * pszName,const char* pszAddr,
                         ChannelMessageDispatcherPtr  _ptrDispatcher,
                         int iMaxLocalChannel = 1);
    void            Destroy();
    int             AddChannel(int id,bool local,const char * pszName,const char* pszAddr);
    ChannelAgent*   GetChannel(int id);
    int             Polling(int timeout = 10);
    ChannelMessageDispatcher *  GetDispatcher(){return ptrDispatcher.get();}
private:
    int             RemoveChannel(int id);
private:
    void*           zmq_context;
    ChannelAgentMap m_mpChannelAgent;  
    zmq_pollitem_t* zpollitems;
    int             nzpollitem;
    ChannelMessageDispatcherPtr  ptrDispatcher;
};


