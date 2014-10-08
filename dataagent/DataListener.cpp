
#include "base/Log.h"
#include "base/Buffer.h"
#include "DataListener.h"
#include "hiredis.h"
#include "RedisAgent.h"


//////////////////////////////////////////////////////////////////
int   DataListener::OnGet(int ret,void* obj,Buffer & cb)
{
    LOG_DEBUG("on get reply type = %d",ret);
    return 0;
}
int   DataListener::OnInsert(int ret,Buffer & cb)
{
    LOG_DEBUG("on insert reply type = %d",ret);
    return 0;
}
int   DataListener::OnRemove(int ret,Buffer & cb)
{
    LOG_DEBUG("on remove reply type = %d",ret);
    return 0;
}
int   DataListener::OnUpdate(int ret,Buffer & cb)
{
    LOG_DEBUG("on update reply type = %d",ret);
    return 0;

}    

