#include "net/TcpServer.h"
#include "GateChannelProxy.h"
#include "base/Log.h"
#include "component/IniConfigParser.h"
////////////////////////////////
#include "GateServerContext.h"


#if 1
int    GateServerContext::Init(const char * pszConfigFile)
{
    parser = shared_ptr<IniConfigParser>(new IniConfigParser());
    parser->SetRootName("gate");    
    if(!pszConfigFile)
    {
        pszConfigFile = "gate.cfg.example";
    }
    if(!File::Exist(pszConfigFile))
    {
        printf("config file  %s is not exist , so create it use default option .\n",pszConfigFile);        
        parser->Create("gate");
        static const char * kv[][2] = {
        //gate server
        {"ip","127.0.0.1"},
        {"port","58800"},
        {"max_clients","5000"},
        {"logfile","gate.log"},
        {"daemon","0"},
        //console
        {"console:ip","127.0.0.1"},
        {"console:port","58810"},

        /////////////add default config above////////////////
        {NULL,NULL}};
        int i = 0;
        ConfigValue v;
        while(kv[i][0])
        {
            v.key  = kv[i][0];
            v.value  = kv[i][1];
            parser->CreateConfig(v);
            ++i;
        }
        parser->DumpConfig(pszConfigFile);
        printf("create default config ok !\n");
        return -1;
    }
    else if(parser->ReadConfigFile(pszConfigFile))
    {        
        return -2;
    }
    return 0;
}
int    GateServerContext::SetServer(TcpServer* pServer)
{
    gateServer = pServer;
    return 0;
}
#endif



