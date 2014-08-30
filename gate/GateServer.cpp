#include "GateServerHandler.h"
#include "net/TcpServer.h"

int main()
{
    TcpServer   server;
    int iMaxClient = 50000;
    //port is 1234
    if(server.Init(SockAddress(1234),true,iMaxClient))
    {
        return -1;
    }
    TcpServerHandler* pHandler = new GateServerHandler(iMaxClient);
    server.SetServerHandler(pHandler);
    if(server.Start()!=0)
    {
        return -1;  
    }

    int iProc = 0;
    while(true)
    {
        iProc = server.Loop();
        if(iProc < 0)
        {
            return -1;
        }
        else if(0 == iProc)
        {
            //1ms
            usleep(1000);
        }
    }

    delete pHandler;
    return 0;
}