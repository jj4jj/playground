#include "base/Log.h"
#include "base/Buffer.h"
#include "proto/gen/gate/include.h"
#include "app/App.hpp"
#include "ZoneAgent.h"
#include "ZoneAgentMgr.h"
#include "SessionMgr.h"


#include "Session.h"

Session::Session(int src,SessionMgr* smgr):gate(src),sessionMgr(smgr)
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
int       Session::ResponseClient(int iEvent,int iParam,Buffer * data)
{
    gate::GateConnection gc;
    gc.set_event((gate::GateConnection_EventType)iEvent);
    
    gc.set_idx(idx);    
    gc.set_uid(accountid);
    gc.set_area(area);
    switch(iEvent)
    {
        case gate::GateConnection::EVENT_CONNECTED:
        break;
        case gate::GateConnection::EVENT_CLOSE:
        gc.set_reason(iParam);
        break;
        case gate::GateConnection::EVENT_DATA:
        break;
        default:
        assert(false);
        return -1;
        break;
    }
    ///////////////////////////////////////////////
    LOG_INFO("report event = %d to gate dst = %d",iEvent,gate);

    //todo use stack buffer 
    Buffer headBuffer;
    headBuffer.Create(gc.ByteSize());
    gc.SerializeToArray(headBuffer.pBuffer,headBuffer.iCap);
    headBuffer.iUsed = headBuffer.iCap;
    vector<Buffer>  sendBuffVec;
    sendBuffVec.push_back(headBuffer);
    if(data)
    {
        sendBuffVec.push_back(*data);
    }    
    int ret = GetApp()->GetChannelProxy().SendToAgent(gate,sendBuffVec,headBuffer.iUsed);
    headBuffer.Destroy();
    return ret;
}
int       Session::Kick(int reason)
{
    //todo notify client close connection
    LOG_DEBUG("kick reason = %u",reason);
    //delete flag
    sessionMgr->GetZoneAgent().GetZoneMgr().GetLoginLogic().Logout(*this);
    //---------------------------------------------------------------
    ResponseClient(gate::GateConnection::EVENT_CLOSE,
                   gate::GateConnection::CONNECTION_CLOSE_BY_DEFAULT);    
    return 0;
}
int       Session::AttachPlayerAgent(PlayerAgentPtr ptrPlayer)
{
    player = ptrPlayer;
    //todo sth
    return 0;
}

int       Session::Send(const cs::CSMsg & csmsg)
{
    switch(GetState())
    {
        case  PLAYER_STATE_LOADING     :
        case  PLAYER_STATE_CREATING    :
        case  PLAYER_STATE_PLAYING     :
            break;
        default:
            //err
            return -1;
    }
    //
    Buffer  buffer ;
    int ret = sessionMgr->PackCSMsg(csmsg,buffer);
    if(ret)
    {
        LOG_ERROR("pack cs msg error !");
        return -1;
    }
    return ResponseClient(gate::GateConnection::EVENT_DATA,0,&buffer);
}
void      Session::SetState(uint8_t st)
{
    LOG_INFO("uid = %lu set state [%d]->[%d] ",GetUID(),GetState(),st);
    state = st;
}

