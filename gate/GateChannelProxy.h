<<<<<<< HEAD
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




=======
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




>>>>>>> 260f3888a8736281c9b1eb253c283df674e163a7
