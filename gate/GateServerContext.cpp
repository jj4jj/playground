#include "net/TcpServer.h"
#include "GateChannelProxy.h"
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
    {"gate:channel:num","1"},
    {"gate:channel#1:id","1"},//connect other channel
    {"gate:channel#1:addr","tcp://127.0.0.1:58880"},
    {"gate:channel#1:listener","0"},

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
    int ichnlNum = parser.GetConfigInt("gate:channel:num",-1);
    GateChannel      chnl;
    char keyBuffer[64];
    channels.clear();
    for(int i = 0;i < ichnlNum ; ++i)
    {
        SNPRINTF(keyBuffer,sizeof(keyBuffer),"gate:channel#%d:id",i+1);
        chnl.id = parser.GetConfigInt(keyBuffer,-1);
        if(chnl.id < 0)
        {
            LOG_ERROR("get config key = %s error !",keyBuffer);
            return -1;
        }


        SNPRINTF(keyBuffer,sizeof(keyBuffer),"gate:channel#%d:listener",i+1);        
        int listen = parser.GetConfigInt(keyBuffer,-1);
        if(listen < 0)
        {
            LOG_ERROR("get config key = %s error !",keyBuffer);
            return -1;
        }
        chnl.listener = (listen > 0 );
        SNPRINTF(keyBuffer,sizeof(keyBuffer),"gate:channel#%d:addr",i+1);
        chnl.channelAddr = parser.GetConfigString(keyBuffer);
        if(strlen(chnl.channelAddr.c_str()) < 1)
        {
            LOG_ERROR("get config key = %s error !",keyBuffer);
            return -1;
        }

        channels.push_back(chnl);
    }
    return 0;   
}
#endif



