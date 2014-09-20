#pragma once
#include "component/IniConfigParser.h"
#include "base/stdinc.h"

class AppContext
{
public:
    shared_ptr<IniConfigParser>     parser;
    //////////////////////////////////////////////
    bool      closing;
    int       tickCountUs;
    int       tickPollCount;
    struct timeval  curTickStart;//tick start time
    struct timeval  curTime;   //this tick time        
public:
    //read config and init the common attr
    int     Init(const char * pszConfigFile);
    //common generate 
    void    GenerateDefaultConfig(const char* pszConfigFile);
public:
    virtual void    OnGenerateDefaultConfig();
    virtual int     OnInit();
};


