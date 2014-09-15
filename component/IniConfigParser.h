#pragma once
#include "ConfigParser.h"

class IniConfigParser : public ConfigParser
{
public:    
    virtual int     ReadConfigFile(const char* pszFileName);
    virtual int     GetConfig(const char* pszKey,ConfigValue  & v);
    ///////////////////////////////////////////////////////
    virtual int     CreateConfig(const char* pszKey,const ConfigValue & v,const char* pszDesc = NULL);    
    ///////////////////////////////////////////////////////    
    virtual int     DumpConfig(const char* pszFileName);
};


