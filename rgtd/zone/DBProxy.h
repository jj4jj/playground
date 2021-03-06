#pragma once
#include "base/stdinc.h"
#include "datacenter/DataCommon.h"
#include "datacenter/MetaSerializer.h"

enum    RoleDBReason
{
    ROLE_DB_REASON_DEFAULT    =   0,
    ROLE_DB_GET_LOGIN         =   1,
    ROLE_DB_UPDATE_LOGOUT         =   2,
    ROLE_DB_UPDATE_PERIOD         =   3,
    
};
struct DBProxyCallBack
{
    uint64_t    uid;
    int         reason;
    int         coid;
    uint64_t    trigger;
    DBProxyCallBack()
    {
        bzero(this,sizeof(*this));
    }
};
namespace   db
{
    class Role;
};

class ZoneAgentMgr;
class DBProxy
{
public:
    typedef     shared_ptr<db::Role>    RolePtr;
    DBProxy(ZoneAgentMgr* _agent):zoneMgr(_agent){}
public:
    int CommonCoroutineResume(int ret,uint64_t uid,int coid,int reason);

    //role db /////////////////////////////////////////////////////////////
    int GetRole(uint64_t uid,int reason,int coid = 0,uint64_t ulTrigger = 0);
    int OnGetRole(int ret,void * obj,uint64_t uid, int reason, int coid,uint64_t ulTrigger);
    db::Role*   FindRoleObject(uint64_t uid);
    MetaSerializer::MetaObjectPtr     GetRoleObjectPtr(uint64_t uid);
    int InsertRole(uint64_t uid,const db::Role* pRole,int coid = 0 );
    int OnInsertRole(int ret,uint64_t uid,int coid,int reason);
    int UpdateRole(uint64_t uid,const db::Role* pRole,int coid = 0,int reason = 0);
    int OnUpdateRole(int ret,uint64_t uid,int coid,int reason);


    //other db /////////////////////////////////////////////////////////////


    
private:
    ZoneAgentMgr    * zoneMgr;
};

