#pragma once
#include "component/IniConfigParser.h"
#include "base/stdinc.h"


struct GateChannel
{
    //id = 0 is local listen channel , otherwise connecting channel
    int     id;
    string  channelAddr;
    bool    listener;
};

class AppContext
{
public:
    IniConfigParser     parser;
    //////////////////////////////////////////////
    string                   channelName;
    std::vector<GateChannel> channels;
    int       tickCountUs;
    int       tickPollCount;
    struct timeval  curTime;   //this tick time        
    int       uniq_process;
    string    lockFilePath;
    int       hook_coredump;



    ///////////////////////////////////////
    int       closing;

public:
    //read config and init the common attr
    int     Init(const char * pszConfigFile);
    //common generate 
    void    GenerateDefaultConfig(const char* pszConfigFile);
public:
    virtual void    OnGenerateDefaultConfig();
    virtual int     OnInit();
};


