#include "ZoneAgentMgr.h"
#include "Session.h"
#include "LoginLogic.h"
#include "utility/Coroutine.h"
#include "DBProxy.h"

int LoginLogic::Init(ZoneAgentMgr * mgr)
{
    zoneMgr = mgr;   

    return 0;
}
struct LoginParam
{
    LoginLogic * login;
    Session    * session;
};
static void	Login(Coroutine* co,void* arg)
{
    LoginParam *  param = (LoginParam*)arg;
    LoginLogic * login =   param->login;
    Session *    session = param->session;
    //////////////////////////////////////
    session->SetState(Session::PLAYER_STATE_LOADING);
    int ret = login->zoneMgr->GetDBProxy().GetRole(session->GetUID(),
        ROLE_DB_GET_LOGIN,co->iID);

    LOG_INFO("get role ret = %d uid = %lu error",ret,session->GetUID());

    if( ret < 0 )
    {
        return ;   
    }
    if(ret == DATA_OK)
    {
        //attach player agent
        db::Role* pRole = login->zoneMgr->GetDBProxy().FindRoleObject(session->GetUID());
        if(!pRole)
        {
            LOG_ERROR("get role object error !");
            return ;
        }
        //create player agent
        PlayerAgentPtr ptr(new PlayerAgent(pRole,true));
        session->AttachPlayerAgent(ptr);        
        session->SetState(Session::PLAYER_STATE_PLAYING);
        login->EnterPlaying(*session);
        return;
    }
    else if(ret == DATA_NO_EXISTS)
    {
        session->SetState(Session::PLAYER_STATE_CREATING);
        if(login->NotifyCreateRole(*session))
        {
            LOG_ERROR("notify create role uid = %lu error !",session->GetUID());
            return ;
        }
        //inserting
        ret = login->zoneMgr->GetDBProxy().InsertRole(session->GetUID(),session->GetPlayerAgent()->GetRoleData(),co->iID);
        if(ret == DATA_OK)
        {
            session->SetState(Session::PLAYER_STATE_PLAYING);
            login->EnterPlaying(*session);
        }
        else
        {
            LOG_ERROR("insert player = %lu error ret = %d",ret);
            //invalid session
            session->Kick(Session::KICK_REASON_DB_ERR);
            return ;
        }
        return;
    }
    else
    {
        LOG_ERROR("get role db ret = %d",ret);
        return ;
    }    
}
int     LoginLogic::Login(Session & session)
{
    LoginParam lp = {this,&session};
    int coid = CoroutineMgr::Instance().Create(::Login,&lp);
    return CoroutineMgr::Instance().Resume(coid);
}
int     LoginLogic::NotifyCreateRole(Session & session)
{
    //todo    
    //auto
    LOG_INFO("todo notify client to create role !");
    return AutoCreateRole(session);
}
int     LoginLogic::AutoCreateRole(Session & session)
{    
    //todo
    //inserting
    db::Role * pRole = new db::Role();    
    pRole->set_rid(session.GetUID());
    LOG_INFO("todo init role = %lu data !",session.GetUID());
    session.AttachPlayerAgent(PlayerAgentPtr(new PlayerAgent(pRole,false)));    
    return 0;
}
int     LoginLogic::EnterPlaying(Session & session)
{
    //
    LOG_INFO("player = %lu  login sucess !");
    return -1;
}

