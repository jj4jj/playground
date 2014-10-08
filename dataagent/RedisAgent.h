#pragma once
#include "base/stdinc.h"
#include "base/Buffer.h"
#include "base/Singleton.hpp"


struct redisAsyncContext;
struct redisReply;

struct  RedisAddr
{
    string  ip;
    int     port;
    uint    dbidx;
};


struct RedisClientContext
{
    RedisAddr   addr;
    redisAsyncContext*  ctx;
    int         db_selected;
    ///////////////////////////
    RedisClientContext()
    {
        ctx = NULL;
        db_selected = false;
    }
};


class RedisCommandListener
{
public:
    virtual   int     OnCommand(redisReply *reply,Buffer & cb,bool timeout = false) = 0;
};
typedef shared_ptr<RedisCommandListener>    RedisCommandListenerPtr;
//////////////////////////////////////////////////////////////////////////////////



struct RedisAgentCallBack
{
    Buffer      cb;
    uint32_t    time;    
};

class RedisAgent : public Singleton<RedisAgent>
{
public:
    RedisAgent();
    ~RedisAgent();
public:
    static  void CommandCallback(redisAsyncContext *c, void *r, void *privdata);
    static  void ConnectCallback(const redisAsyncContext *c, int status);
    static  void DisConnectCallback(const redisAsyncContext *c, int status);
public:
    RedisAgentCallBack * FindCallBack(uint32_t cbid);
    void     OnCommand(redisAsyncContext *c,redisReply *reply,uint32_t cbid);
    bool     AllContextReady();
    int      Init(const vector<RedisAddr> & addrList,RedisCommandListenerPtr lisener_);
    void     Stop();
    int      Polling(int chkPerTick);
public:
    int      Get(string & key,const Buffer & cb);
    int      Update(string & key,const Buffer & obj,const Buffer & cb);
    int      Remove(string & key,const Buffer & cb);
    int      Command(const Buffer & cb,const char * pszFormat,...);    
protected:
    RedisClientContext*    FindContext(const redisAsyncContext *c);
    void     CheckTimeOut(int iChkNum    =   10);
    void     OnSelectDB(redisAsyncContext *c,redisReply *reply,Buffer & cb);
    void     SelectDB(int dbidx);
private:
    RedisCommandListenerPtr                       m_listener;
    uint32_t                                      m_dwCallBackSeq;
    unordered_map<uint32_t,RedisAgentCallBack>    m_mpCallBackPool;
    uint32_t                                      m_chkTimeOut;
    vector<RedisClientContext>                    redisCtxList;
    int                                           m_iConnected;
    int                                           m_closing;
};


