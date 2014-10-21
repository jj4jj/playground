#pragma once
#include "base/stdinc.h"
#include "proto/gen/db/include.h"
#include "logic/PlayerLogicCenter.h"

class PlayerAgent
{
public:
    typedef     shared_ptr<db::Role>     RolePtr;
    PlayerAgent(db::Role * role,bool fromdb);
    ~PlayerAgent();
public:
    uint64_t    GetUID();
    uint32_t    GetAreaID();      
    const string  &    GetName();
    inline      PlayerLogicCenter &  GetLogicCenter(){return *ptrLogic;}
    const db::Role * GetRoleData(){return pRoleData;}
    db::Role *       MutableRoleData(){ m_iDirty++ ;return pRoleData;}    
private:
    PlayerLogicCenterPtr          ptrLogic;    
private:
    db::Role*                     pRoleData;
    int                           m_iDirty;
    bool                          m_fromDB;
};
typedef shared_ptr<PlayerAgent>     PlayerAgentPtr;


