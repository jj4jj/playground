
#include "base/Log.h"
#include "base/DateTime.h"
#include "ipc/SignalHelper.h"
#include "TimerMgr.h"


#if 1
TimerMgr::TimerMgr()
{
    m_lTickUs = 0L;
    dwNextTimerID = 1;
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
    mgr.UpdateMsTime();
    mgr.m_lastTime = tvNow;
    /////////////////////////////////////////////
    //10ms
    if(lElapseTime > 10000)
    {
        mgr.Expired();
    }        
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
int     TimerMgr::AddListener(int iEvent,TimerCallBackFunc listener)
{
    if(m_mpListeners.find(iEvent) == m_mpListeners.end())
    {
        m_mpListeners[iEvent] = listener;
    }
    else
    {
        LOG_ERROR("add listener repeat event = %d",iEvent);
        return -1;
    }
    return 0;
}
void         TimerMgr::Expired()
{
    TimerMapItr it = m_mpTimers.begin();
    while(it != m_mpTimers.end() &&
          it->first <= m_curMsTime)
    {        
        uint32_t dwTimerID = it->second;
        LOG_DEBUG("timer = %u expired time = %ul ms now = %ul ms!",dwTimerID,it->first,m_curMsTime);
        TimerMapItr cur = it++;
        m_mpTimers.erase(cur);
        ////////////////////////////////////////////////////////
        TimerIDEventMapItr tit = m_mpTimerEvents.find(dwTimerID);
        if(tit != m_mpTimerEvents.end())
        {
            TimerEvent te = tit->second;
            m_mpTimerEvents.erase(tit);            
            /////////////////////////////////////////////////////
            if(te.cb)
            {
                te.cb(te.iEvent,te.ulParam);
            }
            else
            {
                //others
                EventTimeListenerMapItr eit = m_mpListeners.find(te.iEvent);
                if(eit != m_mpListeners.end())
                {
                    eit->second(te.iEvent,te.ulParam);
                }
            }            
        }
        else
        {
            LOG_INFO("timer id = %u has been canccel !",dwTimerID);
        }
    }
}
uint32_t     TimerMgr::AddTimer(int msLater,int iEvent,TimerCallBackFunc func ,uint64_t ulParam )
{
    uint64_t expiredTime = m_curMsTime + msLater;
    TimerEvent te;
    te.dwTimerID = 0;
    te.cb = func;
    te.ulParam = ulParam;
    te.iEvent = iEvent;
    te.ulExpiredTime = expiredTime; //auto remove
    
    while(m_mpTimerEvents.find(dwNextTimerID) != m_mpTimerEvents.end())
    {
        ++dwNextTimerID;
    }    
    te.dwTimerID = dwNextTimerID;
    m_mpTimers.insert(std::make_pair(expiredTime,dwNextTimerID));
    m_mpTimerEvents[dwNextTimerID] = te;
    ++dwNextTimerID;
    LOG_DEBUG("add timer = %u event = %d delay = %d",te.dwTimerID,iEvent,msLater);
    return te.dwTimerID;
}
void    TimerMgr::CancelTimer(uint32_t dwEventHandle)
{
    LOG_DEBUG("canccel timer id = %u",dwEventHandle);
    m_mpTimerEvents.erase(dwEventHandle);
}
int TimerMgr::Init(long lTickUs,UpdateTickHandler handler)
{
    m_lTickUs = lTickUs;
    Time::now(m_curTime);
    Time::now(m_lastTime);
    UpdateMsTime();
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


