#pragma once
#include "component/IniConfigParser.h"
#include "base/stdinc.h"


struct ChannelConfig
{
    //id = 0 is local listen channel , otherwise connecting channel
    int     id;
    string  channelAddr;
};

class AppContext
{
public:
    IniConfigParser     parser;
    //////////////////////////////////////////////
    string                     channelName;
    string                     localChannelAddr;
    std::vector<ChannelConfig> channels;
    int       tickCountUs;
    int       tickPollCount;
    struct    timeval  curTime;   //this tick time        
    uint32_t  runtime;
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


