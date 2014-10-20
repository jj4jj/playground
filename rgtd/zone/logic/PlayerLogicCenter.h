#pragma once
#include "base/stdinc.h"

class  PlayerAgent;
class  PlayerTaskLogic;

class PlayerLogicCenter
{
public:
    PlayerLogicCenter(PlayerAgent * player):host(player){InitLogic();}
    ~PlayerLogicCenter(){host = NULL;}
public:
    void     InitLogic();
private:
    PlayerAgent*         host;    
public:
    /////////////////////////////////////////////////////////////
    shared_ptr<PlayerTaskLogic>     task;
};
typedef shared_ptr<PlayerLogicCenter>     PlayerLogicCenterPtr;


