#include "base/Log.h"
#include "ZoneAgentMgr.h"
#include "cs_handler/CSMsgHandler.h"
#include "proto/gen/gate/include.h"
#include "ZoneAgent.h"
#include "SessionMgr.h"


#if 1
SessionMgr::SessionMgr(ZoneAgent* zone):zoneAgent(zone),m_csmsgDispatcher(this)
{
}

int SessionMgr::Init()
{
    login = &(zoneAgent->GetZoneMgr().GetLoginLogic());    
    //////////////////////////////////////////////////
    int ret = m_csmsgDispatcher.SetupAllCSMsgHandler();    
    if(ret)
    {
        LOG_ERROR("setup cs msg handler error !");
        return -1;
    }
    return 0;
}
int SessionMgr::StopSession(int gateid,uint32_t idx,uint64_t uid)
{
    Session *  sson =  FindSession(uid);
    if(!sson)
    {
        LOG_ERROR("can't find session = %lu",uid);
        return -1;
    }
    if(sson->GetIdx() != idx || sson->GetGateID() != gateid)
    {
        LOG_ERROR("session gate id = %d idx = %lu uid = %lu not match !",
                    gateid,idx,uid);
        return -1;
    }
    ///////////////////////////////////////////////////////
    int ret = sson->Kick(Session::KICK_REASON_CLOSE_CLIENT);
    DeleteSession(uid);
    return ret;
}

int SessionMgr::CreateSession(uint64_t uid,int gateid,const gate::GateSSMsg & cnnx)
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
        SessionPtr ptr(new Session(gateid,this));
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
        return login->Login(m_mpSessions[uid]);
    }    
    return ret;
}
int SessionMgr::DeleteSession(uint64_t uid)
{
    LOG_INFO("delete session uid = %lu",uid);
    m_mpSessions.erase(uid);
    return 0;
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
SessionPtr & SessionMgr::GetSession(uint64_t uid)
{
    UIDSessionMapItr it = m_mpSessions.find(uid);
    if(it != m_mpSessions.end())
    {
        return it->second;
    }
    return m_mpSessions[0];
}

int       SessionMgr::PackCSMsg(const cs::CSMsg & csmsg,Buffer & outputBuffer)
{
    int len = csmsg.ByteSize();
    if(len <= 0 || len >= ChannelMsgProxy::MAX_CHANNEL_MESSAGE_SIZE)
    {
        LOG_ERROR("get csmsg byte size = %d error !",len);
        return -1;
    }
    if( m_msgPackBuffer.iCap < len )
    {
        m_msgPackBuffer.Destroy();
        m_msgPackBuffer.Create(len);
    }
    if(!csmsg.SerializeToArray(m_msgPackBuffer.pBuffer,len))
    {
        LOG_ERROR("cs msg pack error !");
        return -1;
    }    
    m_msgPackBuffer.iUsed = len;
    outputBuffer = m_msgPackBuffer;
    return 0;
}

#endif

