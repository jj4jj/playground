#pragma once
#include "base/stdinc.h"
#include "base/Singleton.hpp"


typedef void (* UpdateTickHandler)(struct timeval & now , int64_t  lElapseTime);
//driver of a app
class TimerMgr : public Singleton<TimerMgr>
{
public:
    TimerMgr();
    ~TimerMgr();
public:
    int     Init(long lTickUs,UpdateTickHandler handler = NULL);
    void    Destroy();
    inline  const timeval & GetCurTime(){return m_curTime;}
    /////////////////////////////////////////////////
public:
    static void UpdateTick(int , siginfo_t *, void *);
private:
    long   m_lTickUs;
	struct itimerval m_itv;
    struct timeval   m_curTime; 
    struct timeval   m_lastTime;
    struct sigaction m_old_sa;
    UpdateTickHandler     m_updatTickHandler;
};



