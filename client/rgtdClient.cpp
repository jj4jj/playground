
//1.connect to gate and keep connection a live (send some garbage)
//2.send msg 128B , and return msg 1024B
    //2.1 gate server need summary the stream size and time 
    //2.1 gate add test mode , just return 1024B msg .
    //2.3 gate server add no-auth mode 

#include "base/Log.h"
#include "net/TcpClient.h"
#include "net/BatchTcpConnection.h"
#include "gate/GateClientHandler.h"
#include "proto/gen/cs/include.h"
#include "thread/Thread.h"

static int client_id = 0;
static Buffer send_buffer;
class rgtdClientHandler;
static rgtdClientHandler * sender = NULL;


class rgtdClientHandler: public GateClientHandler
{
public:    
    virtual   int  OnConnect(bool bSuccess)
    {
        ++client_id;
        LOG_INFO("client connected state = %d id = %d",bSuccess,client_id);
        sender = this;
        return 0;
    }
    virtual   int  OnNeedAuth()
    {
        return Authorize(0,client_id,"test");
    }
    virtual   int  OnAuthResult(int result)
    {
        LOG_INFO("server auth result = %d",result);
        return 0;
    }
    virtual   int  OnMessage(char* pBuffer,int iBuffLen)
    {
        LOG_INFO("recv msg len = %d",iBuffLen);
        cs::CSMsg csmsg;
        csmsg.ParseFromArray(pBuffer,iBuffLen);
        string s = csmsg.DebugString();
        printf("recv msg:\n%s\n",s.c_str());
        return 0;
    }
    virtual   int  OnClose(bool bByMyself)//server or me close 
    {
        LOG_INFO("socket has been close by = %d",bByMyself);
        return 0;
    }
};
static char cmdLine[2][1024];
static int w_idx = 0;
static int r_ready = 0;
static void * ReadStdinThread(void*)
{
    while(1)
    {
        printf("rgtdClient>>");
        fflush(stdout);
        fgets(cmdLine[w_idx],sizeof(cmdLine[w_idx]),stdin);
        cmdLine[w_idx][strlen(cmdLine[w_idx])-1] = 0;
        w_idx = (w_idx+1)%2;
        r_ready  = 1;
    }
    return 0;
}
///////////////////////////////////////
void Auth()
{
    
}
void Login()
{
    cs::CSMsg msg;
    //    msg.set_cmd(cs::CSMSG_);
}
void Reg();
void Req();
void Help();
static struct
{
    const char* pszCMD;
    void (*fn)();
}   cmds [] = 
{
    {"auth",Auth},
    {"login",Login},
    {"reg",Reg},
    {"req",Req},
    {"help",Help},
    {NULL,NULL}
};
void Help()
{
    for(int i = 0; cmds[i].pszCMD ; ++i)
    {
        puts(cmds[i].pszCMD);
    }
}
void Reg()
{

}
void Req()
{

}


static void ProcessCMD(const char * pszCMD)
{
    if(strlen(pszCMD) == 0)
    {
        return;
    }
    LOG_INFO("process cmd = [%s] ...",pszCMD);
    for(int i = 0; cmds[i].pszCMD ; ++i)
    {
        if(strcmp(pszCMD,cmds[i].pszCMD) == 0)
        {
            return cmds[i].fn();
        }
    }
}
int main(int argc,char* argv[])
{    
    int port = 52100;
    const char* pszIP = "127.0.0.1";
    int client_num = 1;

    if(argc < 4)
    {
        printf("usage : port ip client_num [use default ip = 127.0.0.1 port = %d num = 1]\n",port);
    }
    else
    {
        port = atoi(argv[1]);
        pszIP = argv[2];
        client_num = atoi(argv[3]);
        printf("connect to %s:%d with %d clients\n",
            pszIP,port,client_num);
    }

    const char* pszLogFileName = NULL;
    if(argc > 4)
    {
        pszLogFileName = argv[4];
    }
    if(pszLogFileName)  
    {
        Log::Instance().Init( pszLogFileName, Log::LOG_LV_DEBUG, 1024000);
    }
    send_buffer.Create(1024*1024*4);
    //use batch connection 
    BatchTcpConnection  btc;
    if(btc.Init(client_num))
    {
        return -1;
    }
    int iErrorNum = 0;
    SockAddress addr(port,pszIP);    
    LOG_INFO("connect to server  %s",addr.ToString());
    for(int i = 0;i < client_num; ++i)
    {
        if(btc.AddConnection(addr,TcpClientHandlerSharedPtr( new rgtdClientHandler())))
        {
            LOG_ERROR("connection error = %d",i);
            ++iErrorNum;
        }
    }
    if( iErrorNum >= client_num)
    {
        LOG_ERROR("connection is error !");
        return -1;
    }

    Thread  trd;
    trd.Create(NULL,ReadStdinThread,NULL);
    while(true)
    {
       int ret = btc.Loop();
       if(ret < 0)
       {
            return -1;
       }
       //no event occur
       else if( 0 == ret)
       {
            usleep(1000);
       }
       if(r_ready)
       {
            ProcessCMD(cmdLine[(w_idx+1)%2]);
            r_ready = 0;
       }
    }     
    send_buffer.Destroy();
    
    return 0;
}

















