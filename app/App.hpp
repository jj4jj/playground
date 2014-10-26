#pragma once

#include "base/Singleton.hpp"
#include "base/stdinc.h"
#include "base/File.h"
#include "base/Log.h"
#include "base/Buffer.h"
#include "base/DateTime.h"
#include "base/CommonMacro.h"
#include "net/UdpSocket.h"
#include "net/UdpDriver.h"
#include "utility/Daemon.h"
#include "ChannelMsgProxy.h"
#include "AppContext.h"

enum    AppControlCommonCloseReason
{
    APP_CTRL_CLOSING_EXCEPTION  = 0,
    APP_CTRL_CLOSING_RESTART    = 1,
    APP_CTRL_CLOSING_STOP       = 2,
    APP_CTRL_CLOSING_COREDUMP   = 3, //todo
};



class App
{
public:
    //return 0 is ok , otherwise exit prcess
    virtual int     OnInit();

    //return 0 is ok , otherwise exit prcess (after ondestroy)    
    virtual int     OnStart();
    
    //control command process return msg to controler
    virtual string  OnCtrl(const std::vector<string> & cmdLine);
    //tick 
    virtual int     OnTick(int64_t lElapseTime);
    //poll system
    //is return 0 or less than 0 , system will sleep a while
    //if return > 0 , system will be busying .
    virtual int     OnPoll(int iRecommendPollNum = 1);

    //system will close for closing Reason
    //return <= 0 , will close finish .
    //return > 0 , will be busying .
    virtual int     OnClosing(int closingReason);

    //destroy sth uninitializing   
    //eg. print some profileing info
    virtual int     OnDestroy();
    virtual void    OnCoreDump();
/////////////////////////////////////////////////////////////////////////////////
public:
    inline  timeval &   GetTime()
    {
        return  m_pAppCtx->curTime;
    }  
    inline AppContext * GetContext()
    {
        return m_pAppCtx;
    }
    inline ChannelMsgProxy & GetChannelProxy()
    {
        return m_proxy;
    }
public:
    int     Init(AppContext * _ctx , bool forStop = false);   
    int     Start();
    int     Shutdown(int closing);
    int     Poll(int iRecommendPollNum = 1);
    int     Tick(int64_t lElapseUsTime);
    string  Ctrl(const std::vector<string> & cmdLine);
    int     Closing(int closingReason);
    int     Destroy();

private:
    void    InitSignal();
    int     InitLockFile(bool forStop);
    int     InitLog();
    int     InitConsole();
    static  void    UpdateTick(struct timeval & tvNow, int64_t lElapseTime);
protected :
    App();
    virtual    ~App();
    DeclareSingltonSupport(App)
private:
    AppContext *                m_pAppCtx;
    UdpDriver                   m_consoleDrv;
    ChannelMsgProxy             m_proxy;
    File                        m_lockFile;
    //////////////////////////////////////////////////////////////////////////////////
public:    
    template<class AppContextType>
    static int main(int argc , char* argv []);     
};
//--------------------------------------------------------------------------------
extern App* GetApp();
//////////////////////////////////////////////////////////////////////////////////
template<class AppContextType>
int App::main(int argc , char* argv [])
{
    AppContextType appCtx;
    if(argc < 2)
    {
        appCtx.GenerateDefaultConfig(argv[0]);
        return -1;
    }
    bool    forStop = false;
    if(argc > 2)
    {        
        if(0 == strcmp(argv[2],"stop"))
        {
            forStop = true;
        }
        else if(0 == strcmp(argv[2],"restart"))
        {
            forStop = true;
        }
        else
        {
            fprintf(stderr,"not support arg = %s",argv[2]);
            return -1 ;
        }
    }
    //////////////////////////////////////////////////////////////
    if(appCtx.Init(argv[1]))
    {
        printf("server context init error !\n");
        return -1;
    }    
    ///////////////////daemon ////////////////////////////////////
    if(appCtx.daemon)
    {
        //daemonlize
        Daemon::Instance().Create();
    }
    /////////////////////////////////////////////////////////
    App *  pApp = GetApp();
    assert(pApp);
    if(pApp->Init(&appCtx,forStop))
    {
        LOG_ERROR("Init error !");
        pApp->Destroy();
        return -1;
    }
    /////////////////////////////////////////////////////////
    appCtx.closing = 0;
    /////////////////////////////////////////////////////////
    if(0 == pApp->Start())
    {
        int proc = 0;
        while(true)
        {        
            if(0 == appCtx.closing)
            {
                proc = pApp->Poll(appCtx.tickPollCount);
                if( proc <= 0 )
                {
                    usleep(appCtx.tickCountUs);
                }
            }
            else
            {            
                //there is no sth todo
                proc = pApp->Closing(appCtx.closing);
                if( proc <= 0 )
                {
                    break;
                }
            }
        }
    }
    else
    {
        LOG_ERROR("start error !");
    }
    return pApp->Destroy();
} 
////////////////////////////////////////////////////////

#define DeclareAppMain(AppCtxName,AppName) \
App* GetApp()\
{\
    return Singleton<AppName>::instance();\
};\
int main(int argc , char * argv[])\
{\
    return App::main<AppCtxName>(argc,argv);\
}// 
//////////////////////////////////////////////////////




