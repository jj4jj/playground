
#include "base/Log.h"

#include "CSMsgLuaHandler.h"


/////////////////////////////////////////////////////////////////////////////////
int     CSMsgLuaHandler::Init(const char* pszFileName)
{    
    if(m_luaCtx.Init(this,"rgtd"))
    {
        LOG_ERROR("lua agent init error !");
        return -1;
    }
    ////////////////////////////////////////
    /////////////////////////////////////////
    if(m_luaCtx.LoadFile(pszFileName))
    {
        LOG_ERROR("lua agent ctx loading file error !");
        return -1;
    }
    int result = 0;
    int ret = m_luaCtx.CallFunction(result,"cs_handler_init",1,this);
    if(ret)
    {
        LOG_ERROR("init lua cshandler error !");
        return -1;
    }
    if(result != 0)
    {
        LOG_ERROR("cs_handler_init calling error = %d!",result);
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
    int result = 0;
    int ret = m_luaCtx.CallFunction(result,"OnCreatingMsg",2,&session,&msg);
    if(ret)
    {
        LOG_ERROR("call creating msg error = %d!",ret);
        return -1;
    }
    return result;
}
int     CSMsgLuaHandler::OnPlayingMsg(Session & session,const cs::CSMsg & msg)
{
    int result = 0;
    int ret = m_luaCtx.CallFunction(result,"OnPlayingMsg",2,&session,&msg);
    if(ret)
    {
        LOG_ERROR("call creating msg error = %d!",ret);
        return -1;
    }
    return result;
}

