
#include "hiredis.h"
#include "async.h"
#include "adapters/libev.h"
#include "RedisAgent.h"
#include "base/Log.h"
#include "base/DateTime.h"
#if 1
RedisAgent::RedisAgent()
{
    m_dwCallBackSeq   =   1;
    m_chkTimeOut  =   5 ;
    m_closing =   false ;
    m_iConnected    = 0 ;
}
RedisAgent::~RedisAgent()
{
    Stop();
}
RedisAgentCallBack * RedisAgent::FindCallBack(uint32_t cbid)
{
    if( m_mpCallBackPool.find(cbid) != m_mpCallBackPool.end())
    {

        return &m_mpCallBackPool[cbid];
    }
    else
    {
        return NULL;
    }
}
void    RedisAgent::OnCommand(redisAsyncContext *c,redisReply *reply,uint32_t cbid)
{
    RedisAgentCallBack * pcb = FindCallBack(cbid);
    if(!pcb)
    {            
        LOG_ERROR("not found cb id = %u",cbid);
        return ;
    }
    m_listener->OnCommand(reply,pcb->cb);
    m_mpCallBackPool.erase(cbid);
}
void RedisAgent::CommandCallback(redisAsyncContext *c, void *r, void *privdata)
{
    uint32_t cbid = (uint32_t)(ptrdiff_t)privdata;
    RedisAgent::Instance().OnCommand(c,(redisReply*)r,cbid);    
}
void RedisAgent::ConnectCallback(const redisAsyncContext *c, int status)
{
    if(status == REDIS_OK)
    {
        ++RedisAgent::Instance().m_iConnected;
    }
    LOG_INFO("redis connection is connected ! status = %d cnx = %d",
        status,RedisAgent::Instance().m_iConnected);
}
void RedisAgent::DisConnectCallback(const redisAsyncContext *c, int status)
{
    if(status == REDIS_OK)
    {
        --RedisAgent::Instance().m_iConnected;
    }
    LOG_ERROR("redis connection is disconnected ! status = %d cnx = %d",
        status,RedisAgent::Instance().m_iConnected);
    //////////////////////////////////////
    //reconndecting todo
    
}

bool     RedisAgent::AllContextConnected()
{
    return      (m_chkTimeOut >= redisCtxList.size());
}
int     RedisAgent::Init(const vector<RedisAddr> & addrList,RedisCommandListenerPtr lisener_)
{
    if(m_listener)
    {
        LOG_ERROR("repeat init redis agent !");
        return -1;
    }
    if(addrList.empty())
    {
        return -1;
    }
    for(uint i = 0;i < addrList.size(); ++i)
    {            
        redisAsyncContext *c = redisAsyncConnect(addrList[i].ip.c_str(), addrList[i].port);
        if (c->err) {
            /* Let *c leak for now... */
            LOG_ERROR("Error: %s\n", c->errstr);
            return -1;
        }
        redisLibevAttach(EV_DEFAULT_ c);
        redisAsyncSetConnectCallback(c,ConnectCallback);
        redisAsyncSetDisconnectCallback(c,DisConnectCallback);   
        redisCtxList.push_back(c);
    }
    ev_loop(EV_DEFAULT_ EVRUN_ONCE);
    if(!AllContextConnected())
    {
        LOG_ERROR("not all context connected  !");
        return -1;
    }        
    m_listener = lisener_;
    m_closing = false;
    return 0;
}
void     RedisAgent::Stop()
{
    if(!m_closing)
    {
        for(uint i = 0; i < redisCtxList.size(); ++i)
        {
            redisAsyncDisconnect(redisCtxList[i]);
        }            
        m_closing   =   true;
    }
}
int     RedisAgent::Polling(int chkPerTick)
{
    if(!m_closing)
    {
        ev_loop(EV_DEFAULT_ EVRUN_NOWAIT);    
        CheckTimeOut(chkPerTick);
    }
    return 0;
}
void    RedisAgent::CheckTimeOut(int iChkNum  )
{
    if(m_chkTimeOut >  m_dwCallBackSeq)
    {
        for(int i = 0;i < iChkNum && iChkNum > 0; ++i,++m_chkTimeOut)
        {
            if(m_mpCallBackPool.find(m_chkTimeOut) == m_mpCallBackPool.end())
            {
                continue;
            }
            if(m_mpCallBackPool[m_chkTimeOut].time + 5 < 
               Time::now().tv.tv_sec)
            {
                m_listener->OnCommand(NULL,m_mpCallBackPool[m_chkTimeOut].cb,true);
                m_mpCallBackPool.erase(m_chkTimeOut);
                continue;
            }
            break;
        }
    }
    else
    {
        for(int i = 0;i < iChkNum && m_chkTimeOut <= m_dwCallBackSeq; ++i,++m_chkTimeOut)
        {
            if(m_mpCallBackPool.find(m_chkTimeOut) == m_mpCallBackPool.end())
            {
                continue;
            }
            if(m_mpCallBackPool[m_chkTimeOut].time + 5 < 
               Time::now().tv.tv_sec)
            {
                m_listener->OnCommand(NULL,m_mpCallBackPool[m_chkTimeOut].cb,true);
                m_mpCallBackPool.erase(m_chkTimeOut);
                continue;
            }
            break;
        }
        if(m_chkTimeOut > m_dwCallBackSeq)
        {
            m_chkTimeOut = m_dwCallBackSeq;
        }
    }
}
#endif


#if 1
int      RedisAgent::Get(string & key,const Buffer & cb)
{
    return Command(cb,"GET %b",key.c_str(),(size_t)key.length());
}
int      RedisAgent::Update(string & key,const Buffer & obj,const Buffer & cb)
{
    return Command(cb,"SET %b %b",key.c_str(),(size_t)key.length(),obj.pBuffer,(size_t)obj.iUsed);
}
int      RedisAgent::Remove(string & key,const Buffer & cb)
{
    return Command(cb,"DEL %b",key.c_str(),(size_t)key.length());
}
int     RedisAgent::Command(const Buffer & cb,const char * pszFormat,...)
{
    //get cbid/////////////////////////////////////////////////////
    //command
    //get ctx
    va_list ap;
    int select = rand()%redisCtxList.size();
    while(m_mpCallBackPool.find(m_dwCallBackSeq) != m_mpCallBackPool.end())
    {
        ++m_dwCallBackSeq;
    }
    ptrdiff_t  vp = m_dwCallBackSeq;

    va_start(ap,pszFormat);
    int r = redisvAsyncCommand(redisCtxList[select], CommandCallback,(void*)vp,pszFormat,ap);
    va_end(ap);
    if( r != REDIS_OK)
    {
        LOG_ERROR("redis command exe error = %d !",r);            
        return -1;
    }
    m_mpCallBackPool[m_dwCallBackSeq].cb = cb;
    m_mpCallBackPool[m_dwCallBackSeq].time = Time::now().tv.tv_sec;        
    ++m_dwCallBackSeq;        
    return 0;
}
#endif
