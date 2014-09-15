#pragma once
#include "ConfigParser.h"
#include "iniparser.h"

class IniConfigParser : public ConfigParser
{
public:    
    int     ReadConfigFile(const char* pszFileName);
    int     GetConfig(ConfigValue  & v);

    int     GetConfigInt(const char* pszKey,int defaultValue = 0);

    double  GetConfigReal(const char* pszKey,double defaultValue = 0.0);
    ///////////////////////////////////////////////////////
    int     CreateConfig(const ConfigValue & v,const char* pszDesc = NULL);
    ///////////////////////////////////////////////////////    
    int     Create();
    int     DumpConfig(const char* pszFileName);
private:
    dictionary * dic;
public:
    IniConfigParser();
    ~IniConfigParser();    
};


