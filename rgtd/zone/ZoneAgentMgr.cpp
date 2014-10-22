#include "AgentApp.h"
#include "ZoneAgentMgr.h"
#include "datacenter/DBAgent.h"
#include "datacenter/CacheAgent.h"
#include "datacenter/ShareMemoryCenter.h"
#include "account/AccountDBListener.h"
#include "account/RoleDBListener.h"
#include "proto/gen/db/include.h"
#include "proto/gen/cs/include.h"
#include "proto/gen/gate/include.h"
#include "account/PlayerAgent.h"

class AgentGateMsgDispatcher : public  ChannelMessageDispatcher
{
public:
    AgentGateMsgDispatcher(ZoneAgentMgr* _mgr):mgr(_mgr){}
    virtual   int DispatchMessage(ChannelAgent & agent , const ChannelMessage & msg)
    {
        //GetID
        int ret = mgr->OnGateMessage(msg);
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
ZoneAgentMgr::ZoneAgentMgr():dbProxy(this),cacheProxy(this)
{


}


#endif


#if 1
int             ZoneAgentMgr::AddZone(uint32_t dwZoneID,ZoneAgentPtr agent)
{
    if(m_mpZoneAgent.find(dwZoneID) != m_mpZoneAgent.end())
    {        
        LOG_FATAL("repeat add zone area = %u",dwZoneID);
        return -1;
    }
    m_mpZoneAgent[dwZoneID] = agent;
    return 0;
}
ZoneAgent *     ZoneAgentMgr::GetAgent(uint32_t dwZoneID)
{
    if(m_mpZoneAgent.find(dwZoneID) != m_mpZoneAgent.end())
    {        
        return m_mpZoneAgent[dwZoneID].get();
    }
    return NULL;
}
int             ZoneAgentMgr::Init()
{            
    AgentContext * contxt = (AgentContext *)GetApp()->GetContext();


    int ret = login.Init(this);
    if(ret)
    {
        LOG_ERROR("login init error !");
        return -1;
    }
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

    vector<uint32_t>    zone_areas;
    zone_areas.assign(contxt->areas.begin(),contxt->areas.end());
    for(uint i = 0 ;i < zone_areas.size(); ++i)
    {
        //default
        ZoneAgentPtr zoneAgentPtr(new ZoneAgent(zone_areas[i],this));
        if(AddZone(zone_areas[i], zoneAgentPtr))
        {
            LOG_ERROR("zone agent id = %u add error !",zone_areas[i]);
            return -1;
        }
        if(zoneAgentPtr->Init())
        {
            LOG_ERROR("zone agent id = %u init error !",zone_areas[i]);
            return -1;
        }        
    }


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
    //csmsg
    gate::GateConnection    ggc;
    uint16_t ggclen =  ntohs(*(uint16_t*)msg.pData);
    if(msg.dwSize < (uint32_t)ggclen +  ChannelMsgProxy::PROXY_CHANNEL_MESSAGE_HEAD_SIZE)
    {
        LOG_FATAL("ggc len = %d all size = %d src = %d error !",ggclen,msg.dwSize,msg.iSrc);
        return -1;
    }
    if(!ggc.ParseFromArray(msg.pData + ChannelMsgProxy::PROXY_CHANNEL_MESSAGE_HEAD_SIZE,ggclen))
    {
        LOG_ERROR("unpack gate msg error !");
        return -1;
    }

    ZoneAgent * pAgent = GetAgent(ggc.area());
    if(!pAgent)
    {
        LOG_ERROR("get agent error area = %u",ggc.area());
        return -1;
    }
    
    int event = ggc.event();
    int csmsglen = msg.dwSize - ggclen - ChannelMsgProxy::PROXY_CHANNEL_MESSAGE_HEAD_SIZE;
    char * pcsmsg = (char*)msg.pData +  ChannelMsgProxy::PROXY_CHANNEL_MESSAGE_HEAD_SIZE + ggclen;
    switch(event)
    {
        case gate::GateConnection::EVENT_CONNECTED:
        {
            //get uid area
            LOG_INFO("client connected ip = [%u] port = [%d] uid [%lu] src = [%d] idx = [%u]",
                ggc.ip(),ggc.port(),ggc.uid(),msg.iSrc,ggc.idx());
            ////////////////////////////////////////////////////////////////////////
            //area new client
            return  pAgent->AttachPlayerAgent(msg.iSrc,ggc);            
            break;
        }
        case gate::GateConnection::EVENT_CLOSE:
        {

            //get uid area
            LOG_INFO("client disconnected  uid [%lu] src = [%d] idx = [%u]",
                ggc.uid(),msg.iSrc,ggc.idx());
            
            ////////////////////////////////////////////////////////////////////////
            //area client close
            return pAgent->DetachPlayerAgent(ggc);
            break;
        }
        case gate::GateConnection::EVENT_DATA:
        {
            //get uid area
            LOG_INFO("client input data size = %u uid [%lu] src = [%d] idx = [%u]",
                csmsglen,ggc.uid(),msg.iSrc,ggc.idx());
            //data msg unpack
            cs::CSMsg   csmsg;
            if(!csmsg.ParseFromArray(pcsmsg,csmsglen))
            {
                LOG_ERROR("unpack client msg error !");
                return -1;
            }
            return  pAgent->DispatchPlayerAgentMsg(ggc,csmsg);                        
        }
            ////////////////////////////////////////////////////////////////////////
            break;       
        default:
            LOG_ERROR("gate connection unknown event = %d",event);
            return -1;
    }        
    return 0;
}
int             ZoneAgentMgr::OnServerMessage(const ChannelMessage & msg)
{
    LOG_DEBUG("todo");
    return 0;
}
PlayerAgentPtr  ZoneAgentMgr::GetZonePlayerAgentPtr(uint64_t uid)
{
    //get zone mgr
    uint16_t area = GetAreaFromRoleID(uid);
    ZoneAgent* agent = GetZoneAgentByArea(area);
    if(agent)
    {
        Session * sson = agent->GetSessionMgr().FindSession(uid);
        if(sson)
        {
            return sson->GetPlayerAgentPtr();
        }
    }
    return PlayerAgentPtr();
}
uint64_t    ZoneAgentMgr::GetRoleID(uint64_t uid,uint32_t area)
{
    ///area[16]uid[32]
    uint64_t rid = (area&0xFFFF);
    rid <<= 32;
    rid |= (uid&0xFFFFFFFF);
    return rid;
}
uint32_t    ZoneAgentMgr::GetUidFromRoleID(uint64_t rid)
{
    return rid&(0xFFFFFFFF);
}
uint16_t    ZoneAgentMgr::GetAreaFromRoleID(uint64_t rid)
{
    return (rid>>32)&0xFFFF;
}
uint16_t    ZoneAgentMgr::GetZoneIDFromArea(uint32_t area)
{
    //now area is zone
    //when merge zone , it will find a map
    //todo
    return (uint16_t)area;
}
ZoneAgent *        ZoneAgentMgr::GetZoneAgentByArea(uint32_t area)
{
    return GetAgent(GetZoneIDFromArea(area));
}

#endif

