#pragma once
#include "base/stdinc.h"
#include "EventCenter.h"

class  PlayerAgent;
class  PlayerTaskLogic;


class PlayerLogic
{
public:
    PlayerLogic(PlayerAgent* _pl,const string  nm):player(_pl),name(nm){}
    virtual  ~PlayerLogic(){player = NULL;}
public:
    virtual void    OnInitBirthPlayer();
    virtual void    OnAttach();
    virtual void    OnDetach();
    virtual void    OnEvent(int iEvent,int iParam = 0,void * arg = NULL);
protected:
    PlayerAgent*    player;
    string          name;
};
typedef shared_ptr<PlayerLogic>     PlayerLogicPtr;
///////////////////////////////////////////////////////////

class PlayerLogicCenter
{
public:
    PlayerLogicCenter(PlayerAgent * player):host(player){InitLogic();}
    ~PlayerLogicCenter(){host = NULL;}
public:
    //initilaize 
    void     InitBirthPlayer();
    //attach role data
    void     Attach();
    //detach role data
    void     Detach();
    //notifier other logic part
    void     NotifyEvent(int iEvent,int iParam = 0,const char* pszLogicName = NULL,void * arg = NULL);
public:
    void     InitLogic();
    int      Register(const char* pszLogicName,PlayerLogicPtr logic);
private:
    PlayerAgent*    host;    
public:
    /////////////////////////////////////////////////////////////
    typedef  std::map<string,PlayerLogicPtr>    LogicMap;
    typedef  LogicMap::iterator                 LogicMapItr;
    LogicMap       m_mpLogics;
};
typedef shared_ptr<PlayerLogicCenter>     PlayerLogicCenterPtr;


