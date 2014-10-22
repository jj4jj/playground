#pragma once
#include "base/stdinc.h"
#include "proto/gen/db/include.h"
#include "logic/PlayerLogicCenter.h"

/*
player is all role data operator
////////////////////////////////
any logic want to hold player data
must register in player logic center .

*/

class PlayerAgent
{
public:
    typedef     shared_ptr<db::Role>     RolePtr;
    PlayerAgent(db::Role * role,bool fromdb);
    ~PlayerAgent();
public:
    //initilaize 
    void     InitBirthPlayer();
    //attach role data
    void     Attach();
    //detach role data
    void     Detach();
public:
    uint64_t    GetUID();
    uint32_t    GetAreaID();      
    const       string  &    GetName();
    inline      PlayerLogicCenter &  GetLogicCenter(){return *ptrLogic;}
    const db::Role * GetRoleData(){return pRoleData;}
    db::Role *       MutableRoleData(){ m_iDirty++ ;return pRoleData;}    
    inline  bool     IsDirty(){return m_iDirty > 0;}
    inline  void     CleanDirty(){m_iDirty = 0;}
private:
    PlayerLogicCenterPtr          ptrLogic;    
private:
    db::Role*                     pRoleData;
    int                           m_iDirty;
    bool                          m_fromDB;
};
typedef shared_ptr<PlayerAgent>     PlayerAgentPtr;


