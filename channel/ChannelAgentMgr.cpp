#include "base/stdinc.h"
#include "base/Log.h"
#include "ChannelMessage.h"
#include "Channel.h"
#include "ChannelMessageDispatcher.h"
#include "ChannelAgent.h"
#include "ChannelAgentMgr.h"


#if 1
ChannelAgentMgr::ChannelAgentMgr()
{
    zmq_context = NULL;
    nzpollitem = 0;
    zpollitems = 0;    
}
ChannelAgentMgr::~ChannelAgentMgr()
{
    Destroy();
}
#endif

#if 1
int             ChannelAgentMgr::Init(const char * pszName,const char* pszListenAddr,
                                      ChannelMessageDispatcherPtr  _ptrDispatcher,
                                      int iMaxLocalChannel )
{
    assert(NULL == zmq_context && iMaxLocalChannel > 0 && zpollitems == NULL);
    zmq_context = zmq_ctx_new();
    if(!zmq_context)
    {
        LOG_FATAL("zmq init error !");
        return -1;
    }            
    zpollitems = (zmq_pollitem_t*)malloc(iMaxLocalChannel*sizeof(zmq_pollitem_t));    
    assert(zpollitems != NULL);
    ptrDispatcher = _ptrDispatcher;
    bzero(zpollitems,iMaxLocalChannel*sizeof(zmq_pollitem_t));
    nzpollitem = 0;
    ///////////////////////////////////////////////////////
    return AddChannel(0,true,pszName,pszListenAddr);
}
void            ChannelAgentMgr::Destroy()
{
    if(zmq_context)
    {
        zmq_ctx_destroy(zmq_context);
        zmq_context = NULL;
    }        
    SAFE_FREE(zpollitems);
    nzpollitem = 0;
}

int             ChannelAgentMgr::AddChannel(int id,bool local,const char * pszName,const char* pszAddr)
{
    if(!local)
    {
        assert(id > 0);
    }
    if(m_mpChannelAgent.find(id) != m_mpChannelAgent.end())
    {
        //already exist
        LOG_ERROR("register channel repeatly id = %d",id);
        return -1;
    }
    LOG_INFO("add channel id = %d local = %d name = %s addr = %s ",
        id,local?1:0,pszName,pszAddr);    
    ChannelAgentPtr p(new ChannelAgent(id));
    char channelName[32];
    assert(strlen(pszName) < 20);
    SNPRINTF(channelName,sizeof(channelName),"%s#%d",pszName,id);
    int chnMode = local?Channel::CHANNEL_MODE_LOCAL:Channel::CHANNEL_MODE_REMOTE;
    int iRet = p->Init(zmq_context,chnMode,channelName,pszAddr);
    if(iRet)
    {
        LOG_ERROR("create channell error = %d",iRet);
        return -2;
    }

    if(local)
    {
        Channel & chnl = p->GetChannel();    
        zmq_pollitem_t & item = zpollitems[nzpollitem++];
        item.socket = chnl.receiver;
        item.fd = id;
        item.events = ZMQ_POLLIN;
        item.revents = 0;
    }
    ///////////////////////////////////////////////////
    m_mpChannelAgent[id] = p;  
    LOG_DEBUG("agent id = %d agent ptr = %p ptr agent id = %d zitem = %d",
        id,p.get(),p->GetID(),nzpollitem);
    return 0;
}
ChannelAgent* ChannelAgentMgr::GetChannel(int id)
{
    if(m_mpChannelAgent.find(id) != m_mpChannelAgent.end())
    {
        return m_mpChannelAgent[id].get();
    }
    //////////////
    return NULL;
}
int             ChannelAgentMgr::RemoveChannel(int id)
{
    return -1; //do not supported
    ////////////////////////////////
    ChannelAgent* p = GetChannel(id);
    if(!p)
    {
        return -1;
    }
    LOG_INFO("remove channel agent = %d",id);
    m_mpChannelAgent.erase(id);

    return 0;
}
int             ChannelAgentMgr::Polling(int timeout)
{

    int n = zmq_poll(zpollitems, nzpollitem,timeout);
    if( n < 0 )
    {
        if(zmq_errno() != EINTR)
        {
            LOG_ERROR("zmq poll error errno = %d for str = %s!",zmq_errno(),
                    zmq_strerror(zmq_errno()) );
            return -1;
        }
        return 0;
    }
    int ievnts = 0;
    ChannelMessage msg;
    int iRet = 0;
    for(int i = 0 ;i < nzpollitem; ++i)
    {
        if(zpollitems[i].revents & ZMQ_POLLIN)
        {
            ChannelAgentMapItr it = m_mpChannelAgent.find(zpollitems[i].fd);
            if(  it == m_mpChannelAgent.end()  )
            {
                continue;
            }
            iRet = it->second->GetMessage(msg);
            if(iRet)
            {
                continue;
            }
            ///////////////////////////////////////////////////////
            ptrDispatcher->DispatchMessage(*(it->second.get()),msg);            
            ////////////////////////////////////////////////////////
            ++ievnts;
        }
    }
    return ievnts;
}

#endif



