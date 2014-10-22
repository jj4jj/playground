
#include "base/Log.h"

#include "PlayerTaskLogic.h"
#include "PlayerAutoSyncDBLogic.h"



///------------------------------------
#include "PlayerLogicCenter.h"


#if 1

void    PlayerLogic::OnInitBirthPlayer()
{
    LOG_DEBUG("todo %s",name.c_str());
}
void    PlayerLogic::OnAttach()
{
    LOG_DEBUG("todo %s",name.c_str());
}
void    PlayerLogic::OnDetach()
{
    LOG_DEBUG("todo %s",name.c_str());
}
void    PlayerLogic::OnEvent(int iEvent,int iParam ,void * arg )
{
    LOG_DEBUG("todo %s",name.c_str());
}

#endif


#if 1


#define PLAYER_LOGIC_CENTER_VISIT_ALL(handler,hdrargs...)    do{\
    LogicMapItr it = m_mpLogics.begin();\
    while(it != m_mpLogics.end())\
    {\
        it->second->On##handler(hdrargs);\
        ++it;\
    }\
}while(0)    

//initilaize 
void     PlayerLogicCenter::InitBirthPlayer()
{
    PLAYER_LOGIC_CENTER_VISIT_ALL(InitBirthPlayer);
}
//attach role data
void     PlayerLogicCenter::Attach()
{
    PLAYER_LOGIC_CENTER_VISIT_ALL(Attach);
}
//detach role data
void     PlayerLogicCenter::Detach()
{
    PLAYER_LOGIC_CENTER_VISIT_ALL(Detach);
}
//notifier other logic part
void     PlayerLogicCenter::NotifyEvent(int iEvent,int iParam,const char* pszLogicName,void * arg )
{
    if(pszLogicName)
    {
        string  sLogicName = pszLogicName;
        LogicMapItr it = m_mpLogics.find(sLogicName);
        if(it != m_mpLogics.end())
        {
            it->second->OnEvent(iEvent,iParam,arg);
        }        
    }
    else
    {
        PLAYER_LOGIC_CENTER_VISIT_ALL(Event,iEvent,iParam,arg);
    }
}
void     PlayerLogicCenter::InitLogic()
{
    string name = string("task");
    Register(name.c_str(),PlayerLogicPtr(new PlayerTaskLogic(host,name)));
    //other
    name = string("autosync");
    Register(name.c_str(),PlayerLogicPtr(new PlayerAutoSyncDBLogic(host,name)));

    
}
int      PlayerLogicCenter::Register(const char* pszLogicName,PlayerLogicPtr logic)
{
    string  sLogicName = pszLogicName;
    LogicMapItr it = m_mpLogics.find(sLogicName);
    if(it != m_mpLogics.end())
    {
        LOG_ERROR("reapeat register name = %s",pszLogicName);
        return -1;
    } 
    m_mpLogics[sLogicName] = logic;
    return 0;
}

#endif



    
