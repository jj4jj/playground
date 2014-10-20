#include "base/Log.h"
#include "ZoneAgentMgr.h"
#include "cs_handler/CSMsgHandler.h"
#include "proto/gen/gate/include.h"
#include "ZoneAgent.h"


#include "SessionMgr.h"


#if 1
SessionMgr::SessionMgr(ZoneAgent* zone):zoneAgent(zone)
{
}
int SessionMgr::Init()
{
    login = &(zoneAgent->GetZoneMgr().GetLoginLogic());
    return 0;
}
int SessionMgr::CreateSession(int gateid,const gate::GateConnection & cnnx)
{    
    Session *  sson =  FindSession(cnnx.uid());
    int ret = 0;
    if(sson)
    {
        //old session
        LOG_INFO("kick old session  = %u !",cnnx.uid());
        ////////////////////////////////////////////////
        sson->Kick(Session::KICK_REASON_RELOGIN);
        sson->Init(cnnx);
    }
    else
    {
        LOG_INFO("create new  session  = %u !",cnnx.uid());
        SessionPtr ptr(new Session(gateid));
        if(!ptr)
        {
            LOG_ERROR("create session = %u error !",cnnx.uid());
            return -1;
        }
        ret = ptr->Init(cnnx);
        if(0 == ret)
        {
            m_mpSessions[cnnx.uid()] = ptr;
            sson = ptr.get();
        }
    }
    if(sson)
    {
        //Login
        return login->LoadPlayer(*sson);
    }    
    return ret;
}
int SessionMgr::DeleteSession()
{
    //todo
    return 0;
}

Session * SessionMgr::FindSession(uint64_t    ulUID)
{
    //todo
    return NULL;
}

CSMsgHandler*   SessionMgr::GetMsgHandler(uint32_t cmd)
{
    //todo
    return NULL;
}
int       SessionMgr::Dispatch(Session & session,const cs::CSMsg & csmsg)
{
    uint32_t    cmd = csmsg.cmd();
    CSMsgHandler * pHandler = GetMsgHandler(cmd);
    if(pHandler)
    {
        //create a coroutine ?
        //
        return pHandler->Handle(session,csmsg);
    }
    else
    {
        LOG_ERROR("known cs msg cmd = %u",cmd);
        return -100; 
    }
}
#endif

