
#include "proto/gen/gate/include.h"
#include "ZoneAgentMgr.h"
#include "ZoneAgent.h"

#if 1
ZoneAgent::ZoneAgent(uint32_t id,ZoneAgentMgr* mgr):m_dwZoneID(id),m_mgr(mgr)
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
    return 0;
}
void ZoneAgent::Destroy()
{

    m_dwZoneID = 0;
    m_mgr = NULL;

    
    return 0;

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
int ZoneAgent::AttachPlayerAgent(const gate::GateConnection & ggc)
{
    return 0;

}
int ZoneAgent::DetachPlayerAgent(const gate::GateConnection & ggc)
{
    return 0;

}
#endif
