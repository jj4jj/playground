#include "net/TcpServer.h"
#include "base/Log.h"
#include "component/IniConfigParser.h"
#include "base/CommonMacro.h"
#include "base/StringUtil.h"

////////////////////////////////
#include "GateServerContext.h"


/*
default addr arrangement:

agent
    51XXX
    channel listen
        51010
        51011
        51012
gate
    52XXX  
    tcp server listen
        52010
        52011
        52012        
console
    XX000
        XX000
account
    56XXX
platform
    58XXX

*/




#if 1
void    GateServerContext::OnGenerateDefaultConfig()
{
    static const char * kv[][2] = {
    //gate server
    {"gate:ip","127.0.0.1"},
    {"gate:port","52010"},
    {"gate:max_clients","50000"},
    {"gate:test_mode","1"},
    {"gate:need_auth","0"},
    {"gate:agent:num","1"},//agent number
    {"gate:agent#1:channel","1 "}, //agent channel id
    {"gate:agent#1:areas","1 2 3 4 5"}, //agent zone areas


    //default agent
    {"console:port","52000"},
    {"channel:num","1"}, //agent zone areas
    {"channel:info#1:addr","tcp://127.0.0.1:51010"}, //agent zone areas
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

    //{"gate:test_mode","1"},
    //{"gate:need_auth","0"},
    //{"gate:agent:num","1"},//agent number
    //{"gate:agent#1:channel","1 "}, //agent channel id
    //{"gate:agent#1:areas","1 "}  , //agent zone areas

    iTestMode = parser.GetConfigInt("gate:test_mode");
    iNeedAuth = parser.GetConfigInt("gate:need_auth");
    mpAreaID2ChannelID.clear();
    int nCount = parser.GetConfigInt("gate:agent:num");
    char szConfigBuffer[64];
    for(int i = 0;i < nCount; ++i)
    {
        snprintf(szConfigBuffer,sizeof(szConfigBuffer),"gate:agent#%d:channel",i+1);        
        int chId = parser.GetConfigInt(szConfigBuffer);
        snprintf(szConfigBuffer,sizeof(szConfigBuffer),"gate:agent#%d:areas",i+1);        
        string sConfigValue  = parser.GetConfigString(szConfigBuffer);        
        int areas[128];
        int num = StringUtil::SplitNum(sConfigValue.c_str(),areas,128," ");
        for(int j = 0; j < num; ++j)
        {
            if(mpAreaID2ChannelID.find(areas[j])!=mpAreaID2ChannelID.end())
            {
                LOG_FATAL("area = %d is exist a other channel !",areas[j]);
                return -1;
            }
            mpAreaID2ChannelID[areas[j]] = chId;
        }           
    }
    
    return 0;   
}
#endif



