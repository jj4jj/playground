#pragma once

class Session;
class ZoneAgentMgr;

class LoginLogic
{
public:
    int     Init(ZoneAgentMgr * mgr);
    int     Login(SessionPtr & session);
    int     NotifyCreateRole(Session & session);
    int     CreateRole(Session & session);
    int     EnterPlaying(Session & session);
    int     Logout(SessionPtr & session);
public:
    ZoneAgentMgr    * zoneMgr;
};
