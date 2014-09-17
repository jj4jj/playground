#pragma once
#include "base/Buffer.h"

struct GateFrame
{
    char    *   pData;
    uint16_t    size;
public:    
    GateFrame(char* p,uint16_t sz);
    GateFrame(const Buffer & buf);
    typedef uint16_t    FrameLength;
    /////////////////////////////////////////////////////////////
    static const uint16_t MAX_GATE_FRAME_SIZE = (FrameLength)(-1);
};

