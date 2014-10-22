
#include "app/App.hpp"
#include "PlayerAutoSyncDBLogic.h"


void    PlayerAutoSyncDBLogic::OnAttach()
{    
    db = &((AgentApp *)GetApp())->zoneMgr.GetDBProxy();
    //add timer

    eventCenter->FireEvent();
}
void    PlayerAutoSyncDBLogic::OnDetach()
{
    //remove timer
}
void    PlayerAutoSyncDBLogic::OnEvent(int iEvent,int iParam ,void * arg )
{
    //sync
    if(EVENT_PLAYER_SYNC_DB == iEvent &&
       player->IsDirty() )
    {
        int ret = db->UpdateRole(player->GetUID(),player->GetRoleData(),0,ROLE_DB_UPDATE_PERIOD);
        if(ret)
        {
            LOG_ERROR("sync player = %lu db error = %d",player->GetUID(),ret);
            return -1;
        }
        player->CleanDirty();        
    }
}


