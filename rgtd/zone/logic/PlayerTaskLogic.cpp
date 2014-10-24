
#include "base/Log.h"
#include "proto/gen/db/include.h"
#include "PlayerTaskLogic.h"

db::RoleTaskInfo* PlayerTaskLogic::GetTaskInfo()
{
    return player->MutableRoleData()->mutable_grow()->mutable_task();
}

void    PlayerTaskLogic::OnInitBirthPlayer()
{
}
int     PlayerTaskLogic::CheckOneTask(db::RoleOneTask * task,int iEvent,uint64_t ulParam,void * arg)
{
    //read config from task table 
    //check task state [task config also config the task event handler]
    
    return 0;    
}
void    PlayerTaskLogic::OnEvent(int iEvent,uint64_t ulParam ,void * arg )
{
    //check event
    db::RoleTaskInfo * task = GetTaskInfo();
    for(int i = 0;i < task->doing_size(); ++i)
    {
        db::RoleOneTask * one_task = task->mutable_doing(i);
        int state = one_task->state();
        //if check error , delete the task
        if(CheckOneTask(one_task,iEvent,ulParam,arg))
        {
            LOG_ERROR("check player = %lu task = %u error , delete it !",
                      player->GetUID(),one_task->id());
            task->mutable_doing()->SwapElements(i,task->doing_size()-1);
            task->mutable_doing()->RemoveLast();
            --i;
            continue;
        }
        if( state == db::RoleOneTask::TASK_STATE_DOING &&
            one_task->state() == db::RoleOneTask::TASK_STATE_DONE )
        {
            //create task finish event
            EventCenter::Instance().FireEvent(EVENT_PLAYER_TASK_DONE,
                GetName().c_str(),0,one_task->id());
        }
    }    
}

