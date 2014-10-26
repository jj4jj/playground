#pragma once
#include "base/Buffer.h"
#include "Session.h"
#include "proto/gen/cs/include.h"
#include "logic/CSMsgDispatcher.h"
#include "LoginLogic.h"

class ZoneAgent;
class SessionMgr
{
public:
public:
    SessionMgr(ZoneAgent* zone);
    int Init();
    int StopSession(uint64_t uid);
    int CreateSession(uint64_t uid,int gateid,const gate::GateSSMsg & cnnx);
    int DeleteSession(uint64_t uid);
    Session * FindSession(uint64_t    ulUID);  
    SessionPtr & GetSession(uint64_t);
    int       PackCSMsg(const cs::CSMsg & csmsg,Buffer & buffer);
    inline    ZoneAgent &   GetZoneAgent(){return *zoneAgent;}
    inline    LoginLogic &  GetLoginLogic(){return *login;}
    inline    CSMsgDispatcher & GetDispatcher(){return m_csmsgDispatcher;}
private:
    typedef unordered_map<uint64_t,SessionPtr>          UIDSessionMap;
    typedef UIDSessionMap::iterator                     UIDSessionMapItr;
    
    ZoneAgent *                             zoneAgent;
    LoginLogic       *                      login;
    CSMsgDispatcher                         m_csmsgDispatcher;
    UIDSessionMap                           m_mpSessions; 
    Buffer                                  m_msgPackBuffer;
};

