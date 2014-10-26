
#include "base/Log.h"

#include "CSMsgLuaHandler.h"


/////////////////////////////////////////////////////////////////////////////////
int     CSMsgLuaHandler::Init(const char* pszFileName)
{    
    if(m_luaCtx.Init())
    {
        LOG_ERROR("lua agent init error !");
        return -1;
    }
    /////////////////////////////////////////
    if(m_luaCtx.ExcuteFile(pszFileName))
    {
        LOG_ERROR("lua agent ctx loading file error !");
        return -1;
    }
    ////////////////////////////////////////
    

    return 0;
}
int     CSMsgLuaHandler::RegisterCMD(uint32_t cmd)
{
    if(m_setluahandlers.find(cmd) != m_setluahandlers.end())
    {
        LOG_ERROR("register lua handler cmd = %u repeat ! !",cmd);
        return -1;
    }
    LOG_INFO("register lua cmd handler = %u",cmd);    
    m_setluahandlers.insert(cmd);
    return 0;
}

bool     CSMsgLuaHandler::IsListeningMsg(uint32_t cmd)
{
    //
    return (m_setluahandlers.find(cmd) != m_setluahandlers.end());
}
int     CSMsgLuaHandler::OnCreatingMsg(Session & session,const cs::CSMsg & msg)
{
    //find lua function
    return -1;
}
int     CSMsgLuaHandler::OnPlayingMsg(Session & session,const cs::CSMsg & msg)
{
    //find lua function
    return -1;
}

