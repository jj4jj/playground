
#include "base/Log.h"
#include "AgentApp.h"
#include "component/TimerMgr.h"
#include "logic/PlayerLogicCenter.h"

#include "EventCenter.h"


#if 1
void    EventCenter::OnTimer(int iEvent,uint64_t ulParam)
{
    int iDelayEventSeq = (int)ulParam;
    LOG_DEBUG("OnTimer event = %d timer param = %lu",iEvent,ulParam);
    EventCenter::Instance().OnDelayTimerExpired(iEvent,iDelayEventSeq);
}
#endif



#if 1
int EventCenter::Init()
{
    zoneMgr = &((AgentApp*)GetApp())->zoneMgr;
    timerMgr = TimerMgr::instance();
    m_iDelayEventNextSeq = 1;
    return 0;
}
int     EventCenter::OnDelayTimerExpired(int iEventCode,int iDelayEventSeq)
{
    LOG_DEBUG("on delay fire event = %d timer param = %d",iEventCode,iDelayEventSeq);
    if(m_mpDelayEventParams.find(iDelayEventSeq) == m_mpDelayEventParams.end())
    {
        LOG_ERROR("delay event code = %d seq = %d is missing !",iDelayEventSeq);
        return -1;
    }
    DelayEventParam dep = m_mpDelayEventParams[iEventCode];
    m_mpDelayEventParams.erase(iEventCode);
    return FireEvent((EventCodeEnum)iEventCode,dep.reciever.c_str(),0,dep.ulParam,dep.arg,true);
}
uint32_t    EventCenter::GenerateDelayEvent(EventCodeEnum iEventCode,
                            const char* pszReceiverName ,int secondsLater ,
                            uint64_t ulParam ,void* arg )
{
    //store
    DelayEventParam delayParam;
    delayParam.iEvent = iEventCode;
    delayParam.ulParam = ulParam;
    delayParam.arg = arg;
    if(pszReceiverName)
    {
        delayParam.reciever = pszReceiverName;
    }
    while(m_mpDelayEventParams.find(m_iDelayEventNextSeq) != m_mpDelayEventParams.end())
    {
        LOG_WARN("delay event code = %d seq = %d is already event queue . cover it .",m_iDelayEventNextSeq);
        ++m_iDelayEventNextSeq;
    }
    //timer
    uint32_t dwTimerID = timerMgr->AddTimer(secondsLater*1000,iEventCode,EventCenter::OnTimer,m_iDelayEventNextSeq);
    m_mpDelayEventParams[m_iDelayEventNextSeq] = delayParam;
    ++m_iDelayEventNextSeq;
    return dwTimerID;
}
//if name is null , broad cast , seconds is 0 then deliver right now , ...
//return timerid if delay
uint32_t     EventCenter::FireEvent(EventCodeEnum iEventCode,
        const char* pszReceiverName ,int secondsLater ,
        uint64_t ulParam ,void* arg,bool timeExpired )
{
    LOG_DEBUG("fire event = %d delay = %d param = %lu",iEventCode,secondsLater,ulParam);
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
        //offline
        if(!player )
        {
            if(secondsLater > 0)
            {
                LOG_ERROR("player is not online event = %d delay = %d",iEventCode,secondsLater);
                return (uint32_t)-1;
            }
            else
            {
                //must have a cache (doing)
                if(m_mpPlayerAgents.find(ulParam) == m_mpPlayerAgents.end())
                {
                    LOG_ERROR("player is not online adn also no player cache event = %d delay = %d",iEventCode,secondsLater);
                    return (uint32_t)-1;
                }
                else
                {
                    player = m_mpPlayerAgents[ulParam];
                }                
            }
        }
        
        if(secondsLater > 0)
        {
            //cache player data
            m_mpPlayerAgents[ulParam] = player;
        }
        else
        {
            //right now 
            player->GetLogicCenter().NotifyEvent(iEventCode,ulParam,pszReceiverName,arg);
            if(timeExpired)
            {
                LOG_DEBUG("erase uid = %lu cache agent !",ulParam);
                m_mpPlayerAgents.erase(ulParam);
            }
        }
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
    //there must has no player agent 
    uint32_t dwTimerID = 0;
    if(secondsLater > 0 )
    {            
        dwTimerID = GenerateDelayEvent(iEventCode, pszReceiverName,secondsLater, ulParam, arg);
    }
    return dwTimerID;
}
#endif



