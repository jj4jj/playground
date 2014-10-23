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
    SessionPtr   session;
};
static void	Login(Coroutine* co,void* arg)
{
    LoginParam *  param = (LoginParam*)arg;
    LoginLogic * login =   param->login;
    SessionPtr   session = param->session;
    ////////////////////////////////////////////////////////////////
    session->SetState(Session::PLAYER_STATE_LOADING);
    db::Role* pRole = login->zoneMgr->GetDBProxy().FindRoleObject(session->GetUID());
    int ret = 0;
    if(!pRole)
    {
        ret = login->zoneMgr->GetDBProxy().GetRole(session->GetUID(),
            ROLE_DB_GET_LOGIN,co->iID);        
    }    
    else
    {
        LOG_INFO("player = %lu found in db proxy cache !",session->GetUID());
    }
    LOG_INFO("get role ret = %d uid = %lu ",ret,session->GetUID());
    if( ret < 0 )
    {
        return ;   
    }
    if(ret == DATA_OK)
    {
        //attach player agent
        if(!pRole)
        {
            pRole = login->zoneMgr->GetDBProxy().FindRoleObject(session->GetUID());
        }
        if(!pRole)
        {
            LOG_ERROR("get role object error !");
            session->Kick(Session::KICK_REASON_DB_ERR);            
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
        LOG_ERROR("get role db error ret = %d",ret);
        session->Kick(Session::KICK_REASON_DB_ERR);        
        return ;
    }    
}
static void	Logout(Coroutine* co,void* arg)
{
    LoginParam *  param = (LoginParam*)arg;
    LoginLogic *  login =   param->login;
    SessionPtr    session = param->session;
    //////////////////////////////////////

    int retry = 3;
TRY_SAVE_PLAYER:    
    int ret = login->zoneMgr->GetDBProxy().UpdateRole(
            session->GetUID(),
            session->GetPlayerAgent()->GetRoleData(),
            co->iID,ROLE_DB_UPDATE_LOGOUT);
    --retry;
    LOG_INFO("update role ret = %d uid = %lu .",ret,session->GetUID());
    if( ret < 0 )
    {
        return ;   
    }
    if(ret == DATA_OK)
    {         
        return;
    }
    else
    {
        if(retry > 0)
        {
            goto TRY_SAVE_PLAYER;
        }
        LOG_ERROR("save role db ret = %d retry 3 times !",ret);
        return ;
    }    
}
int     LoginLogic::Login(SessionPtr & session)
{
    LoginParam lp = {this,session};
    int coid = CoroutineMgr::Instance().Create(::Login,&lp);
    return CoroutineMgr::Instance().Resume(coid);
}
int     LoginLogic::NotifyCreateRole(Session & session)
{
    //todo    
    //auto
    LOG_INFO("todo notify client to create role !");
    return CreateRole(session);
}
int     LoginLogic::CreateRole(Session & session)
{    
    //init player
    db::Role * pRole = new db::Role();    
    pRole->set_rid(session.GetUID());    
    session.AttachPlayerAgent(PlayerAgentPtr(new PlayerAgent(pRole,false)));    
    session.GetPlayerAgent()->InitBirthPlayer();
    return 0;
}
int     LoginLogic::EnterPlaying(Session & session)
{
    //
    LOG_INFO("player = %lu  login sucess !");
    return -1;
}
int     LoginLogic::Logout(SessionPtr & session)
{
    switch(session->GetState())
    {            
        case  Session::PLAYER_STATE_PLAYING     :            
            break;
        default:
            //err
            return -1;
    }
    session->SetState(Session::PLAYER_STATE_OFFLINE);
    /////////////////////////////////////////////////////////////////
    LoginParam lp = {this,session};
    int coid = CoroutineMgr::Instance().Create(::Logout,&lp);
    return CoroutineMgr::Instance().Resume(coid);
    return 0;
}



