#include "base/Log.h"
#include "ZoneAgentMgr.h"
#include "RoleDBListener.h"

int   RoleDBListener::OnGet(int ret,void* obj,Buffer & cb)
{
    DBProxyCallBack * dcb = (DBProxyCallBack *)cb.pBuffer;
    return zoneMgr->GetDBProxy().OnGetRole(ret,obj,dcb->uid,
            dcb->reason,dcb->coid,dcb->trigger);
}
int   RoleDBListener::OnInsert(int ret,Buffer & cb)
{
    DBProxyCallBack * dcb = (DBProxyCallBack *)cb.pBuffer;
    return zoneMgr->GetDBProxy().OnInsertRole(ret,dcb->uid,dcb->coid,dcb->reason);
}
int   RoleDBListener::OnRemove(int ret,Buffer & cb)
{
    LOG_INFO("todo");
    return 0;
}
int   RoleDBListener::OnUpdate(int ret,Buffer & cb)
{
    DBProxyCallBack * dcb = (DBProxyCallBack *)cb.pBuffer;
    return zoneMgr->GetDBProxy().OnUpdateRole(ret,dcb->uid,dcb->coid,dcb->reason);
}

