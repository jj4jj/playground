#include "base/stdinc.h"
#include "base/Log.h"
#include "ChannelMsgProxy.h"
#include "channel/ChannelAgent.h"
#include "ChannelProxyDispatcher.h"
#include "channel/ChannelAgentMgr.h"

#if 1
int ChannelProxyMessageDispatcher::DispatchMessage(ChannelAgent & agent , const ChannelMessage & msg)
{
    LOG_DEBUG("recv a msg from id = %d msg len = %u",msg.iSrc,msg.dwSize);
    //find id msg handler dispatch it.
    ChannelMessageDispatcher* handler = proxy->FindHandler(msg.iSrc);
    if(!handler)
    {
        LOG_ERROR("channel id = %d not found handler",msg.iSrc);
        return -1;
    }       
    ChannelAgent * pAgent = ChannelAgentMgr::Instance().GetChannel(msg.iSrc);
    if(!pAgent)
    {
        LOG_ERROR("get channel agent error id = %d",msg.iSrc);
        return -1;
    }    
    handler->DispatchMessage(*pAgent,msg);
    return 0;
}
#endif

