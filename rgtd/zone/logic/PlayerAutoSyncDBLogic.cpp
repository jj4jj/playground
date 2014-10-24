#include "app/App.hpp"
#include "DBProxy.h"
#include "AgentApp.h"
#include "PlayerAutoSyncDBLogic.h"
#include "component/TimerMgr.h"

void    PlayerAutoSyncDBLogic::OnAttach()
{    
    db = &((AgentApp *)GetApp())->zoneMgr.GetDBProxy();
    //add timer 300s fire event    
    m_dwTimerID = EventCenter::Instance().FireEvent(EVENT_PLAYER_SYNC_DB,name.c_str(),300,player->GetUID());
}
void    PlayerAutoSyncDBLogic::OnDetach()
{
    TimerMgr::Instance().CancelTimer(m_dwTimerID);
}

void    PlayerAutoSyncDBLogic::OnEvent(int iEvent,uint64_t ulParam ,void * arg )
{
    //sync
    if(EVENT_PLAYER_SYNC_DB == iEvent )
    {
        LOG_INFO("auto sysnc db logic event handing ... !");
        if(player->IsDirty())
        {
            int ret = db->UpdateRole(player->GetUID(),player->GetRoleData(),0,ROLE_DB_UPDATE_PERIOD);
            if(ret)
            {
                LOG_ERROR("sync player = %lu db error = %d",player->GetUID(),ret);
                return ;
            }
            player->CleanDirty();  
        }
        m_dwTimerID = EventCenter::Instance().FireEvent(EVENT_PLAYER_SYNC_DB,name.c_str(),300,player->GetUID());    
    }
}


