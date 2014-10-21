#pragma once

class Session;
class ZoneAgentMgr;

class LoginLogic
{
public:
    int     Init(ZoneAgentMgr * mgr);
    int     Login(Session & session);
    int     NotifyCreateRole(Session & session);
    int     AutoCreateRole(Session & session);
    int     EnterPlaying(Session & session);
    int     Logout(Session & session);
public:
    ZoneAgentMgr    * zoneMgr;
};
