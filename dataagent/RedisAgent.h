#pragma once
#include "base/stdinc.h"
#include "base/Buffer.h"
#include "base/Singleton.hpp"

struct  RedisAddr
{
    string  ip;
    int     port;
};

struct redisReply;
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

struct redisAsyncContext;
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
    bool     AllContextConnected();
    int      Init(const vector<RedisAddr> & addrList,RedisCommandListenerPtr lisener_);
    void     Stop();
    int      Polling(int chkPerTick);
    int      Command(const char* pszCMD,const Buffer & cb);
protected:
    void     CheckTimeOut(int iChkNum    =   10);
private:
    RedisCommandListenerPtr                       m_listener;
    uint32_t                                      m_dwCallBackSeq;
    unordered_map<uint32_t,RedisAgentCallBack>    m_mpCallBackPool;
    uint32_t                                      m_chkTimeOut;
    vector<redisAsyncContext*>                    redisCtxList;
    int                                           m_iConnected;
    int                                           m_closing;
};


