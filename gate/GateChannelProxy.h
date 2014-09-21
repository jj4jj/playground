#pragma once
#include "base/Buffer.h"
//just wrap channel 
//send dst id to a channel 
//init all channels

struct GateServerContext;
class GateChannelProxy
{
public:
    int      Init(GateServerContext * pCtx);
    int      SendToAgent(int iDst,const std::vector<Buffer>  &  vBuff);
    int      SendToAgent(int iDst,const Buffer &  buff);
public:
    GateChannelProxy();
    virtual ~GateChannelProxy();

private:
    GateServerContext * m_pServerCtx;
    Buffer chnMsgSendBuffer;
    enum
    {
        MAX_CHANNEL_MESSAGE_SIZE = 65536,
    };
};

