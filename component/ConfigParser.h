#pragma once

#include "base/stdinc.h"
#include "base/Log.h"
//maybe use xml or lua script 
//pb or lua state parse it

struct ConfigValue
{
    string key;
    string value;
    std::vector<ConfigValue>    dic;
    ConfigValue()
    {
        key = value = "";
        dic.clear();
    }
    ~ConfigValue()
    {
        //LOG_DEBUG("destruct key = %s [%p]",key.c_str(),key.c_str());
        if(!dic.empty())
        {
            //LOG_DEBUG("destruct dic size = %zu",dic.size());
        }
        else
        {
            //LOG_DEBUG("destruct value = %s [%p]",value.c_str(),value.c_str());
        }
    }
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
    ConfigParser();
    virtual ~ConfigParser();
public:
    virtual int     ReadConfigFile(const char* pszFileName) = 0;
    virtual int     GetConfig(ConfigValue  & v) = 0;
    ///////////////////////////////////////////////////////
    virtual int     CreateConfig(const ConfigValue & v,const char* pszDesc = NULL) = 0;    
    ///////////////////////////////////////////////////////    
    virtual int     Create(const char* pszRootName = NULL) = 0;//create an empty config
    virtual int     DumpConfig(const char* pszFileName) = 0;    
public:
    void     SetRootName(const char* pszRootName = NULL);
    void     VisualConfig(string & s);
    void     VisualConfig(string & s,ConfigValue & v,int level,const char* prefix);
protected:
    ConfigValue rootValue;
};




