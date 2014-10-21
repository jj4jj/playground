#include "base/Log.h"
#include "proto/gen/gate/include.h"
#include "Session.h"

Session::Session(int src):gate(src)
{
    uid = area = 0;
    idx = -1;
    port = 0;
    state = PLAYER_STATE_INIT;
}
int       Session::Init(uint64_t uid_,const gate::GateConnection & ggc)
{
    //set uid
    uid = uid_;
    accountid = ggc.uid();
    //ip = ggc.ip();
    port = ggc.port();
    idx = ggc.idx();
    area = ggc.area();
    return 0;
}
int       Session::Kick(int reason)
{
    //todo notify client close connection
    //TODO
    LOG_DEBUG("kick reason = %u",reason);
    //delete flag
    SetState(PLAYER_STATE_KICKING);
    return -1;
}
int       Session::AttachPlayerAgent(PlayerAgentPtr ptrPlayer)
{
    player = ptrPlayer;
    //todo sth
    return 0;
}

int       Session::Send(const cs::CSMsg & csmsg)
{
    //
    
    return 0;
}
void      Session::SetState(uint8_t st)
{
    LOG_INFO("uid = %lu set state [%d]->[%d] ",GetUID(),GetState(),st);
    state = st;
}

