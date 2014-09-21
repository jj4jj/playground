#include "net/TcpServer.h"
#include "base/Log.h"
#include "component/IniConfigParser.h"
#include "base/CommonMacro.h"

////////////////////////////////
#include "GateServerContext.h"


#if 1
void    GateServerContext::OnGenerateDefaultConfig()
{
    static const char * kv[][2] = {
    //gate server
    {"gate:ip","127.0.0.1"},
    {"gate:port","58800"},
    {"gate:max_clients","5000"},

    /////////////add default config above////////////////
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
}
int     GateServerContext::OnInit()
{
    return 0;   
}
#endif



