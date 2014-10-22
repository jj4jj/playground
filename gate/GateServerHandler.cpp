#include <stdint.h>
#include "base/Log.h"
#include "proto/gen/gate/gate.pb.h"
#include "GateFrame.h"
#include "app/App.hpp"
#include "GateServerContext.h"
#include "base/DateTime.h"
/////////////////////////////////////////////////////////////////////////////////////
#include "GateServerHandler.h"

#if 1
enum    GateConnectionState
{
    STATE_INVALID       = 0,
    STATE_CONNECTED     = 1,
    STATE_AUTHING       = 2,
    STATE_AUTHORIZED    = 3,
};
static const int CONNECTION_DEFAULT_RECV_BUFF_SIZE = GateFrame::MAX_GATE_FRAME_SIZE + sizeof(uint32_t);
GateServerHandler::Connection::Connection()
{
    Init();
}
GateServerHandler::Connection::~Connection()
{
    recvBuffer.Destroy();
}
void GateServerHandler::Connection::Init()
{
    iIdx = -1;
    iDstChannel = 0;
    iMsgLen = 0;
    bState = STATE_INVALID;
    ulUid = 0;
    recvBuffer.iUsed = 0;
    ///////////////////////
    tLastActTime = 0;
    sToken = "";

    dwRecvNum = 0 ;
    dwSendNum = 0 ;
    dwLastPeriodRecvNum = 0 ;
    dwLastPeriodSendNum = 0 ;
    tLastPeriodTime = 0 ;     
    dwCurrentRecvSpeed = 0;
    dwCurrentSendSpeed = 0;    
}
#define CONNECTION_STAT_PERIOD_TIME     (5)
void GateServerHandler::Connection::UpdateStat(bool recv)
{
    tLastActTime = GetApp()->GetTime().tv_sec;
    if(recv)
    {
        dwRecvNum++;        
    }
    else
    {
        dwSendNum++;
    }
    if(tLastPeriodTime + CONNECTION_STAT_PERIOD_TIME <= tLastActTime)
    {
        //get current speed
        dwCurrentRecvSpeed = (dwRecvNum - dwLastPeriodRecvNum)/CONNECTION_STAT_PERIOD_TIME;
        dwCurrentSendSpeed = (dwSendNum - dwLastPeriodSendNum)/CONNECTION_STAT_PERIOD_TIME;
        dwLastPeriodRecvNum = dwRecvNum;
        dwLastPeriodSendNum = dwSendNum;
        tLastPeriodTime = tLastActTime;
    }
}

void GateServerHandler::Connection::Close()
{
    switch(bState)
    {
        case STATE_CONNECTED:
        case STATE_AUTHING:
        cliSocket.Close();
        break;
        case STATE_AUTHORIZED:
        cliSocket.Close();
        break;
        case STATE_INVALID:
        break;
    }
    bState = STATE_INVALID;
    iIdx = -1;
}           

#endif

#if 1
GateServerHandler::~GateServerHandler()
{
    for(int i = 0;i < (int)m_vecConnections.size(); ++i)
    {
        Connection * pConn = GetConnectionByIdx(i);
        if(pConn)
        {
            RemoveConnection(pConn,gate::GateConnection::CONNECTION_CLOSE_STOP_SVR);
        }
    }
}
GateServerHandler::GateServerHandler(ChannelMsgProxy * p,int iMaxConnections)
{
    m_iMaxConnection = iMaxConnections;
    m_iAlivedConnections = 0;        
    m_mpConnections.clear();
    m_vecConnections.resize(m_iMaxConnection);
    m_pChannelProxy = p;
    /////////////////////////////////////////
        
}



#endif


#if 1
////////////////////////////////////////////////////////////////////////////////////////
int     GateServerHandler::OnNewConnection(TcpSocket   &   client)
{   
    m_iAlivedConnections++;
    int iIdx = GetNextIdx();
    LOG_INFO("new connection comming fd = %d alived = %d total = %d",client.GetFD(),m_iAlivedConnections,m_vecConnections.size());
    if(!m_vecConnections[iIdx].recvBuffer.pBuffer &&
        m_vecConnections[iIdx].recvBuffer.Create(CONNECTION_DEFAULT_RECV_BUFF_SIZE))
    {
        LOG_ERROR("client allocate recv buffer error");
        client.Close();
        return -1;    
    }
    //build client fd map idx
    //////////////////////////////////////////////////////////////////////////////
    m_vecConnections[iIdx].Init();
    m_vecConnections[iIdx].bState = STATE_CONNECTED;//not real state
    m_vecConnections[iIdx].cliSocket = client;
    //authorizing
    if(NotifyNeedAuth(&(m_vecConnections[iIdx])))
    {
        LOG_ERROR("client notify auth error !");
        m_vecConnections[iIdx].Close();
        return -1;
    }    
    ///////////////////////////////////////////////////////
    m_vecConnections[iIdx].iIdx = iIdx;
    m_mpConnections[client.GetFD()] = iIdx;

    return 0; 
}
int     GateServerHandler::OnClientDataRecv(TcpSocket &   client,const Buffer & recvBuffer)
{
    Connection* pConn = GetConnectionByFD(client.GetFD());
    if(!pConn)
    {
        LOG_FATAL("can't find a connection fd = %d",client.GetFD());
        client.Close();
        return -1;
    }

    int iRet = 0;
    #define GATE_MESSAGE_PREFIX_LEN  (sizeof(uint16_t))
    //msg:lenth+data
    //current state must be no msg or part msg
    LOG_DEBUG("recv connx uid = %u data len = %d msg len = %d rcved = %d",
        pConn->ulUid,recvBuffer.iUsed,
        pConn->iMsgLen,pConn->recvBuffer.iUsed);
    int iRcvBuffOffSet = 0;
    if( 0 == pConn->iMsgLen)
    {
        if(pConn->recvBuffer.iUsed >= (int)GATE_MESSAGE_PREFIX_LEN)
        {
            LOG_ERROR("connect idx = %d recv buff part len = %d",ConnxToIndex(pConn),pConn->recvBuffer.iUsed);
            pConn->Close();
            return -1;
        }        
        //if has recv a message prefix length
        if(pConn->recvBuffer.iUsed + recvBuffer.iUsed >=  (int)GATE_MESSAGE_PREFIX_LEN)
        {
            iRcvBuffOffSet = GATE_MESSAGE_PREFIX_LEN - pConn->recvBuffer.iUsed ;
            memcpy(pConn->recvBuffer.pBuffer + pConn->recvBuffer.iUsed,
                   recvBuffer.pBuffer,
                   iRcvBuffOffSet);
            pConn->iMsgLen = ntohs(*((uint16_t*)(pConn->recvBuffer.pBuffer)));
            pConn->recvBuffer.iUsed = 0;
        }
        else
        {
            //copy rest buffer
            memcpy(pConn->recvBuffer.pBuffer + pConn->recvBuffer.iUsed,
                   recvBuffer.pBuffer + iRcvBuffOffSet,
                   recvBuffer.iUsed - iRcvBuffOffSet);                        
        }
    }
    //even a msg prefix length not received
    if( 0 == pConn->iMsgLen)        
    {
        //no message
        return 0;
    }
    /////////////////////////////////////////////////////////////
    //has a part msg , recv new msg        offset:iUsed[len]
    if(recvBuffer.iUsed + pConn->recvBuffer.iUsed - iRcvBuffOffSet >= pConn->iMsgLen)
    {
        //got a msg
        memcpy(pConn->recvBuffer.pBuffer+pConn->recvBuffer.iUsed,
                recvBuffer.pBuffer + iRcvBuffOffSet ,
                pConn->iMsgLen - pConn->recvBuffer.iUsed);
        iRcvBuffOffSet += (pConn->iMsgLen - pConn->recvBuffer.iUsed);
        //////////////////////////////////////////////////////////////////////////////            
        int iMsgRet = OnClientMessage(pConn,(char*)pConn->recvBuffer.pBuffer,pConn->iMsgLen);
        if(iMsgRet)
        {
            LOG_INFO("msg error = %d",iMsgRet);
            ++iRet;
        }     
        pConn->iMsgLen = 0;
        pConn->recvBuffer.iUsed = 0;
        if(recvBuffer.iUsed <= iRcvBuffOffSet)
        {
            return iRet;
        }
        iRet += OnClientDataRecv(client,Buffer((char*)recvBuffer.pBuffer+iRcvBuffOffSet,recvBuffer.iUsed - iRcvBuffOffSet));
    }        
    return iRet;
}
int     GateServerHandler::OnClientMessage(GateServerHandler::Connection* pConn,char* pMsgBuffer,int iMsgLen)
{

    LOG_DEBUG("recv connx uid = %u msg = %d state = %d" ,pConn->ulUid,iMsgLen,pConn->bState);
    pConn->UpdateStat(true);
    switch(pConn->bState)
    {        
        case STATE_CONNECTED:
        {
            //a package
            //read a auth
            gate::GateAuth  ga;
            if(!ga.ParseFromArray(pMsgBuffer,iMsgLen))
            {
                LOG_ERROR("decode ga pack error!");  
                return -1;
            }            
            /////////////////////
            int iRet = Authorizing(pConn,ga.authreq());
            //////////////////////////////////////////
            if(0 == iRet)
            {
                LOG_INFO("got a ga package cmd = %d uid = %lu type =%d pwd = %s",
                    ga.cmd(),ga.authreq().uid(),
                    ga.authreq().auth(),
                    ga.authreq().token().c_str() );
                /////////////////////////////////////////////
                //create a connection                 
                NotifyAuthResult(pConn,iRet);    
                pConn->bState = STATE_AUTHORIZED;                
                ReportEvent(pConn,gate::GateConnection::EVENT_CONNECTED,0);
            }
            else if(iRet > 0)
            {
                pConn->bState = STATE_AUTHING;
            }
            else
            {
                LOG_ERROR("ulUid = %lu authorizing error = %d",ga.authreq().uid(),iRet);
                RemoveConnection(pConn,gate::GateConnection::CONNECTION_CLOSE_EXCEPTION);
                return -1;
            }
        }
        break;
        case STATE_AUTHING:
        {            
            LOG_DEBUG("user id = %lu is authing ",pConn->ulUid);
            return 0;
        }
        break;
        case STATE_AUTHORIZED:
            //rely to agent
            LOG_INFO("rely to agent = %d msg len = %d",pConn->iDstChannel,iMsgLen);
            ForwardData(pConn,Buffer(pMsgBuffer,iMsgLen));
        break;
        ///////////////////////////////////////////
        //nothing to do ? error state ?
        default:
            RemoveConnection(pConn,gate::GateConnection::CONNECTION_CLOSE_EXCEPTION);
            return -1;
    }
    return 0;
}

int     GateServerHandler::OnConnectionClosed(TcpSocket &  client)
{
    m_iAlivedConnections--;
    LOG_INFO("a connection fd = %d closed client total = %d",client.GetFD(),m_iAlivedConnections);
    Connection * pConn = GetConnectionByFD(client.GetFD());
    if(!pConn)
    {
        LOG_ERROR("not found connection by fd = %d",client.GetFD());
        return -1;    
    }
    RemoveConnection(pConn,gate::GateConnection::CONNECTION_CLOSE_BY_CLIENT);
    return 0;
}
#endif



#if 1
int     GateServerHandler::NotifyNeedAuth(GateServerHandler::Connection* pConn)
{    
    gate::GateAuth    ga;
    ga.set_cmd(gate::GateAuth::GATE_NEED_AUTH);    
    //void* data, int size
    return SendToClient(pConn,ga);
}
int         GateServerHandler::Authorizing(GateServerHandler::Connection * pConn,const gate::AuthReq & auth)
{
    GateServerContext * gsc = (GateServerContext *)GetApp()->GetContext();
    LOG_INFO("authorizing uid = %d area = %d type = %d token = %s ... auth mode = %d !",
        auth.uid(),auth.area(),auth.auth(),auth.token().c_str(),gsc->iNeedAuth);
    if(gsc->mpAreaID2ChannelID.find(auth.area()) != gsc->mpAreaID2ChannelID.end())
    {
        pConn->iArea = auth.area();
        pConn->iDstChannel = gsc->mpAreaID2ChannelID[auth.area()];
    }
    else
    {
        LOG_ERROR("area = %d not found !",auth.area());
        return -1;
    }
    pConn->ulUid = auth.uid();
    pConn->sToken = auth.token(); 
    pConn->tLastActTime = Time::GetTimeStampNow();
    if(gsc->iNeedAuth)
    {
        //local auth 
        //
        
        return 1;
    }
    else
    {
        return 0;
    }
}
int         GateServerHandler::NotifyAuthResult(GateServerHandler::Connection* pConn,int result)
{
    gate::GateAuth    ga;
    ga.set_cmd(gate::GateAuth::GATE_AUTH_RSP);
    ga.mutable_authrsp()->set_result(result);
    return SendToClient(pConn, ga);   
}


int     GateServerHandler::GetNextIdx()
{
    if(m_iAlivedConnections >= m_iMaxConnection)
    {
        return -1;
    }
    for(int i = 1;i < (int)m_vecConnections.size(); ++i)
    {
        if(m_vecConnections[i].bState)
        {
            continue;
        }
        return i;
    }
    return -1;
}
void        GateServerHandler::RemoveConnection(Connection* pConn,int iReason)
{
    m_mpConnections.erase(pConn->cliSocket.GetFD());
    ReportEvent(pConn,gate::GateConnection::EVENT_CLOSE,iReason);
    pConn->Close();
}
GateServerHandler::Connection* GateServerHandler::GetConnectionByFD(int fd)
{
    if(m_mpConnections.find(fd) != m_mpConnections.end())
    {
        return GetConnectionByIdx(m_mpConnections[fd]);
    }
    return NULL;
}
int         GateServerHandler::ConnxToIndex(GateServerHandler::Connection* p)
{
    assert(p != NULL);
    assert(p->iIdx >= 0);
    return p->iIdx;
}

GateServerHandler::Connection* GateServerHandler::GetConnectionByIdx(int idx)
{
    if(idx <= 0 || idx >= (int)m_vecConnections.size())
    {
        return NULL;
    }
    if(m_vecConnections.at(idx).bState == STATE_INVALID)
    {
        return NULL;
    }
    return &(m_vecConnections.at(idx));    
}
void        GateServerHandler::ReportEvent(Connection* pConn,int iEvent,int iParam,const Buffer * data )
{

    gate::GateConnection gc;
    gc.set_event((gate::GateConnection_EventType)iEvent);
    gc.set_idx(m_mpConnections[pConn->cliSocket.GetFD()]);    
    gc.set_uid(pConn->ulUid);
    gc.set_area(pConn->iArea);
    switch(iEvent)
    {
        case gate::GateConnection::EVENT_CONNECTED:
        gc.set_ip(pConn->cliSocket.GetPeerAddress().GetIP());
        gc.set_port(pConn->cliSocket.GetPeerAddress().GetPort());
        break;
        case gate::GateConnection::EVENT_CLOSE:
        gc.set_reason(iParam);
        break;
        case gate::GateConnection::EVENT_DATA:
        break;
        default:
        assert(false);
        return ;
        break;
    }
    ///////////////////////////////////////////////
    LOG_INFO("report event = %d to dst = %d",iEvent,pConn->iDstChannel);

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
    m_pChannelProxy->SendToAgent(pConn->iDstChannel,sendBuffVec,headBuffer.iUsed);
    headBuffer.Destroy();
}
void        GateServerHandler::ForwardData(Connection* pConn,const Buffer& buffer)
{
    //
    ReportEvent(pConn,gate::GateConnection::EVENT_DATA,0,&buffer);
    
    //////////////////////////////////////////////////////
    GateServerContext * gsc = (GateServerContext *)GetApp()->GetContext();
    if(gsc->iTestMode)
    {
        //TEST MODE'
        static Buffer  s_b;
        if(!s_b.pBuffer)
        {
            s_b.Create(1024);
            s_b.iUsed = 1024;        
        }
        SendFrameToClient(pConn,GateFrame((char*)(s_b.pBuffer),s_b.iCap));
    }
}
int         GateServerHandler::SendFrameToClient(Connection* pConn,const GateFrame & frame)
{
    pConn->UpdateStat(false);
    uint16_t wMsgLen = htons(frame.size);
    if(pConn->cliSocket.Send(Buffer((char*)&wMsgLen,sizeof(GateFrame::FrameLength))))
    {
        return -1;
    }
    return    pConn->cliSocket.Send(Buffer(frame.pData,frame.size));
}
int         GateServerHandler::SendToClient(Connection* pConn,gate::GateAuth & ga)
{
     //void* data, int size
    assert(ga.ByteSize() < GateFrame::MAX_GATE_FRAME_SIZE );
    Buffer buf;
    if(buf.Create(ga.ByteSize()))
    {
        LOG_ERROR("no mem !");
        return -1;
    }
    if(!ga.SerializeToArray(buf.pBuffer,buf.iCap))
    {
        LOG_ERROR("serialize error !");
        buf.Destroy();
        return -2;
    }
    buf.iUsed = buf.iCap;
    int iSndRet = SendFrameToClient(pConn,GateFrame(buf));    
    buf.Destroy();
    
    return iSndRet;    
}
#endif

