
#include "GateServerHandler.h"
#include "GateFrame.h"
#include "GateAgentChannelMsgHandler.h"



#if 1

GateAgentChannelMsgHandler::GateAgentChannelMsgHandler(GateServerHandler * hdr):gateHandler(hdr)
{
    
}
int GateAgentChannelMsgHandler::DispatchMessage(ChannelAgent & agent , const ChannelMessage & msg)
{
    //gate server
    //todo distill
    gate::GateConnection    ggc;
    uint16_t ggclen =  ntohs(*(uint16_t*)msg.pData);
    if(msg.dwSize < (uint32_t)ggclen +  ChannelMsgProxy::PROXY_CHANNEL_MESSAGE_HEAD_SIZE)
    {
        LOG_FATAL("ggc len = %d all size = %d src = %d error !",ggclen,msg.dwSize,msg.iSrc);
        return -1;
    }
    if(!ggc.ParseFromArray(msg.pData + ChannelMsgProxy::PROXY_CHANNEL_MESSAGE_HEAD_SIZE,ggclen))
    {
        LOG_ERROR("unpack gate msg error !");
        return -1;
    }
    
    int event = ggc.event();
    int csmsglen = msg.dwSize - ggclen - ChannelMsgProxy::PROXY_CHANNEL_MESSAGE_HEAD_SIZE;
    char * pcsmsg = (char*)msg.pData +  ChannelMsgProxy::PROXY_CHANNEL_MESSAGE_HEAD_SIZE + ggclen;
    LOG_INFO("agent server event = [%d]  uid [%lu] src = [%d] idx = [%u] csmsg = [%u]",
        event,ggc.uid(),msg.iSrc,ggc.idx(),csmsglen);            
    GateServerHandler::Connection* pConn = gateHandler->GetConnectionByIdx(ggc.idx());
    if(!pConn)
    {
        LOG_ERROR("agent close client idx = %u is missing !",ggc.idx());
        return -1;
    }
    switch(event)
    {
        case gate::GateConnection::EVENT_CLOSE:
        {
            gateHandler->RemoveConnection(pConn,ggc.reason());
        }
        break;
        case gate::GateConnection::EVENT_DATA:
        {
            //get uid area
            LOG_INFO("agent input data size = %u uid [%lu] src = [%d] idx = [%u]",
                csmsglen,ggc.uid(),msg.iSrc,ggc.idx());
            //data msg unpack
            //forward msg
            gateHandler->downpkgs++;
            gateHandler->rsppkgs++;
            gateHandler->SendFrameToClient(pConn,GateFrame(pcsmsg,csmsglen));
        }
        ////////////////////////////////////////////////////////////////////////
        break;       
        case gate::GateConnection::EVENT_CONNECTED:
        default:
            LOG_ERROR("gate connection unknown event = %d",event);
            return -1;
    }  
    return 0;
}

#endif


