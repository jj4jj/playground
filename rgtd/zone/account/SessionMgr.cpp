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
int SessionMgr::RegisterCSMsgHandler(uint32_t cmd,CSMsgHandlerPtr handler)
{
    if(m_mpHandlers.find(cmd) == m_mpHandlers.end())
    {
        LOG_ERROR("register handler repeatly ! cmd = %u",cmd);
        return -1;
    }
    m_mpHandlers[cmd] = handler;
    return 0;
}

int SessionMgr::Init()
{
    login = &(zoneAgent->GetZoneMgr().GetLoginLogic());    
    return 0;
}
int SessionMgr::StopSession(uint64_t uid)
{
    Session *  sson =  FindSession(uid);
    if(!sson)
    {
        LOG_ERROR("can't find session = %lu",uid);
        return -1;
    }
    ////////////////////////////////////////////////////
    //todo check reason
    return    sson->Kick(Session::KICK_REASON_CLOSE_CLIENT);
}

int SessionMgr::CreateSession(uint64_t uid,int gateid,const gate::GateConnection & cnnx)
{    
    Session *  sson =  FindSession(uid);
    int ret = 0;
    if(sson)
    {
        //old session
        LOG_INFO("kick old session  = %u !",uid);
        ////////////////////////////////////////////////
        sson->Kick(Session::KICK_REASON_RELOGIN);
        sson->Init(uid,cnnx);
    }
    else
    {
        LOG_INFO("create new  session  = %lu !",uid);
        SessionPtr ptr(new Session(gateid));
        if(!ptr)
        {
            LOG_ERROR("create session = %lu error !",uid);
            return -1;
        }
        ret = ptr->Init(uid,cnnx);
        if(0 == ret)
        {
            m_mpSessions[uid] = ptr;
            sson = ptr.get();
        }
    }
    if(sson)
    {
        //Login
        return login->Login(*sson);
    }    
    return ret;
}
int SessionMgr::DeleteSession()
{
    //todo
    return 0;
}
void    CheckInvalidSession()
{
    //todo
}

Session * SessionMgr::FindSession(uint64_t    ulUID)
{
    UIDSessionMapItr it = m_mpSessions.find(ulUID);
    if(it != m_mpSessions.end())
    {
        return it->second.get();
    }
    return NULL;
}

CSMsgHandler *  SessionMgr::GetMsgHandler(uint32_t cmd)
{
    CMDHandlerMapItr it = m_mpHandlers.find(cmd);
    if(it != m_mpHandlers.end())
    {
        return it->second.get();
    }
    return NULL;
}
int       SessionMgr::Dispatch(Session & session,const cs::CSMsg & csmsg)
{
    uint32_t    cmd = csmsg.cmd();
    CSMsgHandler * pHandler = GetMsgHandler(cmd);
    if(pHandler)
    {
        return pHandler->Handle(session,csmsg);
    }
    else
    {
        LOG_ERROR("known cs msg cmd = %u",cmd);
        return -100; 
    }
}
#endif

