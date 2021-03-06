#pragma once

#include "base/stdinc.h"

class CSMsgHandler;
typedef shared_ptr<CSMsgHandler>    CSMsgHandlerPtr;

class CSMsgLuaHandler;
typedef shared_ptr<CSMsgLuaHandler> CSMsgLuaHandlerPtr;

class CSMsgDispatcher
{
public:
    CSMsgDispatcher(SessionMgr* _mgr):sessionMgr(_mgr){}
public:    
    int     SetupAllCSMsgHandler();
    int     RegisterCSMsgHandler(uint32_t cmd,CSMsgHandlerPtr handler);
    int     Dispatch(Session & session,const cs::CSMsg & csmsg);
    CSMsgHandler*   GetMsgHandler(uint32_t cmd);
private:
    typedef unordered_map<uint32_t,CSMsgHandlerPtr>     CMDHandlerMap;
    typedef CMDHandlerMap::iterator                     CMDHandlerMapItr;
    CMDHandlerMap                                       m_mpHandlers;    
    SessionMgr *                                        sessionMgr;
    CSMsgLuaHandlerPtr                                  m_ptrLuaHandler;
};


