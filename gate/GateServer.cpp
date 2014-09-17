#include "GateServerHandler.h"
#include "net/TcpServer.h"
#include "GateChannelProxy.h"
#include "base/Log.h"
#include "component/IniConfigParser.h"
#include "utility/Daemon.h"
#include "GateConsoleHandler.h"
#include "net/UdpDriver.h"
#include "GateServerContext.h"




int main(int argc , char * argv[])
{
    IniConfigParser parser;
    GateServerContext ctx;
    if(argc < 2)
    {
        ctx.Init(NULL);
        return -1;
    }
    if(ctx.Init(argv[1]))
    {
        printf("gate server context init error !");
        return -1;
    }    
    
    string sIP = parser.GetConfigString("ip");
    string sLogFileName = parser.GetConfigString("logfile");
    string sConsoleIP = parser.GetConfigString("console:ip");
    
    int port = parser.GetConfigInt("port");    
    int iMaxClient = parser.GetConfigInt("max_clients");
    int daemon = parser.GetConfigInt("daemon");    
    int console_port = parser.GetConfigInt("console:port");

    const char* pszLogFileName = sLogFileName.c_str();
    const char* pszIP = sIP.c_str();
    const char* pszConsoleIP = sConsoleIP.c_str();

    
    string configString;
    parser.VisualConfig(configString);
    LOG_INFO(configString.c_str());


    if(pszLogFileName)  
    {
        Log::Instance().Init( pszLogFileName, Log::LOG_LV_DEBUG, 1024000);
    }
    if(daemon)
    {
        //daemonlize
        Daemon::Instance().Create();
    }    
    

    ////////////////////////////////////////////////////////////////////////
    UdpDriver  consoleDrv;
    consoleDrv.Init(10);
    UdpSocketHandlerSharedPtr pHdlr(new GateConsoleHandler(&ctx));
    consoleDrv.SetHandler(pHdlr);
    UdpSocket console;
    if(console.Init())
    {
        return -1;
    }
    SockAddress consoleAddr(console_port,pszConsoleIP);
    LOG_INFO("gate console will listen on %s\n",consoleAddr.ToString());
    if(console.Bind(consoleAddr))
    {
        return -1;
    }
    consoleDrv.AddSocket(console.GetFD());    
    /////////////////////////////////////////////////////////////////

    
    TcpServer   server;
    //port is 1234
    SockAddress addr(port,pszIP);
    LOG_INFO("gate server will listen on %s\n",addr.ToString());
    if(server.Init(addr,true,iMaxClient))
    {
        return -1;
    }
    GateChannelProxy proxy;
    if(proxy.Init())
    {
        return -1;
    }
    TcpServerHandlerPtr  ptrHandler = TcpServerHandlerPtr(new GateServerHandler(&proxy,iMaxClient));

    server.SetServerHandler(ptrHandler.get());
    if(server.Start()!=0)
    {
        return -1;  
    }

    ///////////////////////////////////////////////////
    int iProcMsg = 0;
    while(true)
    {
        //check console
        consoleDrv.Loop(1,0);

        //check server
        iProcMsg = server.Loop();
        if(iProcMsg < 0)
        {
            break;
        }
        else if(0 == iProcMsg)
        {
            //1ms
            usleep(1000);
        }
    }

    return 0;
}
