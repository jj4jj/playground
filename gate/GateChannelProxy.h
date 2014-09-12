#pragma once
#include "base/Buffer.h"

//just wrap channel 
//send dst id to a channel 
//init all channels


class GateChannelProxy
{
public:
    int      Init();
    int      SendToAgent(int iDst,const std::vector<Buffer>  &  vBuff);
    int      SendToAgent(int iDst,const Buffer &  buff);
};

