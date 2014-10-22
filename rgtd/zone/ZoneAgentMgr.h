#pragma once

#include "ZoneAgent.h"
#include "app/ChannelMsgProxy.h"
#include "DBProxy.h"
#include "CacheProxy.h"

class DBAgent;
class CacheAgent;
class PlayerAgent;
typedef shared_ptr<PlayerAgent>     PlayerAgentPtr;

class ZoneAgentMgr
{
public:
    ZoneAgentMgr();
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
    inline          DBProxy &          GetDBProxy(){return dbProxy;}
    inline          CacheProxy &       GetCacheProxy(){return cacheProxy;}
    PlayerAgentPtr     GetZonePlayerAgentPtr(uint64_t uid);
    static uint64_t    GetRoleID(uint64_t uid,uint32_t area);
    static uint32_t    GetUidFromRoleID(uint64_t rid);
    static uint16_t    GetAreaFromRoleID(uint64_t rid); 
    uint16_t           GetZoneIDFromArea(uint32_t area);
    ZoneAgent *        GetZoneAgentByArea(uint32_t area);
private:
    unordered_map<uint32_t,ZoneAgentPtr>    m_mpZoneAgent;
    ChannelMsgProxy *                       m_chnlProxy;
    DBAgent      *                          m_db;
    CacheAgent   *                          m_cache;
    LoginLogic                              login;
    DBProxy                                 dbProxy;
    CacheProxy                              cacheProxy;
};


