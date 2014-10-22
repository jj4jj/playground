
#include "base/Log.h"
#include "AgentApp.h"
#include "component/TimerMgr.h"

#include "EventCenter.h"


#if 1
void    EventCenter::OnTimer(int iEvent,uint64_t ulParam)
{
    EventCenter::Instance().OnDelayTimerExpired(iEvent,ulParam);
}
#endif



#if 1
int EventCenter::Init()
{
    zoneMgr = &((AgentApp*)GetApp())->zoneMgr;
    timerMgr = TimerMgr::instance();
    return 0;
}
int     EventCenter::OnDelayTimerExpired(int iEventCode,uint64_t )
{
    if(m_mpDelayEventParams.find(iEventCode) == m_mpDelayEventParams.end())
    {
        LOG_ERROR("delay event code = %d is missing !",iEventCode);
        return -1;
    }
    DelayEventParam dep = m_mpDelayEventParams[iEventCode];
    m_mpDelayEventParams.erase(iEventCode);
    return FireEvent((EventCodeEnum)iEventCode,dep.reciever.c_str(),0,dep.ulParam,dep.arg);
}
//if name is null , broad cast , seconds is 0 then deliver right now , ...
//return timerid if delay
uint32_t     EventCenter::FireEvent(EventCodeEnum iEventCode,const char* pszReceiverName ,int secondsLater ,uint64_t ulParam ,void* arg )
{
    int ret = 0;
    if( iEventCode > EVENT_TEST_MIN && iEventCode < EVENT_TEST_MAX)
    {
        //test
        return 0;
    }
    else if( iEventCode > EVENT_PLAYER_MIN && iEventCode < EVENT_PLAYER_MAX)
    {
        if(secondsLater > MAX_PLAYER_EVENT_DELAY_TIME)
        {
            LOG_ERROR("invalid event = %d seconds later = %d",iEventCode,secondsLater);
            return (uint32_t)-1;
        }
        //must be online palyer when a delay event comes
        PlayerAgentPtr player = zoneMgr->GetZonePlayerAgentPtr(ulParam);
        if(!player && secondsLater > 0)
        {
            LOG_ERROR("player is not online event = %d delay = %d",iEventCode,secondsLater);
            return (uint32_t)-1;
        }
        //if delay event expired  , seconds is 0  if 
        if(m_mpPlayerAgents.find(ulParam) == m_mpPlayerAgents.end() &&
           !player )
        {
            LOG_ERROR("player = %lu not found event = %d ",ulParam,iEventCode);
            return (uint32_t)-1;
        }

        //get valid player agent
        if(!player)
        {
            player = m_mpPlayerAgents[ulParam];
        }
                
        //there must has no player agent 
        if(secondsLater > 0 )
        {
            m_mpPlayerAgents[ulParam] = player;
            //store
            DelayEventParam delayParam;
            delayParam.ulParam = ulParam;
            delayParam.arg = arg;
            if(pszReceiverName)
            {
                delayParam.reciever = pszReceiverName;
            }
            if(m_mpDelayEventParams.find(iEventCode) != m_mpDelayEventParams.end())
            {
                LOG_WARN("delay event code = %d is already event queue . cover it .",iEventCode);
            }
            //timer
            uint32_t dwTimerID = timerMgr->AddTimer(secondsLater*1000,iEventCode);
            m_mpDelayEventParams[iEventCode] = delayParam;\
            return dwTimerID;
        }
        else
        {
            //right now 
            player->GetLogicCenter().NotifyEvent(iEventCode,ulParam,pszReceiverName,arg);
        }
    }
    else if( iEventCode > EVENT_AREA_MIN && iEventCode < EVENT_AREA_MAX)
    {

    }
    else if( iEventCode > EVENT_ZONE_MIN && iEventCode < EVENT_ZONE_MAX)
    {

    }
    else
    {
        LOG_ERROR("not support event = %d",iEventCode);
        return (uint32_t)-1;
    }
    if(ret)
    {
        LOG_ERROR("fire event = %d later = %d param = %lu ret = %d",
                    iEventCode,secondsLater,ulParam,ret);
        return (uint32_t)-1;
    }
    return 0;
}
#endif



