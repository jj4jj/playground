
#include "base/Log.h"
#include "account/SessionMgr.h"
////////////////////////////////////////////////////////////////////////////////////
#include "cs_handler/CSMsgHandler.h"
#include "cs_handler/CSRoleHandler.h"


/////////////////////////////////////////////////////////////////////////////////////
#include "CSMsgDispatcher.h"


int     CSMsgDispatcher::RegisterCSMsgHandler(uint32_t cmd,CSMsgHandlerPtr handler)
{
    if(m_mpHandlers.find(cmd) != m_mpHandlers.end())
    {
        LOG_ERROR("register handler repeatly ! cmd = %u",cmd);
        return -1;
    }
    m_mpHandlers[cmd] = handler;
    return 0;
}
CSMsgHandler *  CSMsgDispatcher::GetMsgHandler(uint32_t cmd)
{
    CMDHandlerMapItr it = m_mpHandlers.find(cmd);
    if(it != m_mpHandlers.end())
    {
        return it->second.get();
    }
    return NULL;
}
int       CSMsgDispatcher::Dispatch(Session & session,const cs::CSMsg & csmsg)
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
///////////////////////////////////////////////////////////////////////////////
void    CSMsgDispatcher::SetupAllCSMsgHandler()
{
    RegisterCSMsgHandler(cs::CSMsg::CSMSG_CMD_ROLE,CSMsgHandlerPtr(new CSRoleHandler()));
    //todo add handler here
    
}



