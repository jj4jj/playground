#pragma once
#include "base/stdinc.h"


enum    EventCodeEnumration
{
    EVENT_TEST_MIN  =   1000,
    EVENT_TEST_TEST,
    EVENT_TEST_MAX ,
    EVENT_TEST_SIZE = EVENT_TEST_MAX - EVENT_TEST_MIN,
    /////////////////////////////////////////////////////////
    EVENT_PLAYER_MIN  =   2000,
    EVENT_PLAYER_TEST,
    EVENT_PLAYER_SYNC_DB,
    
    EVENT_PLAYER_MAX ,
    EVENT_PLAYER_SIZE  = EVENT_PLAYER_MAX - EVENT_PLAYER_MIN,
    /////////////////////////////////////////////////////////

};

class EventCenter
{
public:
    int     RegisterEvent();
    int     FireEvent();
};
typedef     shared_ptr<EventCenter>     EventCenterPtr;

