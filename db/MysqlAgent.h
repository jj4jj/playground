#pragma once
#include "base/stdinc.h"
#include "base/Buffer.h"
#include "thread/Thread.h"
#include "thread/ThreadSync.h"
#include "MysqlProxy.h"
#include "DBTableHandler.h"
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
    int            Request(const DBTableOpReq & req);    
public:
    //return 0 
    int            RegisterTableHandler(const string & name,DBTableHandlerPtr handler);
    int            Init(const std::vector<DBProxyOption> & proxyList,
                        const std::vector<DBTableMeta> & metas,
                        int iMaxRequestQueueSize = 32);
    void           WaitRequest();
    DBTableOpReq * GetRequest(DBTableOpReq & req);
    void    Response(const DBTableOpRsp & rsp);
    static  void * Working(void*  p);
    int     Start();
    int     Stop();
 	int     Polling(int iProcNumPerTick);
    DBTableHandler * GetHandler(string & tblname);
    void    HandleRsp(DBTableOpRsp & rsp);
    void    Destroy();
public:
    DBTableMeta* GetTableMeta(const string & name);
	//-------------------------------------------------------------------------
private:
    //name hash -> table
    unordered_map<size_t,DBTableHandlerPtr>  handlerMap;
    std::vector<DBProxyOption>     proxyAddrList;
    bool    close;
    std::vector<DBTableMeta>       tableMetas;
    vector<Thread>                 worker; 
    std::vector<MysqlProxyPtr>   clientProxyList;
    int                     max_req_queue_size  ;

    std::list<DBTableOpReqPtr>       reqQueue;
    Mutex                            reqlock;
    std::list<DBTableOpRspPtr>       rspQueue;
    Mutex                            rsplock;
    Condition                        statechg;
    Mutex                            statechglock;
    
};






