#include "base/stdinc.h"
#include "base/Log.h"
#include "ChannelProxy.h"
#include "channel/ChannelAgent.h"
#include "ChannelProxyDispatcher.h"

#if 1
int ChannelProxyMessageDispatcher::DispatchMessage(ChannelAgent & agent , const ChannelMessage & msg)
{
    //find id msg handler dispatch it.
    ChannelMessageDispatcher* handler = proxy->FindHandler(agent.GetID());
    if(!handler)
    {
        LOG_ERROR("channel id = %d not found handler",agent.GetID());
        return -1;
    }       
    handler->DispatchMessage(agent,msg);
    return 0;
}
#endif

