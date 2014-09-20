
#include "net/UdpSocketHandler.h"
#include "App.hpp"
#if 1
class AppConsoletHandler : public UdpSocketHandler
{
public :
    AppConsoletHandler(App * _app):app(_app){}
    virtual  ~AppConsoletHandler(){}
public:
    int    SplitCMDLine(string & line,vector<string> & list,const char* pszDeli = " ")
    {
        list.clear();
        //placeholder
        line.append(pszDeli);
        int len = strlen(pszDeli);
        int start = 0;
        int end = line.find(pszDeli,start);
        while(start < (int)line.length() &&
              end != (int)string::npos )
        {
            string seg = line.substr(start,end-start);
            if(strcmp(seg.c_str(),pszDeli) && end > start)
            {
                list.push_back(seg); 
            }
            ///////////////////////////////////////////////
            start = end + len;
            end = line.find(pszDeli,start);
        }
        return 0;
    }
    int    OnDataRecv(UdpSocket& udpSock,const Buffer & recvBuffer,const SockAddress& _addr)
    {
        SockAddress addr(_addr);
        char * p =  (char*)(recvBuffer.pBuffer);
        p[recvBuffer.iUsed] = 0;
        vector<string> sCMDLine;
        string line = p;
        SplitCMDLine(line,sCMDLine," ");
        string result = app->OnCtrl(sCMDLine);
        return udpSock.SendTo(Buffer(result.c_str()),addr);
    }
private:
    App * app;
};
#endif


#if 1

//return 0 is ok , otherwise exit prcess
int     App::OnInit()
{
    LOG_DEBUG("OnInit");
    return 0;
}
//control command process
string     App::OnCtrl(const std::vector<string> & cmdLine)
{
    LOG_DEBUG("OnCtrl");
    return "onctrl";
}

//tick 
int     App::OnTick(int64_t lElapseTime)
{
    LOG_DEBUG("OnTick");
    return 0;
}
//poll system
int     App::OnPoll(int iRecommendPollNum )
{
    LOG_DEBUG("OnPoll");
    return 0;
}
//system will close for closing Reason
int     App::OnClosing(int closingReason)
{
    LOG_DEBUG("OnClosing");
    return 0;        
}
//destroy sth uninitializing
int     App::OnDestroy()
{
    LOG_DEBUG("OnDestroy");
    return 0;
}

int     App::Poll(int iRecommendPollNum )
{
    //console
    m_consoleDrv.Loop(1,0);
    return OnPoll(iRecommendPollNum);
}
int     App::Closing(int closingReason)
{
    return OnClosing(closingReason);
}
//ms
int     App::Tick(int64_t lElapseTime)
{
    return OnTick(lElapseTime);
}
int     App::Destroy()
{
    return OnDestroy();
}
int     App::Init(AppContext * ctx)
{
    IniConfigParser & parser = *(ctx->parser);


    //common option
    /////////////////////////////////////////////////////
    //console
    string sConsoleIP = parser.GetConfigString("console:ip");        
    int console_port = parser.GetConfigInt("console:port");

    //daemon
    int daemon = parser.GetConfigInt("daemon");    

    //log
    string sLogFileName = parser.GetConfigString("log_file");
    int logFileSize = parser.GetConfigInt("log_file_size",1024);
    int maxLogFileNum = parser.GetConfigInt("log_max_file_num",20);
    int loglevel = parser.GetConfigInt("log_min_level",2);

    const char* pszLogFileName = sLogFileName.c_str();
    const char* pszConsoleIP = sConsoleIP.c_str();
       

    string configString;
    parser.VisualConfig(configString);
    LOG_INFO("config :\n%s",configString.c_str());

    LOG_INFO("init log info file = %s level = %d size = %d num = %d",
        pszLogFileName,loglevel,logFileSize,maxLogFileNum);
    if(pszLogFileName)  
    {
        Log::Instance().Init( pszLogFileName,(Log::LogLevel)loglevel,logFileSize,maxLogFileNum);
    }
    if(daemon)
    {
        //daemonlize
        Daemon::Instance().Create();
    }    
    
    ////////////////////////////////////////////////////////////////////////
    m_consoleDrv.Init(10);
    UdpSocketHandlerSharedPtr pHdlr(new AppConsoletHandler(this));
    m_consoleDrv.SetHandler(pHdlr);
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
    m_consoleDrv.AddSocket(console.GetFD());    
    ptrConsoleHandler = pHdlr;        
    /////////////////////////////////////////////////////////////////
    int iRet = OnInit();
    if(iRet)
    {
        LOG_FATAL("App init resutl error = %d ! ",iRet);
        return -1;
    }           
    Time::now(ctx->curTime);    
    ctx->curTickStart = ctx->curTime;
    return 0;
}   
#endif
