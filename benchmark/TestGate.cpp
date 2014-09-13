
//1.connect to gate and keep connection a live (send some garbage)
//2.send msg 128B , and return msg 1024B
    //2.1 gate server need summary the stream size and time 
    //2.1 gate add test mode , just return 1024B msg .
    //2.3 gate server add no-auth mode 

#include "base/Log.h"
#include "net/TcpClient.h"
#include "net/BatchTcpConnection.h"
#include "gate/client/GateClientHandler.h"

static int client_id = 0;
static Buffer send_buffer;

class TestGateClientHandler: public GateClientHandler
{
public:    
    virtual   int  OnConnect(bool bSuccess)
    {
        ++client_id;
        LOG_INFO("client connected state = %d id = %d",bSuccess,client_id);
        return 0;
    }
    virtual   int  OnMessage(char* pBuffer,int iBuffLen)
    {
        LOG_INFO("recv msg len = %d",iBuffLen);
        return 0;
    }
    virtual   int  OnClose(bool bByMyself)//server or me close 
    {
        LOG_INFO("socket has been close by = %d",bByMyself);
        return 0;
    }
};

int main(int argc,char* argv[])
{
    if(argc < 4)
    {
        printf("usage : ip port client_num\n");
        return -1;
    }
    int port = atoi(argv[1]);
    const char* pszIP = argv[2];
    int client_num = atoi(argv[3]);

    Log::Instance().Init("test_gate_client.log");    

    //use batch connection 
    BatchTcpConnection  btc;
    if(btc.Init(client_num))
    {
        return -1;
    }
    for(int i = 0;i < client_num; ++i)
    {
        if(btc.AddConnection(SockAddress(port,pszIP),TcpClientHandlerSharedPtr( new TestGateClientHandler())))
        {
            LOG_ERROR("connection error = %d",i);
        }
    }
    while(true)
    {
       int ret = btc.Loop();
       if(ret < 0)
       {
            return -1;
       }
       //no event occur
       else if( 1 == ret)
       {
            usleep(1000);
       }
    }     
    return 0;
}

















