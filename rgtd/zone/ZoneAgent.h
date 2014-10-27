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
    ////////////////////////////
    int     DispatchPlayerAgentMsg(const gate::GateSSMsg & ggc,const cs::CSMsg & csmsg);
    int     AttachPlayerAgent(int gateid,const gate::GateSSMsg & ggc);
    int     DetachPlayerAgent(int gateid,const gate::GateSSMsg & ggc);
    inline  ZoneAgentMgr &  GetZoneMgr(){return *zoneMgr;}
    inline  SessionMgr   &  GetSessionMgr(){return m_sessionMgr;}
private:
    uint32_t           m_dwZoneID;
    ZoneAgentMgr *     zoneMgr;
    SessionMgr         m_sessionMgr;
};
typedef shared_ptr<ZoneAgent>       ZoneAgentPtr;




