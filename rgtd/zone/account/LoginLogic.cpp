#include "ZoneAgentMgr.h"
#include "Session.h"
#include "LoginLogic.h"
#include "RoleDBListener.h"

int LoginLogic::Init(ZoneAgentMgr * mgr)
{
    zoneMgr = mgr;   

    return 0;
}
int     LoginLogic::OnLoadPlayer(int ret,uint64_t uid,db::Role * role)
{

    LOG_INFO("on load ret = %d uid = %lu",ret,uid);
    return 0;
}

int     LoginLogic::LoadPlayer(Session & session)
{
    vector < string > all;
    RoleDBCallBack  rcb;
    rcb.uid = session.GetUID();
    rcb.type = ROLE_DB_GET_LOGIN;
    db::Role   role;
    role.set_rid(rcb.uid);
    int ret = zoneMgr->GetDBAgent().Get(&role,all,Buffer((char*)&rcb,sizeof(rcb)));
    if(ret)
    {
        LOG_ERROR("db agent get error !");
        return -1;    
    }
    session.SetState(Session::STATE_LOADING);
    return 0;
}
int     LoginLogic::NotifyCreateRole(Session & session)
{
    //todo
    return -1;

}
int     LoginLogic::AutoCreateRole(Session & session)
{
    
    //todo
    return -1;

}
int     LoginLogic::EnterPlaying(Session & session)
{
    //todo
    return -1;

}

