#pragma once
#include "PlayerLogicCenter.h"
class PlayerAutoSyncDBLogic : public PlayerLogic
{
public:    
    PlayerAutoSyncDBLogic(PlayerAgent* _pa,const string & nm):PlayerLogic(_pa,nm){}
    virtual void    OnEvent(int iEvent,int iParam = 0,void * arg = NULL);    
private:
    DBProxy*    db;
    EventCenter* eventCenter;

};
