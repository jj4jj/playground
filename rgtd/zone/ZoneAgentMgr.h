#pragma once

#include "ZoneAgent.h"
#include "app/ChannelMsgProxy.h"

class DBAgent;
class CacheAgent;

class ZoneAgentMgr
{
public:
    int             AddZone(int iZoneID,ZoneAgentPtr agent);
    ZoneAgent &     GetAgent(int iZoneID);
    int             Init();
    int             Polling(int iPollTimeOutMs = 5);
    int             Destroy();
    //
    int             OnGateMessage(const ChannelMessage & msg);
    int             OnServerMessage(const ChannelMessage & msg);
private:
    unordered_map<uint32_t,ZoneAgentPtr>    m_mpZoneAgent;
    ChannelMsgProxy *                       m_chnlProxy;
    DBAgent      *                          m_db;
    CacheAgent   *                          m_cache;

};


