#include "base/File.h"
#include "base/Log.h"
#include "AppContext.h"

//read config and init the common attr
int     AppContext::Init(const char * pszConfigFile)
{
    if(!pszConfigFile)
    {
        return -1;
    }
    if(parser.ReadConfigFile(pszConfigFile))
    {        
        return -2;
    }
    /////////////////////////common config/////////////////////////
    tickCountUs = parser.GetConfigInt("tick_count_us",100);//10000
    tickPollCount = parser.GetConfigInt("tick_poll_num",1);//
    





    //custom config            
    return OnInit();
}
//common generate 
void    AppContext::GenerateDefaultConfig(const char* pszConfigFile)
{
    assert(pszConfigFile);
    char szFileName[32];
    string sConfFile = pszConfigFile;
    string sLogFile = pszConfigFile;
    if(sConfFile.find(".cfg") == string::npos)
    {
        sConfFile += ".cfg";
    }
    sConfFile += ".default";
    if(sLogFile.find(".log") == string::npos)
    {
        sLogFile += ".log";
    }
    LOG_INFO("config file  %s is not exist , so create it use default option root = %s final cfg file = %s\n",pszConfigFile,szFileName,sConfFile.c_str());        
    parser.Create();

    static const char * kv[][2] = {
    //root
    {"log_file",sLogFile.c_str()},
    {"log_file_size","10240"},
    {"log_max_file_num","20"},
    {"log_min_level","2"},
    {"daemon","0"},
    {"tick_count_us","100"},
    {"tick_poll_num","1"},
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
        parser.CreateConfig(v);
        ++i;
    }
    ///////////////////////////////////////////////////////
    OnGenerateDefaultConfig();                
    parser.DumpConfig(sConfFile.c_str());
    LOG_INFO("create default config ok !\n");
}
void    AppContext::OnGenerateDefaultConfig()
{
    LOG_DEBUG("OnGenerateDefaultConfig");
}
int     AppContext::OnInit()
{
    LOG_DEBUG("OnInit");
    return 0;
}

