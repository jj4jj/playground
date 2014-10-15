#include "AgentApp.h"
#include "ZoneAgentMgr.h"
#include "datacenter/DBAgent.h"
#include "datacenter/CacheAgent.h"
#include "datacenter/ShareMemoryCenter.h"
#include "AccountDBListener.h"
#include "RoleDBListener.h"

#if 1
int             ZoneAgentMgr::AddZone(int iZoneID,ZoneAgentPtr agent)
{
    return 0;
}
ZoneAgent &     ZoneAgentMgr::GetAgent(int iZoneID)
{
    return *m_mpZoneAgent[iZoneID].get();
}
int             ZoneAgentMgr::Init()
{    
    m_chnlProxy = &GetAgentServer().GetChannelProxy();
    m_db = &GetAgentServer().db;
    m_cache = &GetAgentServer().cache;
    //

    m_db->AddListener(string("Role"),DataListenerPtr( new RoleDBListener(this)));
    m_db->AddListener(string("AccountR"),DataListenerPtr(new AccountDBListener(this,ACCOUNT_R)));
    m_db->AddListener(string("AccountL"),DataListenerPtr(new AccountDBListener(this,ACCOUNT_L)));
    
    //bind db handler
    //bind cache handler


    //register all shm 

    //from config add zone
    //AddZone(int iZoneID, ZoneAgentPtr agent)


    //init all zone

    
    return 0;
}
int             ZoneAgentMgr::Polling(int iPollTimeOutMs)
{
    m_chnlProxy->Polling(iPollTimeOutMs);
    
    return 0;
}
int             ZoneAgentMgr::Destroy()
{
    return 0;
}
//
int             ZoneAgentMgr::OnGateMessage()
{
    return 0;
}
int             ZoneAgentMgr::OnServerMessage()
{
    return 0;
}
#endif

