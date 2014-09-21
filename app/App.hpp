#pragma once

#include "base/stdinc.h"
#include "base/Log.h"
#include "base/Buffer.h"
#include "base/DateTime.h"
#include "base/CommonMacro.h"
#include "net/UdpSocket.h"
#include "net/UdpDriver.h"
#include "utility/Daemon.h"
#include "AppContext.h"

class App
{
public:
    //return 0 is ok , otherwise exit prcess
    virtual int     OnInit();
    //control command process
    virtual string     OnCtrl(const std::vector<string> & cmdLine);
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
public:
    int     Poll(int iRecommendPollNum = 1);
    int     Closing(int closingReason);
    //ms
    int     Tick(int64_t lElapseTime);
    int     Destroy();
    int     Init(AppContext * ctx);    
public:
    App():ctx(NULL){}
    virtual    ~App(){}  
private:
    AppContext * ctx;
    UdpDriver  m_consoleDrv;
    UdpSocketHandlerSharedPtr   ptrConsoleHandler;
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
    timeval now ;    
    int proc = 0;
    int64_t lElapseTime = 0L; 
    int64_t lRemainTime = 0L;
    while(true)
    {
        
        Time::now(now);
        lElapseTime = Time::uspast(now,ctx.curTickStart);
        Time::usappend(ctx.curTime,lElapseTime);
        ctx.curTickStart = now;
        ///////////////////////////////////////////////////////////////////////        
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
            proc = app.Poll();
            app.Tick(lElapseTime);
        }

        Time::now(now);
        lElapseTime =  Time::uspast(now,ctx.curTickStart);
        lRemainTime = ctx.tickCountUs - lElapseTime ;
        if(lRemainTime > 0 && 0 == proc )
        {
            LOG_DEBUG("sleep lremain=%d proc = %d elapse = %d",
                lRemainTime,proc,lElapseTime);
            usleep(lRemainTime);
        }
    }
    return app.Destroy();
} 

