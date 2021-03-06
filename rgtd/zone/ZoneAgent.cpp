
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
#endif

////////////////////////////
#if 1
int ZoneAgent::DispatchPlayerAgentMsg(const gate::GateSSMsg & ggc,const cs::CSMsg & csmsg)
{   
    uint64_t uid = ZoneAgentMgr::GetRoleID(ggc.uid(),ggc.area());
    Session * session = m_sessionMgr.FindSession(uid);
    if(!session)
    {
        LOG_ERROR("not found session uid = %lu",uid);
        return -1;
    }
    return m_sessionMgr.GetDispatcher().Dispatch(*session,csmsg);
}
int ZoneAgent::AttachPlayerAgent(int gateid,const gate::GateSSMsg & ggc)
{
    uint64_t uid = ZoneAgentMgr::GetRoleID(ggc.uid(),ggc.area());
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
int ZoneAgent::DetachPlayerAgent(int gateid,const gate::GateSSMsg & ggc)
{
    uint64_t uid = ZoneAgentMgr::GetRoleID(ggc.uid(),ggc.area());
    return m_sessionMgr.StopSession(gateid,ggc.idx(),uid);
}
#endif
