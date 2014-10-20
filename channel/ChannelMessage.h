#pragma once

#include "base/Buffer.h"

struct  ChannelMessage
{
    int         iSrc;
    uint32_t    dwSize;
    uint8_t *   pData;
public:
    ChannelMessage():iSrc(-1),dwSize(0),pData(NULL){}
    ChannelMessage(const Buffer & buff):
            dwSize(buff.iUsed),
            pData(buff.pBuffer)
    {           
    }
};

