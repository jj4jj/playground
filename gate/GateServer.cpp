#include "GateServerHandler.h"
#include "net/TcpServer.h"
#include "GateChannelProxy.h"
#include "base/Log.h"
#include "component/IniConfigParser.h"
#include "utility/Daemon.h"

int main(int argc , char * argv[])
{
    IniConfigParser parser;
    parser.SetRootName("gate");
    if(argc < 2)
    {
        printf("usage : %s <config file> \n",argv[0]);        
        parser.Create("gate");
        static const char * kv[][2] = {{"ip","127.0.0.1"},
                                {"port","58800"},
                                {"max_clients","5000"},
                                {"logfile","gate.log"},
                                {"daemon","0"},
                                {NULL,NULL}};
        int i = 0;
        ConfigValue v;
        while(kv[i][0])
        {
            v.key  = kv[i][0];
            v.value  = kv[i][1];
            parser.CreateConfig(v);
            ++i;
        }
        parser.DumpConfig("gate.cfg.example");
        return 0;
    }
    if(parser.ReadConfigFile(argv[1]))
    {        
        return -1;
    }
    string sIP = parser.GetConfigString("ip");
    int port = parser.GetConfigInt("port");    
    int iMaxClient = parser.GetConfigInt("max_clients");
    string sLogFileName = parser.GetConfigString("logfile");
    int daemon = parser.GetConfigInt("daemon");    
    const char* pszLogFileName = sLogFileName.c_str();
    const char* pszIP = sIP.c_str();
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
            break;
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
