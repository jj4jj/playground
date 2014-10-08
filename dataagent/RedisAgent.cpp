
#include "hiredis.h"
#include "async.h"
#include "adapters/libev.h"
#include "base/Log.h"
#include "base/DateTime.h"
#include "RedisAgent.h"


#define SELECTDB_CB   ("$SELECTDB")

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

    //internal cmd
    if(strstr((char*)pcb->cb.pBuffer,SELECTDB_CB))
    {
        OnSelectDB(c,reply,pcb->cb);
    }
    else
    {
        m_listener->OnCommand(reply,pcb->cb);
    }
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
        //select db
        RedisClientContext * ctx =  RedisAgent::Instance().FindContext(c);
        RedisAgent::Instance().SelectDB(ctx->addr.dbidx);
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

bool     RedisAgent::AllContextReady()
{
    for(uint i = 0; i < redisCtxList.size(); ++i)
    {
        if(!redisCtxList[i].db_selected)
        {
            return false;
        }
    }
    return true;
}
RedisClientContext* RedisAgent::FindContext(const redisAsyncContext *c)
{
    for(uint i = 0;i < redisCtxList.size(); ++i)
    {
        if(c == redisCtxList[i].ctx)
        {
            return &(redisCtxList[i]);
        }
    }
    return NULL;
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
    RedisClientContext  rcc;
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
        rcc.addr  = addrList[i];
        rcc.ctx   = c;
        redisCtxList.push_back(rcc);
    }
    ev_loop(EV_DEFAULT_ EVRUN_ONCE);
    int time_out = 3000000;
    while(!AllContextReady() && time_out > 0)
    {
        ev_loop(EV_DEFAULT_ EVRUN_NOWAIT);
        usleep(3000);
        time_out -= 3000;
    }        
    if(!AllContextReady())
    {
        LOG_FATAL("not all redis context is ready !");
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
            redisAsyncDisconnect(redisCtxList[i].ctx);
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
void     RedisAgent::OnSelectDB(redisAsyncContext *c,redisReply *reply,Buffer & cb)
{
    RedisClientContext * ctx = FindContext(c);
    cb.Destroy();
    if(!ctx)
    {
        LOG_FATAL("can't find redis agent context !");
        return ;
    }    
    if(0 == reply->integer && reply->type == REDIS_REPLY_STATUS)
    {
        ctx->db_selected = true;
    }
    else
    {
        LOG_ERROR("select db error for = %s!",reply->str);
    }
    LOG_INFO("context dbidx = %d select type = %d int = %ld!",
        ctx->addr.dbidx,reply->type,reply->integer);
}
void     RedisAgent::SelectDB(int dbidx)
{
    assert(dbidx >= 0);
    Buffer cb;
    cb.Create(strlen(SELECTDB_CB)+1);
    memcpy(cb.pBuffer,SELECTDB_CB,cb.iCap);
    if(Command(cb,"SELECT %d",dbidx))
    {
        LOG_FATAL("select db error !");
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
    int r = redisvAsyncCommand(redisCtxList[select].ctx, CommandCallback,(void*)vp,pszFormat,ap);
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
