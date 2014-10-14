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
#include "ChannelProxy.h"
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
    inline  timeval &   Getime()
    {
        return  ctx->curTime;
    }  
    inline AppContext * GetContext()
    {
        return ctx;
    }
    inline ChannelProxy & GetChannelProxy()
    {
        return proxy;
    }
public:
    int     Init(AppContext * _ctx);   
    int     Poll(int iRecommendPollNum = 1);
    int     Tick(int64_t lElapseUsTime);
    string  Ctrl(const std::vector<string> & cmdLine);
    int     Closing(int closingReason);
    int     Destroy();

private:
    void    InitSignal();
    int     InitLockFile();
    int     InitLog();
    int     InitConsole();
    static  void    UpdateTick(struct timeval & tvNow, int64_t lElapseTime);
protected :
    App();
    virtual    ~App();
    DeclareSingltonSupport(App)
private:
    AppContext * ctx;
    UdpDriver  m_consoleDrv;
    ChannelProxy                proxy;
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
    AppContextType ctx;
    App *  pApp = GetApp();
    assert(pApp);
    if(argc < 2)
    {
        ctx.GenerateDefaultConfig(argv[0]);
        return -1;
    }
    if(ctx.Init(argv[1]))
    {
        printf("server context init error !\n");
        return -1;
    }    
    /////////////////////////////////////////////////////////
    if(pApp->Init(&ctx))
    {
        return -1;
    }
    /////////////////////////////////////////////////////////
    int proc = 0;
    while(true)
    {        
        if(ctx.closing)
        {
            //there is no sth todo
            proc = pApp->Closing(ctx.closing);
            if( proc <= 0 )
            {
                break;
            }
        }
        else
        {            
            proc = pApp->Poll(ctx.tickPollCount);
        }
        if( proc <= 0 )
        {
            usleep(ctx.tickCountUs);
        }
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




