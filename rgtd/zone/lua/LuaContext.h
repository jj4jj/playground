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
    int     GetIntValue(const char* pszKey,int defaultvalue = 0);
    string  GetStringValue(const char* pszKey);    

private:    
    string      m_sCtxName;
    LuaAgent    m_luaAgent;
};










