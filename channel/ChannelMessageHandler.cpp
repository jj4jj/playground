
#include "base/Log.h"
#include "ChannelAgent.h"
#include "ChannelMessage.h"


#include "ChannelMessageHandler.h"
#if 1
ChannelMessageHandler::~ChannelMessageHandler()
{
    m_pAgent = NULL;    
}
#endif

#if 1 
int ChannelMessageHandler::OnRecvMessage(const ChannelMessage & msg )
{
    LOG_INFO("recv a channel msg size = %u!",msg.dwSize);
    return -1;
}
#endif




