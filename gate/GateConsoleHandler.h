#pragma once
#include "net/UdpSocketHandler.h"

struct GateServerContext ;
class GateConsoleHandler: public UdpSocketHandler
{
public:    
    GateConsoleHandler(GateServerContext* pCtx);
public:    
    int OnDataRecv(UdpSocket& udpSock,const Buffer & recvBuffer,const SockAddress& addr);
    string OnCMD(const std::vector<string> & cmdLine);
    int    SplitCMDLine(string & line,vector<string> & list,const char* pszDeli = " ");
private:
    GateServerContext* ctx;
};



