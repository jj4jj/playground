#pragma once

#include "ZoneAgent.h"

class ZoneAgentMgr
{
public:
    int             AddZone(int iZoneID,ZoneAgentPtr agent);
    ZoneAgent &     GetAgent(int iZoneID);
    int             Init();
    int             Polling();
    int             Destroy();
    //
    int             OnGateMessage();
    int             OnServerMessage();
private:
    unordered_map<uint32_t,ZoneAgentPtr>    m_mpZoneAgent;
};


