#pragma once

#include "base/stdinc.h"
#include "cs_handler/CSMsgLuaHandler.h"

class CSMsgHandler;
typedef shared_ptr<CSMsgHandler>    CSMsgHandlerPtr;

class CSMsgDispatcher
{
public:
    CSMsgDispatcher(SessionMgr* _mgr):sessionMgr(_mgr){}
public:    
    void    SetupAllCSMsgHandler();
    int     RegisterCSMsgHandler(uint32_t cmd,CSMsgHandlerPtr handler);
    int     Dispatch(Session & session,const cs::CSMsg & csmsg);
    CSMsgHandler*   GetMsgHandler(uint32_t cmd);
private:
    typedef unordered_map<uint32_t,CSMsgHandlerPtr>     CMDHandlerMap;
    typedef CMDHandlerMap::iterator                     CMDHandlerMapItr;
    CMDHandlerMap                                       m_mpHandlers;    
    SessionMgr *                                        sessionMgr;
    CSMsgLuaHandler                                     m_luaHandler;
};


