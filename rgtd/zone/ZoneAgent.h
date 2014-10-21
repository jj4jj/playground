#pragma once
#include "base/stdinc.h"
#include "proto/gen/cs/include.h"
#include "account/SessionMgr.h"

class ZoneAgentMgr;
class ZoneAgent
{
public:
    ZoneAgent(uint32_t id , ZoneAgentMgr* mgr);
    virtual    ~ZoneAgent();
public:
    ////////////////////////////////////////////////////////////////////
    int         Init();
    void        Destroy();
    int         OnServerMessage();
    uint64_t    GetRoleID(uint64_t uid,uint32_t area);
    ////////////////////////////
    int     DispatchPlayerAgentMsg(const gate::GateConnection & ggc,const cs::CSMsg & csmsg);
    int     AttachPlayerAgent(int gateid,const gate::GateConnection & ggc);
    int     DetachPlayerAgent(const gate::GateConnection & ggc);
    inline  ZoneAgentMgr &  GetZoneMgr(){return *zoneMgr;}
private:
    uint32_t           m_dwZoneID;
    ZoneAgentMgr *     zoneMgr;
    SessionMgr         m_sessionMgr;
};
typedef shared_ptr<ZoneAgent>       ZoneAgentPtr;




