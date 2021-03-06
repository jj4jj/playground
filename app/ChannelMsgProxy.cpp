#include "base/Log.h"
#include "channel/ChannelAgent.h"
#include "channel/ChannelAgentMgr.h"
#include "AppContext.h"
#include "ChannelMsgProxy.h"


#if 1
ChannelMsgProxy::ChannelMsgProxy():m_pCtx(NULL)
{
}
ChannelMsgProxy::~ChannelMsgProxy()
{
    chnMsgSendBuffer.Destroy();
}
#endif


#if 1
int     ChannelMsgProxy::Polling(int iPollTimeOutMs )
{
    return  ChannelAgentMgr::Instance().Polling(iPollTimeOutMs);
}
void    ChannelMsgProxy::Destroy()
{
    chnMsgSendBuffer.Destroy();
    ChannelAgentMgr::Instance().Destroy();
}

int     ChannelMsgProxy::Init(AppContext * pCtx)
{
    assert(NULL == m_pCtx);    
    m_pCtx = pCtx;
    int   iRet = chnMsgSendBuffer.Create(MAX_CHANNEL_MESSAGE_SIZE);
    if(iRet)
    {
        return -1;
    }

    if(pCtx->localChannelAddr.length() == 0)
    {
        LOG_INFO("no listen channel config !");
        return 0;
    }
    //////////////////////////////////////////////////////
    ChannelMessageDispatcherPtr dispatcher(new ChannelProxyMessageDispatcher(this));
    //const char * pszName,const char* pszAddr
    if(ChannelAgentMgr::Instance().Init(pCtx->channelName.c_str(),
                                pCtx->localChannelAddr.c_str(),
                                dispatcher) )
    {
        LOG_ERROR("channel mgr init error !");
        return -1;
    }
    ChannelAgentMgr::Instance().GetChannel(0)->SetIDName(pCtx->localChannelName.c_str());

    /////////////////////////////////////////////////////////////////////////////////////////

    //build all dst chennel , map id . from config file
    LOG_INFO("add channel count = %d",pCtx->channels.size());
    for(uint i = 0;i < pCtx->channels.size(); ++i)
    {
        ChannelConfig & chnl = pCtx->channels[i];
        //add name map id
        int chid = ChannelAgentMgr::Instance().GenerateChannelID(chnl.name);
        LOG_INFO("add channel name = %s addr = %s id = %d",chnl.name.c_str(),chnl.channelAddr.c_str(),chid);
        if(ChannelAgentMgr::Instance().AddChannel(chid,false,
            pCtx->channelName.c_str(),chnl.channelAddr.c_str()) ) 
        {
            LOG_ERROR("chnnel create error id = %d",chid);
            return -1;
        }
        ChannelAgentMgr::Instance().GetChannel(chid)->SetIDName(chnl.name.c_str());
    }
    return 0;
}
int         ChannelMsgProxy::SendToAgent(int iDst,const std::vector<Buffer>  &  vBuff,int iHeadSize)
{
    int iLength = 0;
    for(int i = 0;i < (int)vBuff.size(); ++i)
    {
        iLength += vBuff[i].iUsed;
    }   
    if(iLength + PROXY_CHANNEL_MESSAGE_HEAD_SIZE >= MAX_CHANNEL_MESSAGE_SIZE)
    {
        LOG_ERROR("Buffer size is too much = %d",iLength);
        return -1;
    }
    ChannelAgent  * pChannel = ChannelAgentMgr::Instance().GetChannel(iDst);
    if(!pChannel)
    {
        LOG_ERROR("channel agent id = %d is not ready !",iDst);
        return -1;
    }
    //////////////////////////////////////////////
    //buffer 
    //len
    //msg
        //ga_connection
        //data    
    chnMsgSendBuffer.iUsed = PROXY_CHANNEL_MESSAGE_HEAD_SIZE;
    *(uint16_t*)chnMsgSendBuffer.pBuffer = htons(iHeadSize);
    for(uint i = 0;i < vBuff.size(); ++i)
    {
        memcpy(chnMsgSendBuffer.pBuffer + chnMsgSendBuffer.iUsed,
                vBuff[i].pBuffer,vBuff[i].iUsed);
        chnMsgSendBuffer.iUsed += vBuff[i].iUsed;
    }    
    return pChannel->PostMessage(ChannelMessage(chnMsgSendBuffer));
}
int      ChannelMsgProxy::SendToAgent(int iDst,const Buffer & buff,int iHeadSize)
{
    vector<Buffer>  v;
    v.push_back(buff);
    return SendToAgent(iDst,v,iHeadSize);
}

int      ChannelMsgProxy::SubscribeSingleMsg(int id,ChannelMessageDispatcherPtr hanlder)
{
    if(m_mpHandlerMap.find(id) != m_mpHandlerMap.end())
    {
        LOG_FATAL("subscribe single msg handler repeatly !");
    }
    assert(m_mpHandlerMap.find(id) == m_mpHandlerMap.end());
    m_mpHandlerMap[id] = hanlder;
    LOG_INFO("subscribe single msg id = %d",id);
    return 0;
}
int      ChannelMsgProxy::SubscribeContinuousMsg(int fromid , int endid,ChannelMessageDispatcherPtr hanlder)
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
    LOG_INFO("subscribe single msg id  from = %d to = %d",fromid,endid);
    return 0;    
}
bool        ChannelMsgProxy::MatchSegment(ChannelProxyMsgHandlerTableSegment & seg , int id)
{
    if( id >=  seg.first  && id <= seg.second )
    {
        return true;
    }
    return false;
}
ChannelMessageDispatcher *   ChannelMsgProxy::FindHandler(int id)
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

int      ChannelMsgProxy::SubscribeScatterMsg(std::vector<int> & ids,ChannelMessageDispatcherPtr hanlder)
{
    for(uint i = 0;i < ids.size() ; ++i)
    {
        SubscribeSingleMsg(ids[i],hanlder);
    }
    return 0;
}

#endif

