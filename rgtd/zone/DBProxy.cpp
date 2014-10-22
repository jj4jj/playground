
#include "ZoneAgentMgr.h"
#include "account/LoginLogic.h"
#include "datacenter/DBAgent.h"
#include "utility/Coroutine.h"
//////////////////////////////////////
#include "DBProxy.h"

////////////////////////////////////////////////////////////////////////////////

db::Role*   DBProxy::FindRoleObject(uint64_t uid)
{
    db::Role   role;
    role.set_rid(uid);
    return  (db::Role*)zoneMgr->GetDBAgent().FindObject(&role);
}
MetaSerializer::MetaObjectPtr     DBProxy::GetRoleObjectPtr(uint64_t uid)
{
    db::Role   role;
    role.set_rid(uid);
    return   zoneMgr->GetDBAgent().GetObjectPtr(&role);
}
int DBProxy::GetRole(uint64_t uid,int reason,int coid,uint64_t ulTrigger )
{    
    vector < string > all;
    DBProxyCallBack  dcb;
    dcb.uid = uid;
    dcb.reason = ROLE_DB_GET_LOGIN;
    dcb.coid = coid;
    dcb.trigger = ulTrigger;
    
    db::Role   role;
    role.set_rid(dcb.uid);
    int ret = zoneMgr->GetDBAgent().Get(&role,all,Buffer((char*)&dcb,sizeof(dcb)));
    if(ret)
    {
        LOG_ERROR("db agent get error !");
        return -1;    
    }
    if(coid != 0)
    {
        ret = CoroutineMgr::Instance().Yield();
    }
    return ret;
}
int DBProxy::CommonCoroutineResume(int ret,uint64_t uid,int coid,int reason)
{
    Coroutine*  co = CoroutineMgr::Instance().Find(coid);
    if(!co)
    {            
        LOG_ERROR("coid = %d can't find coroutine with uid = %lu reason = %d",
                coid,uid,reason);
        return -1;
    }
    return CoroutineMgr::Instance().Resume(coid,ret);
}
int DBProxy::OnGetRole(int ret,void * obj,uint64_t uid, int reason, int coid,uint64_t ulTrigger)
{
    if( coid != 0 )
    {
        return CommonCoroutineResume(ret,uid,coid,reason);
    }
    else
    {
        LOG_ERROR("no coid get role uid = %lu ret = %d reason = %d trigger = %lu",uid,ret,reason,ulTrigger);
    }    
    return 0;
}
int DBProxy::InsertRole(uint64_t uid,const db::Role* pRole,int coid )
{
    DBProxyCallBack  dcb;
    dcb.uid = uid;
    dcb.reason = 0;
    dcb.coid = coid;
    dcb.trigger = 0;    
    int ret = zoneMgr->GetDBAgent().Insert((void*)pRole,Buffer((char*)&dcb,sizeof(dcb)));
    if(ret)
    {
        LOG_ERROR("db agent insert error !");
        return -1;    
    }
    if(coid != 0)
    {
        ret = CoroutineMgr::Instance().Yield();
    }
    return ret;
}
int DBProxy::OnInsertRole(int ret,uint64_t uid,int coid,int reason)
{
    if(coid != 0)
    {
        return CommonCoroutineResume(ret,uid,coid,reason);
    }
    else
    {
        LOG_ERROR("no coid insert role uid = %lu ret = %d reason = %d trigger = %lu",uid,ret);
    }
    return 0;
}

int DBProxy::UpdateRole(uint64_t uid,const db::Role* pRole,int coid)
{
    DBProxyCallBack  dcb;
    vector<string>  all;
    dcb.uid = uid;
    dcb.reason = 0;
    dcb.coid = coid;
    dcb.trigger = 0;    
    int ret = zoneMgr->GetDBAgent().Update((void*)pRole,all,Buffer((char*)&dcb,sizeof(dcb)));
    if(ret)
    {
        LOG_ERROR("db agent update error !");
        return -1;    
    }
    if(coid != 0)
    {
        ret = CoroutineMgr::Instance().Yield();
    }
    return ret;
}
int DBProxy::OnUpdateRole(int ret,uint64_t uid,int coid,int reason)
{
    if(coid != 0)
    {
        return CommonCoroutineResume(ret,uid,coid,reason);
    }
    else
    {
        LOG_ERROR("no coid update role uid = %lu ret = %d reason = %d trigger = %lu",uid,ret);
    }
    return 0;
}

