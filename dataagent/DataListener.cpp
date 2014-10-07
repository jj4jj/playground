
#include "base/Log.h"
#include "base/Buffer.h"
#include "DataListener.h"
#include "hiredis.h"
#include "RedisAgent.h"


//////////////////////////////////////////////////////////////////
int   DataListener::OnGet(redisReply* reply,Buffer & cb,bool timeout)
{
    if(reply)
    {
        LOG_DEBUG("on get reply type = %d",reply->type);
    }
    else
    {
        LOG_DEBUG("on get reply timeout = %d",reply->type,timeout?1:0);
    }
    return 0;
}
int   DataListener::OnInsert(redisReply* reply,Buffer & cb,bool timeout)
{
    if(reply)
    {
        LOG_DEBUG("on insert reply type = %d",reply->type);
    }
    else
    {
        LOG_DEBUG("on insert reply timeout = %d",reply->type,timeout?1:0);
    }
    return 0;

}
int   DataListener::OnRemove(redisReply* reply,Buffer & cb,bool timeout)
{
    if(reply)
    {
        LOG_DEBUG("on remove reply type = %d",reply->type);
    }
    else
    {
        LOG_DEBUG("on remove reply timeout = %d",reply->type,timeout?1:0);
    }
    return 0;

}
int   DataListener::OnUpdate(redisReply* reply,Buffer & cb,bool timeout)
{
    if(reply)
    {
        LOG_DEBUG("on update reply type = %d",reply->type);
    }
    else
    {
        LOG_DEBUG("on update reply timeout = %d",reply->type,timeout?1:0);
    }
    return 0;

}    

