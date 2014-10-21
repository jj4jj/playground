#pragma once
#include "PlayerLogicCenter.h"
class PlayerTaskLogic : public PlayerLogic
{
public:    
    PlayerTaskLogic(PlayerAgent* _pl,const string & nm):PlayerLogic(_pl,nm){}
};

