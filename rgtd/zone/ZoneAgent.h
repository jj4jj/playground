#pragma once
#include "base/stdinc.h"
#include "proto/gen/cs/include.h"


namespace gate
{
    class GateConnection;
};


class ZoneAgentMgr;


class ZoneAgent
{
public:
    ZoneAgent(uint32_t id,ZoneAgentMgr* mgr);
    virtual    ~ZoneAgent();
public:
    ////////////////////////////////////////////////////////////////////
    int Init();
    void Destroy();
    int OnServerMessage();
    ////////////////////////////
    int DispatchPlayerAgentMsg(const cs::CSMsg & csmsg);
    int AttachPlayerAgent(const gate::GateConnection & ggc);
    int DetachPlayerAgent(const gate::GateConnection & ggc);
private:
    uint32_t         m_dwZoneID;
    ZoneAgentMgr     * m_mgr;
};
typedef shared_ptr<ZoneAgent>       ZoneAgentPtr;




