
#include "base/Log.h"
#include "base/CommonMacro.h"
#include <hiredis.h>

#include "ConfigCenter.h"

class RedisReplyGuard
{
private:
    redisReply  * & reply;
public:
    RedisReplyGuard(redisReply * & p):reply(p)
    {

    }
    ~RedisReplyGuard()   
    {
        if(reply)
        {
            freeReplyObject(reply);
            reply = NULL;
        }
    }
};


ConfigCenter::ConfigCenter()
{
    m_ctx = NULL;
    m_reply = NULL;
    m_buffer.Create(1024*1024);
}
ConfigCenter::~ConfigCenter()
{
    Destroy();
}
void    ConfigCenter::Destroy()
{
    LOG_INFO("config center destroy !");
    if(m_reply)
    {
        freeReplyObject(m_reply);
        m_reply = NULL;        
    }
    if(m_ctx)
    {
        redisFree(m_ctx);
        m_ctx = NULL;
    }
    m_buffer.Destroy();
}
int     ConfigCenter::Init(const char* pszHostName ,int port,int dbidx ,int timeout )
{
    m_ctx = NULL;
    m_reply = NULL;
    /////////////////////////////////
    struct timeval tvout = { timeout/1000, (timeout%1000)*1000 };
    m_ctx = redisConnectWithTimeout(pszHostName, port, tvout);
    if (m_ctx == NULL || m_ctx->err) 
    {
        if (m_ctx) 
        {
            LOG_ERROR("redis Connection error: %s\n", m_ctx->errstr);
            redisFree(m_ctx);
        }
        else
        {
            LOG_ERROR("redis Connection error: can't allocate redis context\n");
        }
        return -1;
    }    
    m_reply = (redisReply*)redisCommand(m_ctx,"SELECT %d",dbidx);
    freeReplyObject(m_reply);
    LOG_INFO("config center init ok !");
    return HeartBeat();
}
int    ConfigCenter::HeartBeat()
{
    /* PING server */
    m_reply = (redisReply*)redisCommand(m_ctx,"PING");
    {
        RedisReplyGuard guard(m_reply);
        if(m_reply->type  == REDIS_REPLY_ERROR )
        {
            LOG_ERROR("ping redis error = [%s]",m_reply->str);
            return -1;
        }
        return 0;        
    }
}
int     ConfigCenter::RemoveConfig(const char* pszKey,const char* pszConfigName )
{
    m_reply = (redisReply*)redisCommand(m_ctx,"DEL %s:%s",pszConfigName,pszKey);
    {
        RedisReplyGuard guard(m_reply);
        if(m_reply->type  == REDIS_REPLY_ERROR )
        {
            LOG_ERROR("del config  redis error = %s",m_reply->str);
            return -1;
        }
        LOG_INFO("del config = %s:%s ok !",pszConfigName,pszKey);
        return 0;
    }
}

const   char* ConfigCenter::GetConfig(const char* pszKey,const char* pszConfigName)
{
    m_reply = (redisReply*)redisCommand(m_ctx,"GET %s:%s",pszConfigName,pszKey);
    {
        RedisReplyGuard guard(m_reply);
        if(m_reply->type  == REDIS_REPLY_NIL )
        {
            LOG_INFO("get config  redis conf %s:%s is nil",pszConfigName,pszKey);            
            return NULL;
        }
        else if(m_reply->type  == REDIS_REPLY_ERROR ||
                m_reply->str == NULL)
        {
            LOG_INFO("get config  redis conf %s:%s error type = %d",
                pszConfigName,pszKey,m_reply->type);            
            return NULL;
        }
        LOG_INFO("get config  redis conf %s:%s value = %s",
            pszConfigName,pszKey,m_reply->str);            
        STRNCPY((char*)m_buffer.pBuffer,m_buffer.iCap,m_reply->str);
        return (const char*)m_buffer.pBuffer;
    }
}
int     ConfigCenter::SetConfig(const char* pszKey,const char* pszValue,const char* pszConfigName)
{
    m_reply = (redisReply*)redisCommand(m_ctx,"SET %s:%s %s",pszConfigName,pszKey,pszValue);
    {
        RedisReplyGuard guard(m_reply);
        if(m_reply->type  == REDIS_REPLY_ERROR )
        {
            LOG_ERROR("set config  redis error = %s",m_reply->str);
            return -1;
        }
        LOG_INFO("set config = %s:%s %s ok !",pszConfigName,pszKey,pszValue);
        return 0;
    }
}
int     ConfigCenter::SetConfigInt(const char* pszKey,int value,const char* pszConfigName )
{
    char szNumBuffer[20];
    snprintf(szNumBuffer,sizeof(szNumBuffer),"%d",value);
    return SetConfig(pszKey,szNumBuffer,pszConfigName);
}
int     ConfigCenter::GetConfigInt(const char* pszKey,int defaultvalue,const char* pszConfigName )
{
    const char * pv = GetConfig(pszKey,pszConfigName);
    if(!pv)
    {
        return defaultvalue;
    }
    return atoi(pv);
}

