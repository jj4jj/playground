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

class App : public Singleton<App>
{
public:
    //return 0 is ok , otherwise exit prcess
    virtual int     OnInit();
    //control command process
    virtual string  OnCtrl(const std::vector<string> & cmdLine);
    //tick 
    virtual int     OnTick(int64_t lElapseTime);
    //poll system
    virtual int     OnPoll(int iRecommendPollNum = 1);
    //system will close for closing Reason
    virtual int     OnClosing(int closingReason);
    //destroy sth uninitializing
    virtual int     OnDestroy();
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
    //
    int     Poll(int iRecommendPollNum = 1);
    //us
    int     Tick(int64_t lElapseTime);
    string  Ctrl(const std::vector<string> & cmdLine);
    int     Closing(int closingReason);
    int     Destroy();

private:
    void    InitSignal();
    int     InitLockFile();
    int     InitLog();
    int     InitConsole();
    static  void    UpdateTick(struct timeval & tvNow, int64_t lElapseTime);
public:
    App();
    virtual    ~App();
private:
    AppContext * ctx;
    UdpDriver  m_consoleDrv;
    ChannelProxy                proxy;
    File                        m_lockFile;
    //////////////////////////////////////////////////////////////////////////////////
public:    
    template<class AppContextType ,
             class AppType >
    static int main(int argc , char* argv []);
     
};



//////////////////////////////////////////////////////////////////////////////////
template<class AppContextType ,
         class AppType >
int App::main(int argc , char* argv [])
{
    AppContextType ctx;
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
    AppType app;
    if(app.Init(&ctx))
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
            if(0 == app.Closing(ctx.closing))
            {
                break;
            }
        }
        else
        {            
            proc = app.Poll(ctx.tickPollCount);
        }
        if( 0 == proc )
        {
            usleep(ctx.tickCountUs);
        }
    }
    return app.Destroy();
} 

