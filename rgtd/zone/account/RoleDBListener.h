#pragma once
#include "datacenter/DBAgent.h"
#include "datacenter/DataListener.h"
#include "DBProxy.h"

class ZoneAgentMgr;
class RoleDBListener : public DataListener
{
public:
    RoleDBListener(ZoneAgentMgr* zoneMgr_):zoneMgr(zoneMgr_){}
public:   
    virtual   int   OnGet(int ret,void* obj,Buffer & cb);
    virtual   int   OnInsert(int ret,Buffer & cb);
    virtual   int   OnRemove(int ret,Buffer & cb);
    virtual   int   OnUpdate(int ret,Buffer & cb);
private:
    ZoneAgentMgr    * zoneMgr;
};

