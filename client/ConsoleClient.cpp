#include "net/UdpSocket.h"
#include "net/UdpSocketHandler.h"
#include "net/UdpDriver.h"
#include "base/Log.h"

class GateConsoleClientHandler : public UdpSocketHandler
{
public:
    virtual int OnDataRecv(UdpSocket& udpSock,const Buffer & recvBuffer,const SockAddress& addr)
    {
        printf("%s\n",(char*)recvBuffer.pBuffer);
        return 0;
    }
};

int main(int argc , char* argv[])
{
    //ip , port , 'msg'
    if(argc < 3)
    {
        printf("usage:%s <port> <ip>\n",argv[0]);
        return -255;
    }
    SockAddress svrAddr(atoi(argv[1]),argv[2]);
    Log::Instance().Init(NULL, Log::LOG_LV_ERROR);
    ////////////////////////////////////////////////////////////////////////
    UdpDriver  consoleDrv;
    consoleDrv.Init(1);
    UdpSocketHandlerSharedPtr pHdlr(new GateConsoleClientHandler());
    consoleDrv.SetHandler(pHdlr);
    UdpSocket console;
    if(console.Init())
    {
        return -1;
    }
    consoleDrv.AddSocket(console.GetFD());    
    /////////////////////////////////////////////////////////////////
    Buffer buffer;
    buffer.Create(8192);
    buffer.pBuffer[0] = 0;
    char line[1024];
    while(true)
    {
        std::cout<<svrAddr.ToString()<<"$";
        std::cin.getline(line,sizeof(line));
        Buffer sndBuffer(line);
        if(console.SendTo(sndBuffer,svrAddr))
        {
            return -1;
        }
        if(consoleDrv.Loop(1,1000) < 0)
        {
            return -1;
        }
    };
    return 0;
}

