
//1.connect to gate and keep connection a live (send some garbage)
//2.send msg 128B , and return msg 1024B
    //2.1 gate server need summary the stream size and time 
    //2.1 gate add test mode , just return 1024B msg .
    //2.3 gate server add no-auth mode 


#include "base/Log.h"
#include "net/TcpClient.h"
#include "net/BatchTcpConnection.h"
#include "PingPangHandler.h"

static int client_id = 0;
static Buffer send_buffer;

class TestGateClientHandler: public TcpClientHandler
{
public:    
    int     OnConnected( TcpSocket &   client);
    //readable
    int     OnDataRecv( TcpSocket &   client,const Buffer & recvBuffer);
    //peer close .
    int     OnDisconnected( TcpSocket &   client);
    ~PingPangClientHandler(){}
};
int     TestGateClientHandler::OnConnected( TcpSocket &   client)
{
    ++client_id;    
    client.Send(send_buffer);
    return 0;
}
//readable
int     TestGateClientHandler::OnDataRecv( TcpSocket &   client,const Buffer & recvBuffer)
{
    //recognize a package
    return 0;
}
//peer close .
int     TestGateClientHandler::OnDisconnected( TcpSocket &   client)
{
    LOG_INFO("client fd = %d is closed",client.GetFD());
    return 0;





int main()
{

    Log::Instance().Init("test_gate_client.log");

    //use batch connection 
    BatchTcpConnection  btc;
    if(btc.Init(1))
    {
        return -1;
    }
    if(btc.AddConnection(SockAddress(1234,"127.0.0.1"),TcpClientHandlerSharedPtr( new TestGateClientHandler())))
    {
        return -1;
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

















