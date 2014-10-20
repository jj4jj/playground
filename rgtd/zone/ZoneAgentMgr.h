#pragma once

#include "ZoneAgent.h"
#include "app/ChannelMsgProxy.h"

class DBAgent;
class CacheAgent;

class ZoneAgentMgr
{
public:
    int             AddZone(uint32_t dwZoneID,ZoneAgentPtr agent);
    ZoneAgent *     GetAgent(uint32_t dwZoneID);
    int             Init();
    int             Polling(int iPollTimeOutMs = 5);
    int             Destroy();
    //
    int             OnGateMessage(const ChannelMessage & msg);
    int             OnServerMessage(const ChannelMessage & msg);
    inline          LoginLogic&     GetLoginLogic(){return login;}
    inline          DBAgent   &     GetDBAgent(){return *m_db;}
    inline          CacheAgent   &     GetCacheAgent(){return *m_cache;}    
private:
    unordered_map<uint32_t,ZoneAgentPtr>    m_mpZoneAgent;
    ChannelMsgProxy *                       m_chnlProxy;
    DBAgent      *                          m_db;
    CacheAgent   *                          m_cache;
    LoginLogic                              login;
};


