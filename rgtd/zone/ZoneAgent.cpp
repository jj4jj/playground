
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
int ZoneAgent::DispatchPlayerAgentMsg(const cs::CSMsg & csmsg)
{   

    //csmsg.DebugString();
    
    return 0;
}
int ZoneAgent::AttachPlayerAgent(int gateid,const gate::GateConnection & ggc)
{
    int ret = m_sessionMgr.CreateSession(gateid, ggc);
    if(ret != 0)
    {
        LOG_ERROR("attach session error = %u uid = %lu",ret,ggc.uid());
        return -1;
    }    
    return 0;
}
int ZoneAgent::DetachPlayerAgent(const gate::GateConnection & ggc)
{
    return 0;

}
#endif
