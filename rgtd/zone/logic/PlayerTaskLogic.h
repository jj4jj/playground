#pragma once
#include "PlayerLogicCenter.h"
class PlayerTaskLogic : public PlayerLogic
{
public:    
    PlayerTaskLogic(PlayerAgent* _pl):PlayerLogic(_pl,"task"){}
public:
    //todo
    int  AddTask(uint32_t id);
    int  RemoveTask(uint32_t id);
    bool IsTaskDid(uint32_t id);
    bool IsTaskDoing(uint32_t id);

public:
    //
    db::RoleTaskInfo* GetTaskInfo();
    int CheckOneTask(db::RoleOneTask * task,int iEvent,uint64_t ulParam = 0,void * arg = NULL);
    virtual void    OnInitBirthPlayer();
    virtual void    OnEvent(int iEvent,uint64_t ulParam = 0,void * arg = NULL);
};

