#include <stdint.h>
#include "base/Log.h"
#include "proto/gate/gate.pb.h"
#include "GateChannelProxy.h"

/////////////////////////////////////////////////////////////////////////////////////
#include "GateServerHandler.h"

#if 1
GateServerHandler::~GateServerHandler()
{
    for(int i = 0;i < (int)m_vecConnections.size(); ++i)
    {
        Connection * pConn = GetConnectionByIdx(i);
        if(!pConn)
        {
            RemoveConnection(pConn,gate::GateConnection::CONNECTION_CLOSE_STOP_SVR);
        }
    }
}
GateServerHandler::GateServerHandler(GateChannelProxy * p,int iMaxConnections)
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
        m_vecConnections[iIdx].recvBuffer.Create(Connection::DEFAULT_RECV_BUFF_SIZE))
    {
        LOG_ERROR("client allocate recv buffer error");
        client.Close();
        return -1;    
    }
    m_mpConnections[client.GetFD()] = iIdx;
    //m_vecConnections[iIdx].bState = Connection::STATE_CONNECTED;//not real state
    m_vecConnections[iIdx].cliSocket = client;
    m_vecConnections[iIdx].iIdx = iIdx;
    m_vecConnections[iIdx].bState = Connection::STATE_AUTHING;
    //authorizing
    if(NotifyNeedAuth(&(m_vecConnections[iIdx])))
    {
        LOG_ERROR("client notify auth error !");
        m_vecConnections[iIdx].Close();
        return -1;
    }    
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
    int iRcvBuffOffSet = 0;
    if( 0 == pConn->iMsgLen)
    {
        assert(pConn->recvBuffer.iUsed < (int)GATE_MESSAGE_PREFIX_LEN );
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
    if(recvBuffer.iUsed + pConn->recvBuffer.iUsed >= pConn->iMsgLen)
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
        iRet += OnClientDataRecv(client,Buffer((char*)recvBuffer.pBuffer+iRcvBuffOffSet,recvBuffer.iCap - iRcvBuffOffSet));
    }        
    return iRet;
}
int     GateServerHandler::OnClientMessage(GateServerHandler::Connection* pConn,char* pMsgBuffer,int iMsgLen)
{
    switch(pConn->bState)
    {        
        case Connection::STATE_AUTHING:
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
            NotifyAuthResult(pConn,iRet);            
            if(0 == iRet)
            {
                LOG_INFO("got a ga package cmd = %d uid = %lu type =%d pwd = %s",
                    ga.cmd(),ga.authreq().id(),
                    ga.authreq().auth(),
                    ga.authreq().token().c_str() );
                pConn->bState = Connection::STATE_AUTHORIZED;
            }
        }
        break;
        case Connection::STATE_AUTHORIZED:
            //rely to agent
            LOG_INFO("rely to agent = %d",pConn->iDst);
            ForwardData(pConn,Buffer(pMsgBuffer,iMsgLen));
        break;
        ///////////////////////////////////////////
        //nothing to do ? error state ?
        case Connection::STATE_CONNECTED:
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
    int iIdx = ConnxToIndex(pConn);   
    gate::GateAuth    ga;
    ga.set_cmd(gate::GateAuth::GATE_NEED_AUTH);

    //void* data, int size
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
        return -1;
    }
    //todo :  add a buffer cache the msg ?
    //...
    buf.iUsed = ga.ByteSize();
    int iRet = SendToClient(iIdx,buf);
    buf.Destroy();
    return iRet;
}
int         GateServerHandler::Authorizing(GateServerHandler::Connection * pConn,const gate::AuthReq & auth)
{
    LOG_INFO("authorizing uid = %d type = %d token = %s ... ok !",auth.id(),auth.auth(),auth.token().c_str());
    return 0;
}
int         GateServerHandler::NotifyAuthResult(GateServerHandler::Connection* pConn,int result)
{
    int iIdx = ConnxToIndex(pConn);
    gate::GateAuth    ga;
    ga.set_cmd(gate::GateAuth::GATE_AUTH_RSP);
    ga.mutable_authrsp()->set_result(result);
    //void* data, int size
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
        return -1;
    }
    buf.iUsed = ga.ByteSize();
    int iSndRet = SendToClient(iIdx,buf);
    buf.Destroy();
    return iSndRet;
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
    return p->iIdx;
}

GateServerHandler::Connection* GateServerHandler::GetConnectionByIdx(int idx)
{
    if(idx <= 0 || idx >= (int)m_vecConnections.size())
    {
        return NULL;
    }
    if(m_vecConnections.at(idx).bState == Connection::STATE_INVALID)
    {
        return NULL;
    }
    return &(m_vecConnections.at(idx));    
}
void        GateServerHandler::ReportEvent(Connection* pConn,int iEvent,int iParam)
{

    gate::GateConnection gc;
    gc.set_event((gate::GateConnection_EventType)iEvent);
    gc.set_idx(m_mpConnections[pConn->cliSocket.GetFD()]);    
    switch(iEvent)
    {
        case gate::GateConnection::EVENT_CONNECTED:
        gc.set_ip(pConn->cliSocket.GetPeerAddress().GetIP());
        gc.set_port(pConn->cliSocket.GetPeerAddress().GetPort());
        gc.set_uid(pConn->ulUid);
        break;
        case gate::GateConnection::EVENT_CLOSE:
        gc.set_uid(pConn->ulUid);            
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
    LOG_INFO("report event = %d to dst = %d",iEvent,pConn->iDst);

    Buffer buff;
    buff.Create(gc.ByteSize());
    gc.SerializeToArray(buff.pBuffer,buff.iCap);
    m_pChannelProxy->SendToAgent(pConn->iDst,buff);
    buff.Destroy();
}
void        GateServerHandler::ForwardData(Connection* pConn,const Buffer& buffer)
{
    gate::GateConnection gc;
    gc.set_event(gate::GateConnection::EVENT_DATA);
    gc.set_idx(m_mpConnections[pConn->cliSocket.GetFD()]);    
    gc.set_uid(pConn->ulUid);            
    string msg;
    Buffer buff;
    buff.Create(gc.ByteSize());
    gc.SerializeToArray(buff.pBuffer,buff.iCap);
    std::vector<Buffer> vBuff;
    vBuff.push_back(buff);
    vBuff.push_back(buffer);
    m_pChannelProxy->SendToAgent(pConn->iDst,vBuff);    
    buff.Destroy();
}
int         GateServerHandler::SendToClient(int iIdx,const Buffer & buff)
{
    Connection* pConn = GetConnectionByIdx(iIdx);
    if(!pConn)
    {
        return -1;
    }
    
    assert(buff.iUsed < (1<<(sizeof(uint16_t)*8)) );
    uint16_t wMsgLen = htons((uint16_t)buff.iUsed);
    if(pConn->cliSocket.Send(Buffer((char*)&wMsgLen,sizeof(uint16_t))))
    {
        return -1;
    }
    return    pConn->cliSocket.Send(buff);
}

#endif

