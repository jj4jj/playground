#pragma once

class TcpServer ;
class IniConfigParser;
class GateChannelProxy;
struct GateServerContext
{
    TcpServer *          gateServer;
    IniConfigParser  *   parser;
    GateChannelProxy *   proxy;
    ///////////////////////
public:
    int    Init(const char * pszConfigFile);
    int    SetServer(TcpServer* pServer);           
};


