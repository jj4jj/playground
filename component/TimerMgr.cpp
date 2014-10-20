
#include "base/Log.h"
#include "base/DateTime.h"
#include "ipc/SignalHelper.h"
#include "TimerMgr.h"


#if 1
TimerMgr::TimerMgr()
{
    m_lTickUs = 0L;
    m_updatTickHandler = NULL;
    bzero(&m_old_sa,sizeof(m_old_sa));
}
TimerMgr::~TimerMgr()
{
    Destroy();    
}
#endif


#if 1
void TimerMgr::UpdateTick(int , siginfo_t *, void *)
{
    TimerMgr & mgr = TimerMgr::Instance();
    struct timeval tvNow;
    Time::now(tvNow);    
    int lElapseTime = Time::uspast(tvNow,mgr.m_lastTime);
    Time::usappend(mgr.m_curTime,lElapseTime);
    mgr.m_lastTime = tvNow;
    /////////////////////////////////////////////
    if(mgr.m_updatTickHandler)
    {
        mgr.m_updatTickHandler(mgr.m_curTime,lElapseTime);
    }
    /////////////////////////////////////////////
}
//driver of a app
void TimerMgr::Destroy()
{
    if(m_old_sa.sa_handler)
    {
        sigaction(SIGALRM, &m_old_sa,NULL);
        bzero(&m_old_sa,sizeof(m_old_sa));
        m_updatTickHandler = NULL;
        LOG_DEBUG("destory timer mgr");
    }
}
int TimerMgr::Init(long lTickUs,UpdateTickHandler handler)
{
    m_lTickUs = lTickUs;
    Time::now(m_curTime);
    Time::now(m_lastTime);
    struct sigaction sa;
    bzero(&sa,sizeof(sa));
    sa.sa_sigaction = TimerMgr::UpdateTick;
    //the signal interupt system call will not restart (EINTR)
    //handler has info
    //when handler is calling , reset signal 
    sa.sa_flags = SA_RESTART | SA_SIGINFO ;
    SignalHelper::ClearSigSet(&sa.sa_mask);
    //when timer is calling , block the quit signal
    SignalHelper::AddSigSet(&sa.sa_mask,SIGQUIT);
    SignalHelper::AddSigSet(&sa.sa_mask,SIGINT);        
    ////////////////////////////////////////////
    SignalHelper::BlockSignal(SIGALRM);            
    if(SignalHelper::SignalAction(SIGALRM,
        &sa,
        &m_old_sa))
    {
        SignalHelper::UnBlockSignal(SIGALRM);
        LOG_ERROR("signal error = %d for = %s",errno,strerror(errno));
        return -1;
    }
	m_itv.it_interval.tv_sec = (lTickUs / 1000000);
	m_itv.it_interval.tv_usec = (lTickUs % 1000000);
	m_itv.it_value.tv_sec = (lTickUs / 1000000);
	m_itv.it_value.tv_usec = (lTickUs % 1000000);
	int ret =  setitimer(ITIMER_REAL, &m_itv, NULL);       
    SignalHelper::UnBlockSignal(SIGALRM);
    if(ret)
    {
        LOG_ERROR("setitimer error ! for errno = %d errorstr = %s",
                errno,strerror(errno));
        return -1;
    }   
    m_updatTickHandler = handler;
    return ret;
}
#endif


