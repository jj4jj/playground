#pragma once
#include "base/Buffer.h"
#include "Session.h"
#include "proto/gen/cs/include.h"
#include "LoginLogic.h"

class CSMsgHandler;
class ZoneAgent;
class SessionMgr
{
public:
    typedef shared_ptr<CSMsgHandler>        CSMsgHandlerPtr;
public:
    SessionMgr(ZoneAgent* zone);
    int Init();
    int RegisterCSMsgHandler(uint32_t cmd,CSMsgHandlerPtr handler);
    int StopSession(uint64_t uid);
    int CreateSession(uint64_t uid,int gateid,const gate::GateConnection & cnnx);
    int DeleteSession(uint64_t uid);
    Session * FindSession(uint64_t    ulUID);  
    SessionPtr & GetSession(uint64_t);
    CSMsgHandler*   GetMsgHandler(uint32_t cmd);
    int       Dispatch(Session & session,const cs::CSMsg & csmsg);
    int       PackCSMsg(const cs::CSMsg & csmsg,Buffer & buffer);
    inline    ZoneAgent &   GetZoneAgent(){return *zoneAgent;}
    inline    LoginLogic &  GetLoginLogic(){return *login;}
private:
    typedef unordered_map<uint64_t,SessionPtr>          UIDSessionMap;
    typedef unordered_map<uint32_t,CSMsgHandlerPtr>     CMDHandlerMap;
    typedef UIDSessionMap::iterator                     UIDSessionMapItr;
    typedef CMDHandlerMap::iterator                     CMDHandlerMapItr;
    ZoneAgent *                             zoneAgent;
    LoginLogic       *                      login;
    UIDSessionMap                           m_mpSessions; 
    CMDHandlerMap                           m_mpHandlers;
    Buffer                                  m_msgPackBuffer;
};

