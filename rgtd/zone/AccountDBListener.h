#pragma once
#include "datacenter/DataListener.h"


//R : openid ->  gid
//L : gid    ->  openid

#define  ACCOUNT_R  ('R')
#define  ACCOUNT_L  ('L')

class ZoneAgentMgr;
class AccountDBListener : public DataListener
{
public:
    AccountDBListener(ZoneAgentMgr* zoneMgr_,char dir):zoneMgr(zoneMgr_),chDirection(dir){}
public:   
    virtual   int   OnGet(int ret,void* obj,Buffer & cb);
    virtual   int   OnInsert(int ret,Buffer & cb);
    virtual   int   OnRemove(int ret,Buffer & cb);
    virtual   int   OnUpdate(int ret,Buffer & cb);
private:
    ZoneAgentMgr    * zoneMgr;
    char              chDirection;
};

