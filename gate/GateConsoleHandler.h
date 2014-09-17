#pragma once
#include "net/UdpSocketHandler.h"

struct GateServerContext ;
class GateConsoleHandler: public UdpSocketHandler
{
public:    
    GateConsoleHandler(GateServerContext* pCtx);
public:    
    int OnDataRecv(UdpSocket& udpSock,const Buffer & recvBuffer,const SockAddress& addr);

private:
    GateServerContext* ctx;
};



