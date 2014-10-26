#pragma once

#include "CSMsgHandler.h"
#include "lua/LuaContext.h"


class CSMsgLuaHandler : public CSMsgHandler
{
public:
    int     Init(const char* pszFileName);
    int     Load(const char* pszFileName);        
    bool    IsListeningMsg(uint32_t cmd);
    int     RegisterCMD(uint32_t cmd);
public:
    virtual int   OnCreatingMsg(Session & session,const cs::CSMsg & msg);
    virtual int   OnPlayingMsg(Session & session,const cs::CSMsg & msg);
public:
    int     Handle(Session & session,const cs::CSMsg & msg);
private:
    LuaContext                 m_luaCtx;
    unordered_set<uint32_t>     m_setluahandlers;
};

