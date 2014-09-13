#include "GateServerHandler.h"
#include "net/TcpServer.h"
#include "GateChannelProxy.h"
#include "base/Log.h"
int main(int argc , char * argv[])
{
    if(argc < 3)
    {
        printf("usage : gate <ip> <port> <max_clients>\n");
        return -1;
    }
    const char* pszIP = argv[1];
    int port = atoi(argv[2]);
    int iMaxClient = atoi(argv[3]);
    printf("listening on %s:%d with max client num = %d\n",
            pszIP,port,iMaxClient);

    TcpServer   server;
    //port is 1234
    SockAddress addr(port,pszIP);
    LOG_INFO("server will listen on %s\n",addr.ToString());
    if(server.Init(addr,true,iMaxClient))
    {
        return -1;
    }
    GateChannelProxy proxy;
    if(proxy.Init())
    {
        return -1;
    }
    TcpServerHandler* pHandler = new GateServerHandler(&proxy,iMaxClient);

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
