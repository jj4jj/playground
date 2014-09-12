
#include "base/stdinc.h"
#include "base/Log.h"
#include "ChannelAgent.h"
#include "ChannelAgentMgr.h"
#include "zmq.h"

int             ChannelAgentMgr::Init()
{
    assert(NULL == zmq_context);
    zmq_context = zmq_ctx_new();
    if(!zmq_context)
    {
        LOG_FATAL("zmq init error !");
        return -1;
    }
            
    return 0;
}
int             ChannelAgentMgr::AddChannel(int id,bool bRemote,const char * pszName,const char* pszAddr,ChannelMessageHandler* pHandler)
{
    if(m_mpChannelAgent.find(id) != m_mpChannelAgent.end())
    {
        //already exist
        LOG_ERROR("register channel repeatly id = %d",id);
        return -1;
    }
    ChannelAgentPtr p(new ChannelAgent());
    int chnMode = bRemote?Channel::CHANNEL_MODE_REMOTE:Channel::CHANNEL_MODE_LOCAL;
    int iRet = p->Init(zmq_context,chnMode,pszName,pszAddr,pHandler);
    if(iRet)
    {
        LOG_ERROR("create channell error = %d",iRet);
        return -2;
    }
    m_mpChannelAgent[id] = p;    
    return 0;
}
ChannelAgent* ChannelAgentMgr::GetChannel(int id)
{
    if(m_mpChannelAgent.find(id)!= m_mpChannelAgent.end())
    {
        return m_mpChannelAgent[id].get();
    }
    //////////////
    return NULL;
}
int             ChannelAgentMgr::RemoveChannel(int id)
{
    ChannelAgent* p = GetChannel(id);
    if(!p)
    {
        return -1;
    }
    m_mpChannelAgent.erase(id);
    return 0;
}
int             ChannelAgentMgr::Polling()
{
    //
    ChannelAgentMapItr it = m_mpChannelAgent.begin();
    int iRet = 0;
    ChannelMessage msg;
    //todo use zmq_poll
    for(;it != m_mpChannelAgent.end();++it)
    {   
        iRet = it->second->GetMessage(msg);
        if(iRet)
        {
            continue;
        }
        it->second->DispatchMessage(msg);            
    }
    return iRet;
}





