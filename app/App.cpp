
#include "net/UdpSocketHandler.h"
#include "ipc/SignalHelper.h"
#include "component/TimerMgr.h"
#include "App.hpp"





#if 1
class AppConsoletHandler : public UdpSocketHandler
{
public :
    AppConsoletHandler(App * _app):app(_app){}
    virtual  ~AppConsoletHandler(){app = NULL;}
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
        LOG_DEBUG("recv command req = [%s]",recvBuffer.pBuffer);
        SplitCMDLine(line,sCMDLine," ");
        string result = app->Ctrl(sCMDLine);
        LOG_DEBUG("send command rsp = [%s]",result.c_str());            
        return udpSock.SendTo(Buffer(result.c_str()),addr);
    }
private:
    App * app;
};
#endif


#if 1
App::App():m_pAppCtx(NULL){}
App::~App(){m_pAppCtx = NULL;}  
#endif

#if 1
//return 0 is ok , otherwise exit prcess
int     App::OnInit()
{
    LOG_DEBUG("OnInit");
    return 0;
}
//return 0 is ok , otherwise exit process
int     App::OnStart()
{
    LOG_DEBUG("OnStart");
    return 0;
}

//control command process
string     App::OnCtrl(const std::vector<string> & cmdLine)
{
    return "not support !";
}

//tick 
int     App::OnTick(int64_t lElapseUsTime)
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
void     App::OnCoreDump()
{
    LOG_ERROR("program coredump , closing !!!! ");
    while(Closing(APP_CTRL_CLOSING_COREDUMP) > 0 )
    {
        usleep(10000);
    }
}
#endif

#if 1
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
//us
int     App::Tick(int64_t lElapseUsTime)
{    
    Time::usappend(m_pAppCtx->runTime,lElapseUsTime);
    return OnTick(lElapseUsTime);
}
int     App::Destroy()
{    
    LOG_INFO("app is destroying ...");
    int ret = OnDestroy();
    m_pAppCtx = NULL;
    m_lockFile.Unlock();
    m_lockFile.Close();

    ///////////////////////////////
    TimerMgr::Instance().Destroy();
    m_proxy.Destroy();
    m_consoleDrv.Destroy();    
    return ret;
}

static  void OnSignalCoreDump(int sign, siginfo_t * siginfo, void * p)
{
    App * app =  GetApp();
    app->OnCoreDump();    
    ////////////////////////////////////////////
    SignalHelper::SetSigHandler(SIGSEGV,SIG_DFL);
    SignalHelper::SendSelfSignal(SIGSEGV);
}
static void OnSignal(int signo)
{
    App * app =  GetApp();
    if(signo == SIGUSR1)
    {
        LOG_INFO("receive usr1 signal , system will exit for restarting ... set closing");
        app->Shutdown(APP_CTRL_CLOSING_RESTART);
    }
    else if(signo == SIGUSR2)
    {
        LOG_INFO("receive usr2 signal , system will exit for stopping ... set closing");    
        app->Shutdown(APP_CTRL_CLOSING_STOP);
    }
    else
    {
        LOG_INFO("receiver unkown signo = %d",signo);
    }
}
void    App::InitSignal()
{
    SignalHelper::IgnoreSignal(SIGPIPE);
    SignalHelper::IgnoreSignal(SIGTERM);
    SignalHelper::SetSigHandler(SIGUSR1,OnSignal);
    SignalHelper::SetSigHandler(SIGUSR2,OnSignal);
    /////////////////////////////////////////////////////////
    if(m_pAppCtx->hook_coredump)
    {
        struct sigaction   sig_segv_action;   
        bzero(&sig_segv_action,sizeof(sig_segv_action));
        sig_segv_action.sa_sigaction = OnSignalCoreDump;
        sig_segv_action.sa_flags = SA_SIGINFO|SA_RESETHAND;
        SignalHelper::ClearSigSet(&sig_segv_action.sa_mask);
        SignalHelper::SignalAction(SIGSEGV,&sig_segv_action,NULL);
    }
    ////////////////////////////////////////////////////////
    //SignalHelper::IgnoreSignal(SIGINT);
}
void  App::UpdateTick(timeval & tvNow,int64_t lElapseTime)
{
    App & app =  *GetApp();
    if(app.m_pAppCtx)
    {
        app.m_pAppCtx->curTime = tvNow;
        app.Tick(lElapseTime);
    }    
}
string  App::Ctrl(const std::vector<string> & cmdLine)
{
    string mainCMD = cmdLine[0];
    LOG_DEBUG("recv command control = [%s]",cmdLine[0].c_str());    
    char buffer[128];
    string rsp = mainCMD + ":";
    if(mainCMD == "quit")
    {
        m_pAppCtx->closing = 1;
        return "system is closing ...";
    }
    else if(mainCMD == "time")
    {
        rsp += Time(GetTime()).ToString();
        return rsp;
    }
    else if(mainCMD == "rtime")
    {
        int runseconds = m_pAppCtx->runTime.tv_sec;
        snprintf(buffer,sizeof(buffer),"%d hours %d min %d seconds",runseconds/3600,
                (runseconds%3600)/60,
                 runseconds%60);
    }
    else
    {
        return OnCtrl(cmdLine);
    }
    rsp += buffer;
    return rsp;
}
int    App::InitLockFile(bool forStop)
{
    if(m_pAppCtx->uniq_process)
    {
        LOG_INFO("uniq process checking file path = [%s] ...",m_pAppCtx->lockFilePath.c_str());
        //if file not exist
        //ok , create it .
        //else
        //read pid
        //send pid signal stopping
        //init
        bool existFile = File::Exist(m_pAppCtx->lockFilePath.c_str());
        if(!existFile)
        {
            //touch
            m_lockFile.Open(m_pAppCtx->lockFilePath.c_str(),"w+");
            m_lockFile.Close();            
        }
        int ret = m_lockFile.Open(m_pAppCtx->lockFilePath.c_str(),"r+");
        if(ret)
        {
            LOG_ERROR("lock file = %s open error !",m_pAppCtx->lockFilePath.c_str());
            return -1;
        }
        char pidBuffer[64];        
        if(existFile)
        {
            if(true)//m_lockFile.ExcluLock())//it's blocking io
            {
                //send signal  by context assign
                LOG_ERROR("lock file = %s get lock error , pls be sure there is no another process running !",m_pAppCtx->lockFilePath.c_str());
                //send signal
                m_lockFile.Seek();
                m_lockFile.GetLine(pidBuffer,sizeof(pidBuffer));  
                if(strlen(pidBuffer) == 0)
                {
                    LOG_ERROR("read pid from lock file error ! maybe u need kill another program manually");
                    m_lockFile.Close();
                    return -1;
                }            
                pidBuffer[sizeof(pidBuffer)-1] = '\0';            
                int pid = atoi(pidBuffer);
                LOG_INFO("old process id=[%d] is running ...",pid);
                //todo
                if(!forStop)
                {
                    LOG_INFO("stop old process = [%d] for restarting ...",pid);
                    SignalHelper::SendProcessSignal(SIGUSR1,pid);
                    //check thread is over ?                
                    LOG_INFO("waiting old process destroy (todo)");

                }
                else
                {
                    LOG_INFO("stop old process = [%d] for stopping ...",pid);
                    SignalHelper::SendProcessSignal(SIGUSR2,pid);
                    return -1;
                }
            }            
        }
        else
        {
            if(false)//m_lockFile.ExcluLock())
            {
                LOG_ERROR("get file = %s lock error ! impossible !!! ",m_pAppCtx->lockFilePath.c_str());
                return -1;
            }
        }
        //lock success write pid
        pid_t pid = getpid();
        SNPRINTF(pidBuffer,sizeof(pidBuffer),"%u",pid);        
        LOG_INFO("write lock file pid = %u",pid);
        m_lockFile.Seek();
        if(m_lockFile.Write(pidBuffer,strlen(pidBuffer)+1) < 0)
        {
            LOG_ERROR("warnning write lock file pid = %u error for errstr = %s",
                    pid,strerror(errno));
        }
        m_lockFile.Flush();
    }
    return 0;
}
int     App::InitConsole()
{
    IniConfigParser & parser = m_pAppCtx->parser;
    //console
    string sConsoleIP = parser.GetConfigString("console:ip");        
    int console_port = parser.GetConfigInt("console:port");
    const char* pszConsoleIP = sConsoleIP.c_str();

    m_consoleDrv.Init(10);
    m_consoleDrv.SetHandler(UdpSocketHandlerSharedPtr(new AppConsoletHandler(this)));
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
    return 0;
}
int     App::InitLog()
{
    IniConfigParser & parser = m_pAppCtx->parser;    
    //log
    string sLogFileName = parser.GetConfigString("log_file");
    int logFileSize = parser.GetConfigInt("log_file_size",1024);
    int maxLogFileNum = parser.GetConfigInt("log_max_file_num",20);
    int loglevel = parser.GetConfigInt("log_min_level",2);
    const char* pszLogFileName = sLogFileName.c_str();
    LOG_INFO("init log info file = %s level = %d size = %d num = %d",
        pszLogFileName,loglevel,logFileSize,maxLogFileNum);
    if(pszLogFileName)  
    {
        Log::Instance().Init( pszLogFileName,(Log::LogLevel)loglevel,logFileSize,maxLogFileNum);
    }
    return 0;
}
int      App::Shutdown(int closing)
{
    if(m_pAppCtx->closing == 0)
    {
        LOG_INFO("shutdown system reason = %d",closing);        
        m_pAppCtx->closing = closing;
        return 0;
    }
    else
    {
        LOG_ERROR("system is already shutdowning ... ");
        return -1;
    }
}
int     App::Init(AppContext * _ctx , bool forStop)
{
    m_pAppCtx = _ctx;
    
    IniConfigParser & parser = m_pAppCtx->parser;
    
    //common option
    fprintf(stderr,"app is initializing ... \n");
    ///////////////////daemon ////////////////////////////////////
    int daemon = parser.GetConfigInt("daemon");    
    if(daemon)
    {
        //daemonlize
        Daemon::Instance().Create();
    }    
    /////////////////////  log  ///////////////////////////////
    if(InitLog())
    {
        LOG_FATAL("Log init error !");
        return -1;
    }
    ////////////////////////lock file ////////////////////////////
    if(InitLockFile(forStop))
    {
        LOG_ERROR("init lock file error !");
        return -1;
    }
    //////////////////////////////////////////////////////////
    string configString;
    parser.VisualConfig(configString);
    LOG_INFO("server config :\n%s",configString.c_str());
    //////////////////////////////////////////////////////////

    ////////////////////// signal  ///////////////////////////////
    InitSignal();    

    ////////////////////// console //////////////////////////////
    if(InitConsole())
    {
        LOG_FATAL("console init error !");
        return -1;
    }
  
    /////////////////// channel m_proxy ///////////////////////////
    int ret = m_proxy.Init(m_pAppCtx);
    if(ret)
    {
        LOG_FATAL("m_proxy init error = %d",ret);
        return -1;
    }
    

    ///////////////////// timer //////////////////////////////////
    if(TimerMgr::Instance().Init(m_pAppCtx->tickCountUs,UpdateTick))
    {
        LOG_FATAL("timer mgr init error !");
        return -1;
    }
    m_pAppCtx->curTime = TimerMgr::Instance().GetCurTime();


    ////////////////////// app  /////////////////////////////////
    ret = OnInit();
    if(ret)
    {
        LOG_FATAL("App init resutl error = %d ! ",ret);
        return -1;
    }  
    
    return 0;
}   
int     App::Start()
{
   int ret = OnStart();
   if(ret)
   {
        LOG_FATAL("App Start result error = %d !",ret);
        return -1;
   }
   return 0;
}

#endif
