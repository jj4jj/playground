#pragma once
#include "PlayerLogicCenter.h"

class DBProxy;
class PlayerAutoSyncDBLogic : public PlayerLogic
{
public:    
    PlayerAutoSyncDBLogic(PlayerAgent* _pa):PlayerLogic(_pa,"autosync"){}
    virtual void    OnAttach();
    virtual void    OnDetach();
    virtual void    OnEvent(int iEvent,uint64_t ulParam = 0,void * arg = NULL);    
private:
    DBProxy*     db;
    uint32_t     m_dwTimerID;
};

