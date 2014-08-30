#pragma once


struct ITimer
{
    virtual void OnExpired(int iID,int iPastMs,const char* pszName );
    virtual void OnCancel(int  iID,int iPastMs,const char* pszName );
    
};

//driver of a app
class TimerMgr
{
    //return < 0 error
    //return >= 0 is ok , and it's timer id .
    int AddTimer(int type,int param,ITimer * pITimer,const char* pszTimerName = NULL);
    int CancelTimer(int iID,const char* pszTimerName = NULL);
    
};























