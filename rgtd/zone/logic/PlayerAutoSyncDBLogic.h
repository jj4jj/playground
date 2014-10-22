#pragma once
#include "PlayerLogicCenter.h"

class DBProxy;
class PlayerAutoSyncDBLogic : public PlayerLogic
{
public:    
    PlayerAutoSyncDBLogic(PlayerAgent* _pa,const string & nm):PlayerLogic(_pa,nm){}
    virtual void    OnAttach();
    virtual void    OnDetach();
    virtual void    OnEvent(int iEvent,int iParam = 0,void * arg = NULL);    
private:
    DBProxy*     db;
    uint32_t     m_dwTimerID;
};

