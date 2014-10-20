#include "base/Log.h"
#include "ZoneAgentMgr.h"
#include "RoleDBListener.h"


int   RoleDBListener::OnGet(int ret,void* obj,Buffer & cb)
{
    RoleDBCallBack * pCB = (RoleDBCallBack*)cb.pBuffer;
    LOG_DEBUG("on get !");
    switch(pCB->type)
    {
        case ROLE_DB_GET_TEST:
            break;
        case ROLE_DB_GET_LOGIN:
            zoneMgr->GetLoginLogic().OnLoadPlayer(ret,pCB->uid,(db::Role*)obj);
            break;        
    }
    return 0;
}
int   RoleDBListener::OnInsert(int ret,Buffer & cb)
{
    LOG_INFO("todo");
    return 0;
}
int   RoleDBListener::OnRemove(int ret,Buffer & cb)
{
    LOG_INFO("todo");
    return 0;
}
int   RoleDBListener::OnUpdate(int ret,Buffer & cb)
{
    LOG_INFO("todo");
    return 0;
}

