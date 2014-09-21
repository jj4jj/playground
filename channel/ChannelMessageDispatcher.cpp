
#include "base/Log.h"
#include "ChannelAgent.h"
#include "ChannelMessage.h"

#include "ChannelMessageDispatcher.h"


#if 1 
int ChannelMessageDispatcher::DispatchMessage(ChannelAgent & agent , const ChannelMessage & msg)
{
    LOG_INFO("recv a channel id = %d  msg size = %u!",agent.GetID(),msg.dwSize);
    return -1;
}
#endif




