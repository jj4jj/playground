#pragma once
#include "script/LuaAgent.h"

class CSMsgLuaHandler;
class LuaContext
{
public:
    int     Init(CSMsgLuaHandler * host,const char* pszCtxName,const char* pszLuaPath = NULL,
                         const char* pszLuaCPath = NULL);
    int     LoadFile(const char* pszFileName);
    int     CallFunction(int & result ,const char* pszFunctionName,int nParam,...);
protected:
    
private:    
    string      m_sCtxName;
    LuaAgent    m_luaAgent;
};










