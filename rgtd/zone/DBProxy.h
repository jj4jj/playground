#pragma once
#include "base/stdinc.h"
#include "datacenter/DataCommon.h"

enum    RoleDBReason
{
    ROLE_DB_REASON_DEFAULT    =   0,
    ROLE_DB_GET_LOGIN         =   1,
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

class ZoneAgentMgr;
class DBProxy
{
public:
    DBProxy(ZoneAgentMgr* _agent):zoneMgr(_agent){}
public:
    int CommonResume(int ret,uint64_t uid,int coid,int reason);
    int GetRole(uint64_t uid,int reason,int coid = 0,uint64_t ulTrigger = 0);
    int OnGetRole(int ret,void * obj,uint64_t uid, int reason, int coid,uint64_t ulTrigger);
    db::Role*   FindRoleObject(uint64_t uid);
    int InsertRole(uint64_t uid,const db::Role* pRole,int coid = 0 );
    int OnInsertRole(int ret,uint64_t uid,int coid,int reason);

private:
    ZoneAgentMgr    * zoneMgr;
};

