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

int    MysqlAgent::Request(const MysqlRequest & req)
{
    if(reqQueue.size() >=(size_t)max_req_queue_size )
    {
        LOG_ERROR("request queue reach too max !");
        return -1;
    }
    reqlock.Lock();
    reqQueue.push_back(MysqlRequestPtr(new MysqlRequest(req)));
    reqlock.Unlock();
    statechg.NotifyAll();
    return 0;
}

#endif

////////////////////////////////////////////////////////////

#if 1
void     MysqlAgent::SetListener(MysqlCommandListenerPtr listener)
{
    m_ptrListener = listener;
}

int     MysqlAgent::Init(const std::vector<MysqlProxyOption> & proxyList,
                         const std::vector<MysqlMeta> & metas,
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
MysqlRequest * MysqlAgent::GetRequest(MysqlRequest & req)
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

void    MysqlAgent::Response(const MysqlResponse & rsp)
{
    rsplock.Lock();
    //pthread_mutext_lock();
    rspQueue.push_back(MysqlResponsePtr(new MysqlResponse(rsp)));
    rsplock.Unlock();
    //pthread_mutext_unlock();        
}
void * MysqlAgent::Working(void*  p)
{
    LOG_DEBUG("create worker ok , it is working ...");
    MysqlProxy * proxy = (MysqlProxy*)p;
    MysqlAgent * agent = proxy->GetAgent();
    MysqlRequest req;
#define CHECK_CLOSE \
    if(!r && agent->close)\
    {\
       break;\
    }
    MysqlRequest * r;
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
        MysqlResponse * rsp = rspQueue.front().get();            
        HandleRsp(*rsp); 
        rspQueue.pop_front();
        --iProcNumPerTick;
    } 
    rsplock.Unlock();
    return iProcNumPerTick;
}
void     MysqlAgent::HandleRsp(MysqlResponse & rsp)
{
    //find table
    if(!m_ptrListener)
    {
        LOG_FATAL("not found handler table = %s ret = %d op = %d",
            rsp.tblname.c_str(),rsp.ret,rsp.op);
        return ;
    }
    m_ptrListener->OnResponse(rsp);
    //
}
void    MysqlAgent::Destroy()
{
    if(!close)
    {
        Stop();
    }
}
MysqlMeta* MysqlAgent::GetTableMeta(const string & name)
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

