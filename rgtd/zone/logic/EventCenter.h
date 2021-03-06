#pragma once
#include "base/stdinc.h"
#include "base/Singleton.hpp"

enum    EventCodeEnum
{
    EVENT_TEST_MIN  =   1000,
    EVENT_TEST_TEST,
    EVENT_TEST_MAX ,
    EVENT_TEST_SIZE = EVENT_TEST_MAX - EVENT_TEST_MIN,
    /////////////////////////////////////////////////////////
    EVENT_PLAYER_MIN  =   2000,
    EVENT_PLAYER_TEST,
    EVENT_PLAYER_SYNC_DB,
    EVENT_PLAYER_TASK_DONE,
    EVENT_PLAYER_ITEM_ADD,
    EVENT_PLAYER_ITEM_DEC,
    EVENT_PLAYER_REMOVE_GRID_ITEM,
    EVENT_PLAYER_CHANGE_PACK_MAX_GRID,
    
    EVENT_PLAYER_MAX ,
    EVENT_PLAYER_SIZE  = EVENT_PLAYER_MAX - EVENT_PLAYER_MIN,
    /////////////////////////////////////////////////////////
    EVENT_AREA_MIN  =   3000,
    EVENT_AREA_TEST,    
    EVENT_AREA_MAX ,
    EVENT_AREA_SIZE  = EVENT_PLAYER_MAX - EVENT_PLAYER_MIN,
    /////////////////////////////////////////////////////////
    EVENT_ZONE_MIN  =   4000,
    EVENT_ZONE_TEST,
    
    EVENT_ZONE_MAX ,
    EVENT_ZONE_SIZE  = EVENT_PLAYER_MAX - EVENT_PLAYER_MIN,
    /////////////////////////////////////////////////////////

};

struct  DelayEventParam
{
    int         iEvent;
    uint64_t    ulParam;
    string      reciever;
    void *      arg;    
    DelayEventParam()
    {
        iEvent = 0;
        reciever = "";
        ulParam = 0L;
        arg = NULL;
    }
};


class ZoneAgentMgr;
class TimerMgr;
class PlayerAgent;
typedef shared_ptr<PlayerAgent>     PlayerAgentPtr;


class EventCenter : public Singleton<EventCenter> 
{
public:
    enum
    {
        //max player event delay time (10min)
        MAX_PLAYER_EVENT_DELAY_TIME =   600,
    };
private:
    DeclareDefaultPrivateConstructor(EventCenter)
    DeclareSingltonSupport(EventCenter)
public:
    int     Init();
    //if name is null , broad cast , seconds is 0 then deliver right now , ...
    //return timerid if delay
    uint32_t     FireEvent(EventCodeEnum iEventCode,
                    const char* pszReceiverName = NULL,int secondsLater = 0,
                    uint64_t ulParam = 0,void* arg = NULL,bool timeExpired = false);
public:
    static  void    OnTimer(int iEvent,uint64_t ulParam);
protected:
    uint32_t    GenerateDelayEvent(EventCodeEnum iEventCode,
                            const char* pszReceiverName ,int secondsLater ,
                            uint64_t ulParam ,void* arg );
    int          OnDelayTimerExpired(int iEventCode,int iDelayEventSeq );
    
private:
    //listener name - code -
    ZoneAgentMgr    *   zoneMgr;
    TimerMgr        *   timerMgr;
    //for timer events
    unordered_map<uint64_t,PlayerAgentPtr>           m_mpPlayerAgents;
    typedef     unordered_map<int,DelayEventParam>   DelayEventMap;
    typedef     DelayEventMap::iterator              DelayEventMapItr;
    int                    m_iDelayEventNextSeq;
    DelayEventMap          m_mpDelayEventParams;
    //global timer 
    //session mgr
};
typedef     shared_ptr<EventCenter>     EventCenterPtr;

