#include "ZoneAgentMgr.h"

#if 1
int             ZoneAgentMgr::AddZone(int iZoneID,ZoneAgentPtr agent)
{
    return 0;
}
ZoneAgent &     ZoneAgentMgr::GetAgent(int iZoneID)
{
    return *m_mpZoneAgent[iZoneID].get();
}
int             ZoneAgentMgr::Init()
{
    //todo
    return 0;
}
int             ZoneAgentMgr::Polling()
{
    return 0;
}
int             ZoneAgentMgr::Destroy()
{
    return 0;
}
//
int             ZoneAgentMgr::OnGateMessage()
{
    return 0;
}
int             ZoneAgentMgr::OnServerMessage()
{
    return 0;
}
#endif

