#pragma once
#include "account/PlayerAgent.h"
#include "account/Session.h"
#include "proto/gen/cs/include.h"

class CSMsgHandler
{
public:
    virtual int   OnCreatingMsg(Session & session,const cs::CSMsg & msg);
    virtual int   OnPlayingMsg(Session & session,const cs::CSMsg & msg);
public:
    int     Handle(Session & session,const cs::CSMsg & msg);
};

