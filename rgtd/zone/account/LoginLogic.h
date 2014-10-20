#pragma once

class Session;
class ZoneAgentMgr;

class LoginLogic
{
public:
    int     Init(ZoneAgentMgr * mgr);
    int     LoadPlayer(Session & session);
    int     OnLoadPlayer(int ret,uint64_t uid,db::Role * role);
    int     NotifyCreateRole(Session & session);
    int     AutoCreateRole(Session & session);
    int     EnterPlaying(Session & session);
private:
    ZoneAgentMgr    * zoneMgr;
};
