
#include "proto/gen/gate/include.h"
#include "ZoneAgentMgr.h"
#include "ZoneAgent.h"

#if 1
ZoneAgent::ZoneAgent(uint32_t id,ZoneAgentMgr * mgr):m_dwZoneID(id),zoneMgr(mgr),m_sessionMgr(this)
{

}
ZoneAgent::~ZoneAgent()
{
    Destroy();
}


#endif


#if 1
int ZoneAgent::Init()
{
    int ret = 0;

    ret = m_sessionMgr.Init();
    
    return ret;
}
void ZoneAgent::Destroy()
{

    m_dwZoneID = 0;
    zoneMgr = NULL;

}
int ZoneAgent::OnServerMessage()
{
    return 0;
}
uint64_t    ZoneAgent::GetRoleID(uint64_t uid,uint32_t area)
{
    ///area[16]uid[32]
    uint64_t rid = (area&0xFFFF);
    rid <<= 32;
    rid |= (uid&0xFFFFFFFF);
    return rid;
}

#endif

////////////////////////////
#if 1
int ZoneAgent::DispatchPlayerAgentMsg(const gate::GateConnection & ggc,const cs::CSMsg & csmsg)
{   
    uint64_t uid = GetRoleID(ggc.uid(),ggc.area());
    Session * session = m_sessionMgr.FindSession(uid);
    if(!session)
    {
        LOG_ERROR("not found session uid = %lu",uid);
        return -1;
    }
    return m_sessionMgr.Dispatch(*session,csmsg);
}
int ZoneAgent::AttachPlayerAgent(int gateid,const gate::GateConnection & ggc)
{
    uint64_t uid = GetRoleID(ggc.uid(),ggc.area());
    int ret = m_sessionMgr.CreateSession(uid,gateid, ggc);
    if(ret != 0)
    {
        LOG_ERROR("attach session error = %u uid = %lu",ret,uid);
        return -1;
    }    
    LOG_INFO("create new session ok uid = %lu gate = %u idx = %u area = %u",
            uid,gateid,ggc.idx(),ggc.area());
    return 0;
}
int ZoneAgent::DetachPlayerAgent(const gate::GateConnection & ggc)
{
    uint64_t uid = GetRoleID(ggc.uid(),ggc.area());
    return m_sessionMgr.StopSession(uid);
}
#endif
