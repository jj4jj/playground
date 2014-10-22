#pragma once
#include "base/stdinc.h"
#include "base/Singleton.hpp"




typedef void (* UpdateTickHandler)(struct timeval & now , int64_t  lElapseTime);
typedef void (*TimerCallBackFunc)(int iEvent,uint64_t ulParam);
struct  TimerEvent
{
    int iEvent;
    uint64_t    ulParam;    
    TimerCallBackFunc   cb;
    uint64_t    ulExpiredTime;
    uint32_t    dwTimerID;
    TimerEvent()
    {
        bzero(this,sizeof(*this));
    }
};

//driver of a app
class TimerMgr : public Singleton<TimerMgr>
{
private:
    TimerMgr();
    ~TimerMgr();
    DeclareSingltonSupport(TimerMgr)
public:
    int     Init(long lTickUs,UpdateTickHandler handler = NULL);
    void    Destroy();
    inline  void    UpdateMsTime(){ m_curMsTime = m_curTime.tv_sec*1000 + m_curTime.tv_usec/1000;}
    inline  const timeval & GetCurTime(){return m_curTime;}
    int     AddListener(int iEvent,TimerCallBackFunc listener);
    void    Expired();
    //return timer id , when you want to cancel
    uint32_t    AddTimer(int msLater,int iEventCode,TimerCallBackFunc func = NULL,uint64_t ulParam = 0L);
    void        CancelTimer(uint32_t dwTimerID);
    /////////////////////////////////////////////////
public:
    static void UpdateTick(int , siginfo_t *, void *);
private:
    long   m_lTickUs;
	struct itimerval m_itv;
    struct timeval   m_curTime; 
    uint64_t         m_curMsTime;
    struct timeval   m_lastTime;
    struct sigaction m_old_sa;
    UpdateTickHandler     m_updatTickHandler;
    typedef std::multimap<uint64_t,uint32_t>    TimerMap;
    typedef TimerMap::iterator                  TimerMapItr;
    TimerMap             m_mpTimers;
    typedef     unordered_map<uint32_t,TimerEvent>  TimerIDEventMap;
    typedef     TimerIDEventMap::iterator           TimerIDEventMapItr;
    TimerIDEventMap            m_mpTimerEvents;
    typedef unordered_map<int,TimerCallBackFunc>  EventTimeListenerMap;
    typedef EventTimeListenerMap::iterator        EventTimeListenerMapItr;
    EventTimeListenerMap                          m_mpListeners;   
    uint32_t     dwNextTimerID;
};
//////////////////////////////////////////////////////////////////

