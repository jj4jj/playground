#include "base/Log.h"
#include "channel/ChannelAgent.h"
#include "channel/ChannelAgentMgr.h"
#include "GateServerContext.h"
#include "GateChannelProxy.h"

class  GateMessageHandler : public  ChannelMessageHandler
{
public:    
    virtual   int OnRecvMessage(const ChannelMessage & msg)
    {
        //todo
        return -1;
    }
};



#if 1
GateChannelProxy::GateChannelProxy():m_pServerCtx(NULL)
{
}
GateChannelProxy::~GateChannelProxy()
{
    chnMsgSendBuffer.Destroy();
}

#endif


#if 1

int     GateChannelProxy::Init(GateServerContext * pCtx)
{
    assert(NULL == m_pServerCtx);
    m_pServerCtx = pCtx;
    int   iRet = chnMsgSendBuffer.Create(MAX_CHANNEL_MESSAGE_SIZE);
    if(iRet)
    {
        return -1;
    }
    //build all dst chennel , map id . from config file
    for(uint i = 0;i < pCtx->channels.size(); ++i)
    {
        GateChannel & chnl = pCtx->channels[i];
        ChannelMessageHandlerPtr ptr(new GateMessageHandler());
        if(ChannelAgentMgr::Instance().AddChannel(chnl.id,!chnl.listener,
            "gate",chnl.channelAddr.c_str(),ptr) ) 
        {
            LOG_ERROR("chnnel create error id = %d",chnl.id);
            return -1;
        }
    }

    return 0;
}
int         GateChannelProxy::SendToAgent(int iDst,const std::vector<Buffer>  &  vBuff)
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
int      GateChannelProxy::SendToAgent(int iDst,const Buffer & buff)
{
    vector<Buffer>  v;
    v.push_back(buff);
    return SendToAgent(iDst,v);
}

#endif

