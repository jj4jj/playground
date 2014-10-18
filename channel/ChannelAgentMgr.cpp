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
int             ChannelAgentMgr::Init(int iMaxChannel,ChannelMessageDispatcher*  _pDispatcher)
{
    assert(NULL == zmq_context && iMaxChannel > 0 && zpollitems == NULL);
    zmq_context = zmq_ctx_new();
    if(!zmq_context)
    {
        LOG_FATAL("zmq init error !");
        return -1;
    }            
    nzpollitem = iMaxChannel;
    zpollitems = (zmq_pollitem_t*)malloc(iMaxChannel*sizeof(zmq_pollitem_t));    
    assert(zpollitems != NULL);
    pDispatcher = _pDispatcher;
    bzero(zpollitems,iMaxChannel*sizeof(zmq_pollitem_t));
    return 0;
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

int             ChannelAgentMgr::AddChannel(int id,bool bRemote,const char * pszName,const char* pszAddr)
{
    if(m_mpChannelAgent.find(id) != m_mpChannelAgent.end())
    {
        //already exist
        LOG_ERROR("register channel repeatly id = %d",id);
        return -1;
    }
    LOG_INFO("add channel id = %d remote = %d name = %s addr = %s ",
        id,bRemote,pszName,pszAddr);
    ChannelAgentPtr p(new ChannelAgent(id));
    char channelName[32];
    assert(strlen(pszName) < 20);
    SNPRINTF(channelName,sizeof(channelName),"%s#%d",pszName,id);
    int chnMode = bRemote?Channel::CHANNEL_MODE_REMOTE:Channel::CHANNEL_MODE_LOCAL;
    int iRet = p->Init(zmq_context,chnMode,channelName,pszAddr);
    if(iRet)
    {
        LOG_ERROR("create channell error = %d",iRet);
        return -2;
    }
    Channel & chnl = p->GetChannel();    
    zmq_pollitem_t & item = zpollitems[nzpollitem++];
    item.socket = chnl.receiver;
    item.fd = id;
    item.events = ZMQ_POLLIN;
    item.revents = 0;
    m_mpChannelAgent[id] = p;    
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
            pDispatcher->DispatchMessage(*(it->second.get()),msg);            
            ////////////////////////////////////////////////////////
            ++ievnts;
        }
    }
    return ievnts;
}

#endif



