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
    int   CreateItem();
    //role item
    int   PutItem();
    int   RemoveItem(uint32_t id,uint32_t num);
    int   GetItemNum(uint32_t id);
    int   GetMaxGrid();
    int   ChangeMaxGrid(int chg);

};

