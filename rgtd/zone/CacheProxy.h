#pragma once

class ZoneAgentMgr;
class CacheProxy
{

public:
    CacheProxy(ZoneAgentMgr* _agent):agentMgr(_agent){}

private:
    ZoneAgentMgr    * agentMgr;
};



