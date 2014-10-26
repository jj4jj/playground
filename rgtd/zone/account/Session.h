#pragma once
#include "PlayerAgent.h"

namespace cs
{
    class CSMsg;
};

namespace gate
{
    class GateSSMsg;
};
namespace db
{
    class Role;
};

class SessionMgr;
class Buffer;

class Session
{
public:
    enum    SessionState
    {
        PLAYER_STATE_INIT          =   0,
        PLAYER_STATE_LOADING       =   1,
        PLAYER_STATE_CREATING      =   2,
        PLAYER_STATE_PLAYING       =   3,
        PLAYER_STATE_OFFLINE       =   4,
        PLAYER_STATE_KICKING       =   10       
    };
    enum    SessionKickReason
    {
        KICK_REASON_DEFAULT =   0,
        KICK_REASON_RELOGIN =   1,
        KICK_REASON_DB_ERR  =   2,
        KICK_REASON_CLOSE_CLIENT = 3,
    };
public:
    Session(int src,SessionMgr * smgr);
    ~Session();
public:    
    int       Init(uint64_t uid_,const gate::GateSSMsg & ggc);
    int       Kick(int reason);
    int       AttachPlayerAgent(PlayerAgentPtr ptrPlayer);
    int       ResponseClient(int iEvent,int iParam,Buffer * data = NULL);
public:
    int       Send(const cs::CSMsg & csmsg);    
    void      SetState(uint8_t st);
    inline    uint8_t       GetState(){return state;}
    inline    uint64_t      GetUID(){return uid;}
    inline    PlayerAgent * GetPlayerAgent(){return player.get();}
    inline    PlayerAgentPtr    GetPlayerAgentPtr(){return player;}
private:
    /////////////////////////////////////todo : instead of session data with protobuf
    uint64_t        uid;
    uint64_t        accountid;
    uint32_t        area;
    uint32_t        gate;
    int             idx;
    string          ip;
    int             port;
private:   
    uint8_t         state;
private:
    PlayerAgentPtr      player;
    SessionMgr      *   sessionMgr;
};
typedef shared_ptr<Session> SessionPtr;

