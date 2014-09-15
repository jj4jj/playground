#pragma once

#include "base/stdinc.h"

//maybe use xml or lua script 
//pb or lua state parse it

struct ConfigValue
{
    enum
    {
        CONFIG_VALUE_TYPE_INVALID = 0,
        CONFIG_VALUE_TYPE_INTEGER = 1,
        CONFIG_VALUE_TYPE_REAL =    2,
        CONFIG_VALUE_TYPE_VECTOR =  3,
        CONFIG_VALUE_TYPE_MAP =     4,        
    };
    int type;
    string key;
    //union u
    //{
        string s;
        int    i;
        double r;
        std::vector<ConfigValue>    vec;        
        std::vector<ConfigValue>    dic;
    //} //value;        
};

//EBNF
//    key:id
//        id.key
//        id#idx.key
//-----------------------
//      key:id
//          ckey.key
//      cky:id
//          ckey#idx

class ConfigParser
{
public:
    ConfigParser()
    {
        rootValue.key = "root";
    }
    virtual ~ConfigParser()
    {

    }
public:
    virtual int     ReadConfigFile(const char* pszFileName) = 0;
    virtual int     GetConfig(const char* pszKey,ConfigValue  & v) = 0;
    ///////////////////////////////////////////////////////
    virtual int     CreateConfig(const char* pszKey,const ConfigValue & v,const char* pszDesc = NULL) = 0;    
    ///////////////////////////////////////////////////////    
    virtual int     DumpConfig(const char* pszFileName) = 0;    
private:
    ConfigValue rootValue;
};




