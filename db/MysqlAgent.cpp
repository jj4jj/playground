#include "base/DateTime.h"
#include "base/Log.h"
#include "base/StringUtil.h"

#include "MysqlAgent.h"


#if 1
MysqlAgent::MysqlAgent()
{

}

MysqlAgent::~MysqlAgent()
{
    Destroy();
}

#endif
#if 1

int    MysqlAgent::Request(const DBTableOpReq & req)
{
    if(reqQueue.size() >=(size_t)max_req_queue_size )
    {
        LOG_ERROR("request queue reach too max !");
        return -1;
    }
    reqlock.Lock();
    reqQueue.push_back(DBTableOpReqPtr(new DBTableOpReq(req)));
    reqlock.Unlock();
    statechg.NotifyAll();
    return 0;
}

#endif

////////////////////////////////////////////////////////////

#if 1
//return 0 
int     MysqlAgent::RegisterTableHandler(const string & name,DBTableHandlerPtr handler)
{
    size_t hashid = StringUtil::Hash(name);
    if(handlerMap.find(hashid) != handlerMap.end())
    {
        LOG_FATAL("repeat register hnalder = %s hashid = %u",name.c_str(),hashid);
        return -1;
    }
    LOG_INFO("register table handler = %s hash = %u",name.c_str(),hashid);
    handlerMap[hashid] = handler;
    return 0;        
}
int     MysqlAgent::Init(const std::vector<DBProxyOption> & proxyList,
                         const std::vector<DBTableMeta> & metas,
                         int iMaxRequestQueueSize )
{
    close = true;
    clientProxyList.clear();
    for(uint i = 0;i < proxyList.size(); ++i)
    {
        MysqlProxyPtr  ptr(new MysqlProxy(this));
        if(ptr->Init(metas))
        {
            LOG_FATAL("init proxy error !");
            return -1;
        }
        if(ptr->Connect(proxyList[i]))
        {
            LOG_FATAL("connect proxy error !");
            return -1;
        }            
        clientProxyList.push_back(ptr);    
    } 
    close = false;
    tableMetas = metas;
    max_req_queue_size = iMaxRequestQueueSize;
    LOG_DEBUG("init mysql agent ok !");
    return 0;
}
void           MysqlAgent::WaitRequest()
{
    LOG_DEBUG("waiting request !");
    statechglock.Lock();
    statechg.WaitMutex(statechglock);
    statechglock.Unlock();
    //pthread_cond_wait(&stateChg,&stateChgMutext);
}
DBTableOpReq * MysqlAgent::GetRequest(DBTableOpReq & req)
{
    if(reqQueue.empty())
    {
        return NULL;
    }
    reqlock.Lock();
    //pthread_mutext_lock(&reqlock);
    req = *(reqQueue.front());
    reqQueue.pop_front();
    reqlock.Unlock();
    //pthread_mutext_unlock(&reqlock);
    return &req;;
}

void    MysqlAgent::Response(const DBTableOpRsp & rsp)
{
    rsplock.Lock();
    //pthread_mutext_lock();
    rspQueue.push_back(DBTableOpRspPtr(new DBTableOpRsp(rsp)));
    rsplock.Unlock();
    //pthread_mutext_unlock();        
}
void * MysqlAgent::Working(void*  p)
{
    LOG_DEBUG("create worker ok , it is working ...");
    MysqlProxy * proxy = (MysqlProxy*)p;
    MysqlAgent * agent = proxy->GetAgent();
    DBTableOpReq req;
#define CHECK_CLOSE \
       if(!r && agent->close)\
       {\
           break;\
       }
    DBTableOpReq * r;
    while(!agent->close)
    {
        while(!(r = agent->GetRequest(req)))
        {
           agent->WaitRequest();
           CHECK_CLOSE
        }
        CHECK_CLOSE
        proxy->DispatchReq(req);
    }   
    LOG_DEBUG("proxy thread closed !");
    return NULL;
}
int     MysqlAgent::Start()
{
    Thread thread;
    for(uint i = 0;i < clientProxyList.size(); ++i)
    {
        if(thread.Create(NULL, Working,clientProxyList[i].get()))
        {
            LOG_ERROR("create worker error !");
            return -1;
        }
        LOG_DEBUG("create worker = %u ok !",i);
        worker.push_back(thread);
    }
    return 0;
}
int     MysqlAgent::Stop()
{
    LOG_DEBUG("agent stoping ...");
    close = true;
    statechg.NotifyAll();
    for(uint i = 0;i < worker.size();++i)
    {
        worker[i].Join();
    }
    LOG_DEBUG("all proxy is stoped ...");
    worker.clear();
    clientProxyList.clear();
    //mutext cond destroy ?
    reqlock.Destroy();
    rsplock.Destroy();
    statechg.Destroy();
    statechglock.Destroy();
    return 0;
}
int     MysqlAgent::Polling(int iProcNumPerTick)
{
    //check ready queue
    rsplock.Lock();
    //pthread_mutext_lock(&rsplock);        
    while(!rspQueue.empty() && iProcNumPerTick > 0)
    {
        DBTableOpRsp * rsp = rspQueue.front().get();            
        HandleRsp(*rsp); 
        rspQueue.pop_front();
        --iProcNumPerTick;
    } 
    rsplock.Unlock();
    return iProcNumPerTick;
}
DBTableHandler * MysqlAgent::GetHandler(string & tblname)
{
    size_t hashid = StringUtil::Hash(tblname);
    if(handlerMap.find(hashid) != handlerMap.end())
    {
        return handlerMap[hashid].get();
    }
    return NULL;
}
void     MysqlAgent::HandleRsp(DBTableOpRsp & rsp)
{
    //find table
    DBTableHandler * pHandler = GetHandler(rsp.tblname);
    if(!pHandler)
    {
        LOG_FATAL("not found handler table = %s",rsp.tblname.c_str());
        return ;
    }
    //dispatch op resp
    switch(rsp.op)
    {
        case OP_SELECT:
            pHandler->OnGet(rsp.ret,rsp.data,rsp.cb);
        break;
        case OP_INSERT:
            pHandler->OnInsert(rsp.ret,rsp.cb);
        break;
        case OP_UPDATE:
            pHandler->OnUpdate(rsp.ret,rsp.cb);
        break;
        case OP_DELETE:
            pHandler->OnRemove(rsp.ret,rsp.cb);
        break; 
        case OP_CREATE_TB:
            pHandler->OnCreateTable(rsp.ret,rsp.cb);
        break;
        default:
            LOG_ERROR("not support op = %d",rsp.op);
        break;
    }
    //
}
void    MysqlAgent::Destroy()
{
    if(!close)
    {
        Stop();
    }
}
DBTableMeta* MysqlAgent::GetTableMeta(const string & name)
{
    for(uint i = 0;i < tableMetas.size(); ++i)
    {
        if(name == tableMetas[i].tblname)
        {
            return &(tableMetas[i]);
        }
    }
    return NULL;
}

#endif

