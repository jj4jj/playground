
#include "base/stdinc.h"
#include "base/Log.h"
#include "ChannelAgent.h"
#include "ChannelAgentMgr.h"

int             ChannelAgentMgr::Init(const char* pszLocalAddr)
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
int             ChannelAgentMgr::AddChannel(int id,const char* pszLocalAddr,const char* pszRemoteAddr,ChannelMessageHandler* pHandler)
{
    if(m_mpChannelAgent.find(id) != m_mpChannelAgent.end())
    {
        //already exist
        LOG_ERROR("register channel repeatly id = %d",id):
        return -1;
    }
    ChannelAgentPtr p(new ChannelAgent());
    if(p->Init(pHandler))
    {
        LOG_ERROR("create channell error = %d",iRet);
        return -2;
    }
    int iRet = p->CreateChannel(id,zmq_context,pszLocalAddr,pszRemoteAddr);
    if(iRet)
    {
        LOG_ERROR("create channell error = %d",iRet);
        return -3;
    }
    m_mpChannelAgent[id] = p;    
    return 0;
}
ChannelAgentPtr ChannelAgentMgr::GetChannel(int id)
{
    if(m_mpChannelAgent.find(id)!= m_mpChannelAgent.end())
    {
        return m_mpChannelAgent[id];
    }
    //////////////
    return nullptr;
}
int             ChannelAgentMgr::RemoveChannel(int id)
{
    ChannelAgentPtr p = GetChannel(id);
    if(!p)
    {
        return -1;
    }
    p->DestroyChannel();
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





