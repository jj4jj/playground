#pragma once
#include "base/stdinc.h"
#include "base/Buffer.h"
#include "thread/Thread.h"
#include "thread/ThreadSync.h"
#include "MysqlProxy.h"
#include "MysqlCommandListener.h"
////////////////////////////////////////////////////////////

/*
Agent(main thread)
    |_________ worker thread
                |
               proxy (connection)
*/
/*
main thread
    poll rsp queue handle them
    
worker thread
    poll req queue handle them
    
*/
//req - > sql
class MysqlAgent
{
public:
    MysqlAgent();
    ~MysqlAgent();
public:
    int            Request(const MysqlRequest & req);    
public:
    //return 0 
    void           SetListener(MysqlCommandListenerPtr listener);
    int            Init(const std::vector<MysqlProxyOption> & proxyList,
                        const std::vector<MysqlMeta> & metas,
                        int iMaxRequestQueueSize = 32);
    void           WaitRequest();
    MysqlRequest * GetRequest(MysqlRequest & req);
    void    Response(const MysqlResponse & rsp);
    static  void * Working(void*  p);
    int     Start();
    int     Stop();
 	int     Polling(int iProcNumPerTick);
    void    HandleRsp(MysqlResponse & rsp);
    void    Destroy();
public:
    MysqlMeta* GetTableMeta(const string & name);
	//-------------------------------------------------------------------------
private:
    std::vector<MysqlProxyOption>     proxyAddrList;
    bool    close;
    std::vector<MysqlMeta>       tableMetas;
    vector<Thread>                 worker; 
    std::vector<MysqlProxyPtr>   clientProxyList;
    int                     max_req_queue_size  ;

    std::list<MysqlRequestPtr>       reqQueue;
    Mutex                            reqlock;
    std::list<MysqlResponsePtr>      rspQueue;
    Mutex                            rsplock;
    Condition                        statechg;
    Mutex                            statechglock;
    MysqlCommandListenerPtr          m_ptrListener;
    
};






