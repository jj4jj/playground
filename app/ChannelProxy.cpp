#include "base/Log.h"
#include "channel/ChannelAgent.h"
#include "channel/ChannelAgentMgr.h"
#include "AppContext.h"
#include "ChannelProxy.h"


#if 1
ChannelProxy::ChannelProxy():m_pCtx(NULL),m_dispatcher(this)
{
}
ChannelProxy::~ChannelProxy()
{
    chnMsgSendBuffer.Destroy();
}
#endif


#if 1
int     ChannelProxy::Init(AppContext * pCtx)
{
    assert(NULL == m_pCtx);    
    m_pCtx = pCtx;
    int   iRet = chnMsgSendBuffer.Create(MAX_CHANNEL_MESSAGE_SIZE);
    if(iRet)
    {
        return -1;
    }
    if(pCtx->channels.empty())
    {
        LOG_INFO("no channel config !");
        return 0;
    }
    if(ChannelAgentMgr::Instance().Init(pCtx->channels.size(),&m_dispatcher))
    {
        LOG_ERROR("channel mgr init error !");
        return -1;
    }
    //build all dst chennel , map id . from config file
    for(uint i = 0;i < pCtx->channels.size(); ++i)
    {
        GateChannel & chnl = pCtx->channels[i];
        if(ChannelAgentMgr::Instance().AddChannel(chnl.id,!chnl.listener,
            pCtx->channelName.c_str(),chnl.channelAddr.c_str()) ) 
        {
            LOG_ERROR("chnnel create error id = %d",chnl.id);
            return -1;
        }
    }
    return 0;
}
int         ChannelProxy::SendToAgent(int iDst,const std::vector<Buffer>  &  vBuff)
{
    int iLength = 0;
    for(int i = 0;i < (int)vBuff.size(); ++i)
    {
        iLength += vBuff[i].iUsed;
    }   
    if(iLength > MAX_CHANNEL_MESSAGE_SIZE)
    {
        LOG_ERROR("Buffer size is too much = %d",iLength);
        return -1;
    }
    ChannelAgent  * pChannel = ChannelAgentMgr::Instance().GetChannel(iDst);
    if(pChannel)
    {
        LOG_ERROR("channel agent is not ready !");
        return -1;
    }
    //////////////////////////////////////////////
    //buffer 
    //len
    //msg
        //ga_connection
        //data    
    chnMsgSendBuffer.iUsed = 0;
    for(int i = 0;i < (int)vBuff.size(); ++i)
    {
        memcpy(chnMsgSendBuffer.pBuffer + chnMsgSendBuffer.iUsed,
                vBuff[i].pBuffer,vBuff[i].iUsed);
        chnMsgSendBuffer.iUsed += vBuff[i].iUsed;
    }    
    return pChannel->PostMessage(ChannelMessage(chnMsgSendBuffer));
}
int      ChannelProxy::SendToAgent(int iDst,const Buffer & buff)
{
    vector<Buffer>  v;
    v.push_back(buff);
    return SendToAgent(iDst,v);
}

int      ChannelProxy::SubscribeSingleMsg(int id,ChannelMessageDispatcherPtr hanlder)
{
    if(m_mpHandlerMap.find(id) == m_mpHandlerMap.end())
    {
        LOG_FATAL("subscribe single msg handler repeatly !");        
    }
    assert(m_mpHandlerMap.find(id) == m_mpHandlerMap.end());
    m_mpHandlerMap[id] = hanlder;
    return 0;
}
int      ChannelProxy::SubscribeContinuousMsg(int fromid , int endid,ChannelMessageDispatcherPtr hanlder)
{
    for(uint i = 0; i < m_vecHandlers.size(); ++i)
    {
        ChannelProxyMsgHandlerTableSegment & sec = m_vecHandlers[i].first;
        if(MatchSegment(sec,fromid) || MatchSegment(sec,endid))
        {

            LOG_FATAL("subscribe msg error for reaptly from = %d end = %d!",
                        fromid,endid);
            return -1;
        }        
    }
    std::pair<int,int> sec = std::make_pair(fromid,endid);
    m_vecHandlers.push_back(std::make_pair(sec,hanlder));           
    return 0;    
}
bool        ChannelProxy::MatchSegment(ChannelProxyMsgHandlerTableSegment & seg , int id)
{
    if( id >=  seg.first  && id <= seg.second )
    {
        return true;
    }
    return false;
}
ChannelMessageDispatcher *   ChannelProxy::FindHandler(int id)
{
    if(m_mpHandlerMap.find(id) != m_mpHandlerMap.end())
    {
        return m_mpHandlerMap[id].get();
    }
    for(uint i = 0; i < m_vecHandlers.size(); ++i)
    {
        if(MatchSegment(m_vecHandlers[i].first,id))
        {
            return m_vecHandlers[i].second.get();
        }
    }
    return NULL;
}

int      ChannelProxy::SubscribeScatterMsg(std::vector<int> & ids,ChannelMessageDispatcherPtr hanlder)
{
    for(uint i = 0;i < ids.size() ; ++i)
    {
        SubscribeSingleMsg(ids[i],hanlder);
    }
    return 0;
}

#endif

