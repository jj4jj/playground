#include "base/File.h"
#include "base/Log.h"
#include "base/CommonMacro.h"

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

    hook_coredump = parser.GetConfigInt("hook_coredump");
    
    /////////////////////////common config////////////////////////
    tickCountUs = parser.GetConfigInt("tick_count_us",100);//10000
    tickPollCount = parser.GetConfigInt("tick_poll_num",1);//

    uniq_process = parser.GetConfigInt("uniq_process",0);
    lockFilePath = parser.GetConfigString("file_lock_path","./lock.file");;

    //channel
    int ichnlNum = parser.GetConfigInt("channel:num",-1);
    GateChannel      chnl;
    char keyBuffer[64];
    channels.clear();
    channelName  = parser.GetConfigString("channel:name");
    for(int i = 0;i < ichnlNum ; ++i)
    {
        SNPRINTF(keyBuffer,sizeof(keyBuffer),"channel:info#%d:id",i+1);
        chnl.id = parser.GetConfigInt(keyBuffer,-1);
        if(chnl.id < 0)
        {
            LOG_ERROR("get config key = %s error !",keyBuffer);
            return -1;
        }


        SNPRINTF(keyBuffer,sizeof(keyBuffer),"channel:info#%d:listener",i+1);        
        int listen = parser.GetConfigInt(keyBuffer,-1);
        if(listen < 0)
        {
            LOG_ERROR("get config key = %s error !",keyBuffer);
            return -1;
        }
        chnl.listener = (listen > 0 );
        SNPRINTF(keyBuffer,sizeof(keyBuffer),"channel:info#%d:addr",i+1);
        chnl.channelAddr = parser.GetConfigString(keyBuffer);
        if(strlen(chnl.channelAddr.c_str()) < 1)
        {
            LOG_ERROR("get config key = %s error !",keyBuffer);
            return -1;
        }

        channels.push_back(chnl);
    }


    /////////////////////////////////////////////////////////////
    closing = 0;


    //custom config            

    return OnInit();
}
//common generate 
void    AppContext::GenerateDefaultConfig(const char* pszConfigFile)
{
    assert(pszConfigFile);
    string sConfFile = pszConfigFile;
    string sLogFile = pszConfigFile;
    string sLockFile = pszConfigFile;
    if(sConfFile.find(".conf") == string::npos)
    {
        sConfFile += ".conf";
    }
    sConfFile += ".default";
    if(sLogFile.find(".log") == string::npos)
    {
        sLogFile += ".log";
    }
    if(sLockFile.find(".lock") == string::npos)
    {
        sLockFile += ".lock";
    }
    
    LOG_INFO("config file %s is not exist , so create it use default option \nfinal cfg file = %s",pszConfigFile,sConfFile.c_str());        
    parser.Create();

    static const char * kv[][2] = {
    //root
    {"log_file",sLogFile.c_str()},
    {"log_file_size","10240"},
    {"log_max_file_num","20"},
    {"log_min_level","2"},
    {"daemon","0"},
    {"hook_coredump","0"},        
    {"tick_count_us","10000"},
    {"tick_poll_num","1"},
    {"uniq_process","1"},//support path system none
    {"file_lock_path",sLockFile.c_str()},
    //console
    {"console:ip","127.0.0.1"},
    {"console:port","28800"},
    //channel
    {"channel:name","ch"},
    {"channel:num","1"},
    {"channel:info#1:id","1"},//connect other channel
    {"channel:info#1:addr","tcp://127.0.0.1:18800"},
    {"channel:info#1:listener","0"},

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

