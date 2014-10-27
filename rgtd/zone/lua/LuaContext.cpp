
#include "base/Log.h"
#include "LuaContext.h"
#include "cs_handler/CSMsgLuaHandler.h"
//////////////////////////////////////////////////////////////////////////////////////

//global namespace is rgtd


extern void CreateRgtdLuaLib(lua_State* pls);

//////////////////////////////////////////////////////////////////////////////////////
int     LuaContext::Init(CSMsgLuaHandler * host,const char* pszCtxName,const char* pszLuaPath,
                         const char* pszLuaCPath)
{
    int ret = m_luaAgent.Init();
    if(ret)
    {
        return -1;
    }
    m_sCtxName = pszCtxName;
    //export rgtd all function

    m_luaAgent.OpenStdLib();
    //path
    if(pszLuaPath)
    {
        m_luaAgent.PushString(pszLuaPath);
        m_luaAgent.SetGlobal("LUA_PATH");
    }

    if(pszLuaCPath)
    {
        m_luaAgent.PushString(pszLuaCPath);
        m_luaAgent.SetGlobal("LUA_CPATH");
    }
    
    //m_luaAgent.PushCClosure(BackTrace,0);
	//assert(m_luaAgent.GetTop() == 1);
   
    //export rgtd server core api to lua state
    ::CreateRgtdLuaLib(m_luaAgent.GetLuaState());
    
    return 0;
}

int     LuaContext::CallFunction(int & result ,const char* pszFunctionName,int nParam,...)
{
    //get function
    //lua pcall
    result = -1;
    va_list ap;    
    va_start(ap,nParam);

    m_luaAgent.GetGlobal(pszFunctionName);
    if(m_luaAgent.IsNil(-1))
    {
        LOG_ERROR("can not found function = %s",pszFunctionName);
        return -1;
    }
    for(int i = 0;i < nParam; ++i)
    {
        void* p = va_arg(ap,void*);
        m_luaAgent.PushLightUserData(p);
    }    
    va_end(ap);
    int ret =  m_luaAgent.PCall(nParam,0,0);    
    if(ret == 0)
    {
        result =  m_luaAgent.ToInteger(-1);
        m_luaAgent.Pop(1);
        return 0;
    }
    else
    {
		LOG_ERROR("lua call function = [%s] error : [%s]",pszFunctionName,m_luaAgent.ToLString(-1));
        m_luaAgent.Pop(1);
        return -1;
    }
}
int     LuaContext::GetIntValue(const char* pszKey,int defaultvalue )
{
    m_luaAgent.GetGlobal(pszKey);
    int type = m_luaAgent.GetType(-1);
    if(LUA_TNUMBER != type)
    {
        LOG_ERROR("lua key = %s type = %d not match !",pszKey,type);
        return defaultvalue;
    }
    int val = m_luaAgent.ToInteger(-1);
    m_luaAgent.Pop(1);
    return val;
}
string  LuaContext::GetStringValue(const char* pszKey)
{
    m_luaAgent.GetGlobal(pszKey);
    int type = m_luaAgent.GetType(-1);
    if(LUA_TSTRING != type)
    {
        LOG_ERROR("lua key = %s type = %d not match !",pszKey,type);
        return NULL;
    }
    string value = m_luaAgent.ToLString(-1);
    m_luaAgent.Pop(1);
    return value;
}
int     LuaContext::SetIntValue(const char* pszKey,int value)
{
    m_luaAgent.PushInteger(value);
    m_luaAgent.SetGlobal(pszKey);
    return 0;
}
int     LuaContext::SetStringValue(const char* pszKey,const char* value)
{
    m_luaAgent.PushString(value);
    m_luaAgent.SetGlobal(pszKey);
    return 0;
}
int     LuaContext::LoadFile(const char* pszFileName)
{
    LOG_INFO("lua context = %s load file = %s",m_sCtxName.c_str(),pszFileName);
    //load file
    if (m_luaAgent.LoadFile(pszFileName) || m_luaAgent.PCall(0, 0, 0) )
    {
        LOG_ERROR("cannot run lua file: %s",m_luaAgent.ToLString(-1));            
        return -1;
    }
    m_luaAgent.SetTop(0);
    return 0;
}

