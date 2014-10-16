#pragma once
#include "base/stdinc.h"

class ZoneAgent
{
public:
    int Init();
    int Polling();
    int Destroy();
    int OnGateMessage();
    int OnServerMessage();
};
typedef shared_ptr<ZoneAgent>       ZoneAgentPtr;




