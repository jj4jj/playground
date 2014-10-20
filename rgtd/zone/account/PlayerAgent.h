#pragma once
#include "base/stdinc.h"
#include "proto/gen/db/include.h"
#include "logic/PlayerLogicCenter.h"

class PlayerAgent
{
public:
    typedef     shared_ptr<db::Role>     RolePtr;
public:
    int         Attach(RolePtr ptr);
    uint64_t    GetUID();
    uint32_t    GetAreaID();      
    string      GetName();
    inline      PlayerLogicCenter &  GetLogicCenter(){return *ptrLogic;}
private:
    PlayerLogicCenterPtr      ptrLogic;    
private:
    RolePtr             ptrRole;
};
typedef shared_ptr<PlayerAgent>     PlayerAgentPtr;


