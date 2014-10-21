#include "base/Log.h"
#include "CSMsgHandler.h"

int   CSMsgHandler::OnCreatingMsg(Session & session,const cs::CSMsg & msg)
{
    LOG_DEBUG("on creating msg cmd = %d",msg.cmd());
    return 0;
}
int   CSMsgHandler::OnPlayingMsg(Session & session,const cs::CSMsg & msg)
{
    LOG_DEBUG("on playing msg cmd = %d",msg.cmd());
    return 0;
}
int     CSMsgHandler::Handle(Session & session,const cs::CSMsg & msg)
{
    switch(session.GetState())
    {
        case Session::PLAYER_STATE_CREATING:
            return OnCreatingMsg(session,msg);
            break;
        case Session::PLAYER_STATE_PLAYING:
            return OnPlayingMsg(session,msg);
            break;
        /////////////////////////////////////////////                
        case Session::PLAYER_STATE_LOADING:
        case Session::PLAYER_STATE_INIT:
        default:
            LOG_ERROR("session uid = %u get msg = %d when state = %u",
                session.GetUID(),session.GetState());
            return -1;
            break;
    }
}
