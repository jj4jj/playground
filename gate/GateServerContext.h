#pragma once
#include "base/stdinc.h"
class TcpServer ;
class IniConfigParser;
class GateChannelProxy;
struct GateServerContext
{
    TcpServer *          gateServer;
    shared_ptr<IniConfigParser>     parser;
    GateChannelProxy *   proxy;
    ///////////////////////
public:
    int    Init(const char * pszConfigFile);
    int    SetServer(TcpServer* pServer);           
};


