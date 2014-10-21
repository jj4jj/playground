
#include "base/Log.h"
#include "base/Buffer.h"
#include "DataListener.h"
#include "hiredis.h"
#include "RedisAgent.h"


//////////////////////////////////////////////////////////////////
int   DataListener::OnGet(int ret,void* obj,Buffer & cb)
{
    LOG_DEBUG("on get reply ret = %d",ret);
    return 0;
}
int   DataListener::OnInsert(int ret,Buffer & cb)
{
    LOG_DEBUG("on insert reply ret = %d",ret);
    return 0;
}
int   DataListener::OnRemove(int ret,Buffer & cb)
{
    LOG_DEBUG("on remove reply ret = %d",ret);
    return 0;
}
int   DataListener::OnUpdate(int ret,Buffer & cb)
{
    LOG_DEBUG("on update reply ret = %d",ret);
    return 0;

}    
int   DataListener::OnCreateTable(const char* pszName,int ret)
{
    LOG_INFO("create table = %s ret = %d",pszName,ret);
    return 0;
}


