#pragma once
#include "PlayerLogicCenter.h"



class PlayerKnapsackLogic : public PlayerLogic
{
public:    
    PlayerKnapsackLogic(PlayerAgent* _pl):PlayerLogic(_pl,"knapsack"){}
    db::RolePackage * GetPackInfo();
    virtual void    OnInitBirthPlayer();
    virtual void    OnEvent(int iEvent,uint64_t ulParam = 0,void * arg = NULL);
public:
    //todo
    //get role item
    int   CreateItem(db::RoleItem & item);
    //role item    
    int   PutItem(const db::RoleItem & item,uint32_t num);
    int   GetItemCapacity(uint32_t  id);
    int   RemoveItem(uint32_t id,uint32_t num);
    int   RemoveGrid(uint32_t pos);
    int   GetItemNum(uint32_t id);
    int   GetMaxGrid();
    int   ChangeMaxGrid(uint8_t chg);
private:
    bool    IsGridFill(uint32_t pos);
    int     GetFreeGridPos();    
    db::RolePackGrid *  FindPackGrid(uint32_t pos);
    int FindPackGridIdx(uint32_t pos);

};

