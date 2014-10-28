#include "base/File.h"
#include "base/Log.h"
#include "base/CommonMacro.h"
#include "datacenter/ConfigCenter.h"
#include "AppContext.h"

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
    channel listen
        52010
    tcp server listen
        52100
        52101
        52102        
console
    XX000
        XX000
account
    56XXX
platform
    58XXX

*/

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
    {"console:port","51000"},
    //channel
    {"channel:name","ch"},
    {"channel:local:addr","tcp://127.0.0.1:51010"},
    {"channel:local:name","gate#1"},
    {"channel:num","1"},
    {"channel:info#1:addr","tcp://127.0.0.1:52010"},
    {"channel:info#1:name","agent#1"},
    //config center
    {"configcenter:ip","127.0.0.1"},
    {"configcenter:port","6379"},
    {"configcenter:dbidx","0"},

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

    ////////////////////configcenter////////////////////////////
    confcenter.ip = parser.GetConfigString("configcenter:ip");
    confcenter.port = parser.GetConfigInt("configcenter:port");
    confcenter.dbidx = parser.GetConfigInt("configcenter:dbidx");
    ConfigCenter & cc = ConfigCenter::Instance();
    int ret = cc.Init(confcenter.ip.c_str(), confcenter.port,confcenter.dbidx);
    if(ret)
    {
        LOG_ERROR("config center init error !");
        return -1;
    }
    //////////////////////////////////////////////////////////////
    
    
    /////////////////////////common config////////////////////////
    tickCountUs = parser.GetConfigInt("tick_count_us",100);//10000
    tickPollCount = parser.GetConfigInt("tick_poll_num",1);//
    uniq_process = parser.GetConfigInt("uniq_process",0);
    lockFilePath = parser.GetConfigString("file_lock_path","./lock.file");;
    daemon = parser.GetConfigInt("daemon",0);
    hook_coredump = parser.GetConfigInt("hook_coredump");


    ////////////////channel//////////////////////////////////////////////
    int ichnlNum = parser.GetConfigInt("channel:num",-1);
    ChannelConfig      chnl;
    char keyBuffer[64];
    channels.clear();
    channelName  = parser.GetConfigString("channel:name");
    localChannelName = parser.GetConfigString("channel:local:name");    
    localChannelAddr = parser.GetConfigString("channel:local:addr");
    const char* pszLocalChAddr = cc.GetConfig(localChannelName.c_str());
    if(!pszLocalChAddr)
    {
        LOG_INFO("local addr not found in config center , register it !");            
        if(localChannelAddr.length() > 0)
        {
            cc.SetConfig(localChannelName.c_str(),localChannelAddr.c_str());
        }
        else
        {
           return -1;
        }
    }
    else
    {
        LOG_INFO("get cc local name = %s , addr = %s",
                localChannelName.c_str(),pszLocalChAddr);
        if(localChannelAddr != string(pszLocalChAddr))
        {
            LOG_ERROR("local addr not match cc !",pszLocalChAddr);
            return -1;
        }
    }
        
    for(int i = 0;i < ichnlNum ; ++i)
    {
        SNPRINTF(keyBuffer,sizeof(keyBuffer),"channel:info#%d:name",i+1);
        chnl.name = parser.GetConfigString(keyBuffer);
        const char* pszChName = cc.GetConfig(chnl.name.c_str());
        SNPRINTF(keyBuffer,sizeof(keyBuffer),"channel:info#%d:addr",i+1);
        chnl.channelAddr = parser.GetConfigString(keyBuffer);
        if(!pszChName)
        {
            LOG_ERROR("get config key = %s error and not found in cc !",keyBuffer);
            return -1;                
        }            
        if(chnl.channelAddr  != string(pszChName))
        {
            LOG_ERROR("not match chanenl name = %s name = %s , %s use cc",
                     chnl.name.c_str(),chnl.channelAddr.c_str(),pszChName);
        }
        chnl.channelAddr = pszChName;
        /////////////////////////////////////////////////////////////////////////
        
        channels.push_back(chnl);
    }  
    /////////////////////////////////////////////////////////////
    closing = 0;
    runTime.tv_sec = 0;
    runTime.tv_usec = 0;

    //custom config            

    ret = OnInit();
    cc.Destroy();
    return ret;
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

