#pragma once
#include "PlayerAgent.h"

namespace cs
{
    class CSMsg;
};

namespace gate
{
    class GateConnection;
};


class Session
{
public:
    enum    SessionState
    {
        STATE_INIT          =   0,
        STATE_LOADING       =   1,
        STATE_CREATING      =   2,
        STATE_PLAYING       =   3,
        STATE_OFFLINE       =   4,        
    };
    enum    SessionKickReason
    {
        KICK_REASON_DEFAULT =   0,
        KICK_REASON_RELOGIN =   1,
    };
public:
    Session(int src);
    int       Init(const gate::GateConnection & ggc);
    int       Kick(int reason);
public:
    int       Send(const cs::CSMsg & csmsg);    
    void      SetState(uint8_t st);
    inline    uint8_t     GetState(){return state;}
    inline    uint64_t    GetUID(){return uid;}
private:
    /////////////////////////////////////todo : instead of session data with protobuf
    uint64_t        uid;
    uint32_t        area;
    uint32_t        gate;
    int             idx;
    string          ip;
    int             port;
private:   
    uint8_t         state;
private:
    PlayerAgentPtr  player;

};
typedef shared_ptr<Session> SessionPtr;

