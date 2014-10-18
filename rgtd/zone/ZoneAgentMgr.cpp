#include "AgentApp.h"
#include "ZoneAgentMgr.h"
#include "datacenter/DBAgent.h"
#include "datacenter/CacheAgent.h"
#include "datacenter/ShareMemoryCenter.h"
#include "AccountDBListener.h"
#include "RoleDBListener.h"
#include "proto/gen/db/include.h"

class AgentGateMsgDispatcher : public  ChannelMessageDispatcher
{
public:
    AgentGateMsgDispatcher(ZoneAgentMgr* _mgr):mgr(_mgr){}
    virtual   int DispatchMessage(ChannelAgent & agent , const ChannelMessage & msg)
    {
        //GetID
        int ret = mgr->OnGateMessage(msg);
        LOG_DEBUG("agent gate msg chnnel id = %d ret = %d",agent.GetID(),ret);
        return ret;
    }
private:
    ZoneAgentMgr*   mgr;
};
class AgentServerMsgDispatcher : public  ChannelMessageDispatcher
{
public:
    AgentServerMsgDispatcher(ZoneAgentMgr* _mgr):mgr(_mgr){}
    virtual   int DispatchMessage(ChannelAgent & agent , const ChannelMessage & msg)
    {
        //GetID
        int ret = mgr->OnServerMessage(msg);
        LOG_DEBUG("agent server msg chnnel id = %d ret = %d",agent.GetID(),ret);
        return ret;
    }
private:
    ZoneAgentMgr*   mgr;
};


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

    db::AccountLR   alr;
    db::AccountRL   arl;
    db::Account   acc;
    db::Role      role;

    
    m_chnlProxy = &GetAgentServer().GetChannelProxy();
    m_db = &GetAgentServer().db;
    m_cache = &GetAgentServer().cache;
    //

    m_db->AddListener(string("Role"),DataListenerPtr( new RoleDBListener(this)));
    m_db->AddListener(string("Account"),DataListenerPtr(new AccountDBListener(this,ACCOUNT_DB)));
    m_db->AddListener(string("AccountLR"),DataListenerPtr(new AccountDBListener(this,ACCOUNT_LR)));
    m_db->AddListener(string("AccountRL"),DataListenerPtr(new AccountDBListener(this,ACCOUNT_RL)));
    
    //bind db handler
    //bind cache handler


    //register all shm 

    //from config add zone
    //AddZone(int iZoneID, ZoneAgentPtr agent)


    //init all zone
    //gate msg
    AgentContext * ctx = ( AgentContext *)GetApp()->GetContext();
    vector<int>    ids;
    ids.assign(ctx->gates.begin(),ctx->gates.end());
    m_chnlProxy->SubscribeScatterMsg(ids,ChannelMessageDispatcherPtr(new AgentGateMsgDispatcher(this)));
    
    
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
int             ZoneAgentMgr::OnGateMessage(const ChannelMessage & msg)
{

    
    return 0;
}
int             ZoneAgentMgr::OnServerMessage(const ChannelMessage & msg)
{
    return 0;
}
#endif

