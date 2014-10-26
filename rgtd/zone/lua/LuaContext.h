#pragma once
#include "script/LuaAgent.h"

class LuaContext
{
public:
    int     Init();
    int     LoadFile(const char* pszFileName);
    int     ExcuteFile(const char* pszFileName);
    
protected:
    int     Define();

    
private:    
    LuaAgent    m_luaAgent;
};










