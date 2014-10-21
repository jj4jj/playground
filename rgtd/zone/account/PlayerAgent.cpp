#include "base/CommonMacro.h"
#include "AgentApp.h"


////////////////////////////////////////////////////
#include "PlayerAgent.h"



#if 1
PlayerAgent::PlayerAgent(db::Role * role,bool fromdb)
{
    pRoleData = role;
    m_fromDB = fromdb;
    ptrLogic = PlayerLogicCenterPtr(new PlayerLogicCenter(this));
    m_iDirty = 0;
}
PlayerAgent::~PlayerAgent()
{
    //free role data
    AgentApp * agent = (AgentApp*)GetApp();
    if(pRoleData != NULL)
    {
        if(m_fromDB)
        {
            agent->meta.FreeObj(pRoleData);
        }
        else
        {
            SAFE_DELETE(pRoleData);
        }
        pRoleData = NULL;
    }
    m_iDirty = 0;
}
#endif

#if 1
//initilaize 
void     PlayerAgent::InitBirthPlayer()
{
    ptrLogic->InitBirthPlayer();
}
//attach role data
void     PlayerAgent::Attach()
{
    ptrLogic->Attach();
}
//detach role data
void     PlayerAgent::Detach()
{
    ptrLogic->Detach();
}
#endif

#if 1


uint64_t    PlayerAgent::GetUID()
{
    return pRoleData->rid();
}
uint32_t    PlayerAgent::GetAreaID()
{
    //get id
    //todo
    return -1;
}
const string  &   PlayerAgent::GetName()
{
    return pRoleData->name();
}

#endif


