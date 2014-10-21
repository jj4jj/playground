
#include "PlayerTaskLogic.h"

///------------------------------------
#include "PlayerLogicCenter.h"


void     PlayerLogicCenter::InitLogic()
{
    task = PlayerTaskLogicPtr(new PlayerTaskLogic(host));
}



    