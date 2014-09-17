#include "GateConsoleHandler.h"
#include "GateServerContext.h"
#include "base/Log.h"

#if 1
GateConsoleHandler::GateConsoleHandler(GateServerContext* p)
{
    ctx = p;
}
int GateConsoleHandler::OnDataRecv(UdpSocket& udpSock,const Buffer & recvBuffer,const SockAddress& _addr)
{    
    SockAddress addr(_addr);
    LOG_INFO("recv console msg = %s len = %d from %s",(char*)recvBuffer.pBuffer,recvBuffer.iUsed,(addr).ToString());    
    udpSock.SendTo(Buffer("an rsp!"),addr);
    return 0;
}
#endif


