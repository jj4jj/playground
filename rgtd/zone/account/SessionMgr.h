#pragma once
#include "Session.h"
#include "proto/gen/cs/include.h"
#include "LoginLogic.h"

class CSMsgHandler;
class ZoneAgent;

class SessionMgr
{
public:
    SessionMgr(ZoneAgent* zone);
    int Init();
    int CreateSession(int gateid,const gate::GateConnection & cnnx);
    int DeleteSession();
    Session * FindSession(uint64_t    ulUID);    
    CSMsgHandler*   GetMsgHandler(uint32_t cmd);
    int       Dispatch(Session & session,const cs::CSMsg & csmsg);
private:
    ZoneAgent*                              zoneAgent;
    unordered_map<uint64_t,SessionPtr>      m_mpSessions; 
    LoginLogic       *                      login;
};

