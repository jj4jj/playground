
#include "GateServerHandler.h"
#include "base/Log.h"
#include "proto/gate/gate.pb.h"
#if 1
GateServerHandler::~GateServerHandler()
{
    for(int i = 0;i < m_vecConnections.size(); ++i)
    {
        Connection * pConn = GetConnectionByIdx(i);
        if(!pConn)
        {
            RemoveConnection(pConn,gate::GateConnection::CONNECTION_CLOSE_STOP_SVR);
        }
    }
}
GateServerHandler::GateServerHandler(int iMaxConnections)
{
    m_iMaxConnection = iMaxConnections;
    m_iAlivedConnections = 0;        
    m_mpConnections.clear();
    m_vecConnections.resize(m_iMaxConnection);
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
        m_vecConnections[iIdx].recvBuffer.Create(Connection::DEFAULT_RECV_BUFFER_SIZE))
    {
        LOG_ERROR("client allocate recv buffer error");
        client.Close();
        return -1;    
    }
    m_mpConnections[client.GetFD()] = iIdx;
    m_vecConnections[iIdx].bState = Connection::STATE_CONNECTED;
    m_vecConnections[iIdx].cliSocket = client;
    //authorizing
    gate::GateAuth    ga;
    ga.set_cmd(GateAuth::GATE_NEED_AUTH);
    string  seri;
    ga.SerializeToString(&seri);      
    //todo :  add a buffer cache the msg ?
    //...
    client.Send(Buffer(seri.data(),seri.length()));  
    m_vecConnections[iIdx].bState = Connection::STATE_AUTHING;
    
    return 0; 
}
int     GateServerHandler::OnClientDataRecv(TcpSocket &   client,const Buffer & recvBuffer) 
{
    //get connection    
    Connection* pConn = GetConnectionByFD(client.GetFD());
    if(!pConn)
    {
        LOG_FATAL("can't find a connection fd = %d",client.GetFD());
        client.Close();
    }
    //
    switch(pConn->bState)
    {
        
        case Connection::STATE_AUTHING:
        {                        
            int iAvail = pConn->recvBuffer.iCap - pConn->recvBuffer.iUsed;
            if(recvBuffer.iUsed < iAvail)
            {
                memcpy(pConn->recvBuffer.pBuffer + pConn->recvBuffer.iUsed,
                           recvBuffer.pBuffer,recvBuffer.iUsed);
                pConn->recvBuffer.iUsed += recvBuffer.iUsed;                  
            }
            else
            {
                memcpy(pConn->recvBuffer.pBuffer + pConn->recvBuffer.iUsed,
                           recvBuffer.pBuffer,iAvail);  
                pConn->recvBuffer.iUsed = pConn->recvBuffer.iCap;
            }
            //sizeof(uint16_t) = 2
            if(0 == pConn->iMsgLen && pConn->recvBuffer.iUsed > sizeof(uint16_t))
            {
                pConn->iMsgLen = ntohs(*(short*)pConn->recvBuffer.pBuffer);                
            }
            if(pConn->recvBuffer.iUsed < pConn->iMsgLen)
            {
                break;
            }

            //a package
            //read a auth
            bool    bGotGA = true;
            gate::GateAuth  ga;
            if(!ga.SerializeFromString(pConn->recvBuffer.pBuffer + sizeof(uint16_t)))
            {
                bGotGA = false;
                LOG_ERROR("decode ga pack error!");                                        
            }                                       
            memmove(pConn->recvBuffer.pBuffer,
                    pConn->recvBuffer.pBuffer + pConn->iMsgLen,
                    pConn->recvBuffer.iUsed - pConn->iMsgLen);
            pConn->recvBuffer.iUsed -= pConn->iMsgLen;
            pConn->iMsgLen = 0;
            ///////////////////////////////////////////////////
            if(bGotGA)
            {
                LOG_INFO("got a ga package cmd = %d uid = %lu type =%d pwd = %s",
                    ga.get_cmd(),ga.get_authReq().get_id(),
                    ga.get_authReq().get_auth(),
                    ga.get_authReq().get_token());
                //authorized
                pConn->recvBuffer.Destroy();
                pConn->iState = Connection::STATE_AUTHORIZED;
            }
            else
            {
                RemoveConnection(pConn);
            }                
        }
        break;
        case Connection::STATE_AUTHORIZED:
            //rely to agent
            LOG_INFO("rely to agent = %d",pConn->iDst);
            ForwardData(pConn,recvBuffer);
        break;
        ///////////////////////////////////////////
        case Connection::STATE_CONNECTED:
            //nothing to do ? error state ?
        default:
            RemoveConnection(pConn);
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
    RemoveConnection(pConn,0)
    return 0;
}
#endif



#if 1
int     GateServerHandler::GetNextIdx()
{
    if(m_iAlivedConnections >= m_iMaxConnection)
    {
        return -1;
    }
    for(int i = 1;i < m_vecConnections.size(); ++i)
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
    m_mpConnections.erase(pConn->cliSocket.GetFD()));
    ReportEvent(pConn,gate::GateConnection::EVENT_CLOSE,iReason);
    pConn->Close();
}
Connection* GateServerHandler::GetConnectionByFD(int fd)
{
    if(m_mpConnections.find(fd) != m_mpConnections.end())
    {
        return GetConnectionByIdx(m_mpConnections[fd]);
    }
    return NULL;
}
Connection* GateServerHandler::GetConnectionByIdx(int idx)
{
    if(idx <= 0 || idx >= m_vecConnections.size())
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
    gc.set_event(iEvent);
    gc.set_idx(m_mpConnections[pConn->cliClient.GetFD());    
    switch(iEvent)
    {
        case GateConnection::EVENT_CONNECTED:
        gc.set_ip(pConn->cliSocket.GetPeerAddress().GetIP())
        gc.set_port(pConn->cliSocket.GetPeerAddress().GetPort())
        gc.set_uid(pConn->ullUid);
        break;
        case GateConnection::EVENT_CLOSE:
        gc.set_uid(pConn->ullUid);            
        gc.set_reason(iParam);
        break;
        case GateConnection::EVENT_DATA:
        break;
        default:
        assert(false);
        return -1;
        break;
    }
    ///////////////////////////////////////////////
    LOG_INFO("report event = %d to dst = %d",iEvent,pConn->iDst);

    string msg;
    gc.SerializeToString(&msg);
    SendToAgent(pConn->iDst,Buffer(msg.data(),msg.legnth());
}
void        GateServerHandler::ForwardData(Connection* pConn,const Buffer& buffer)
{
    gate::GateConnection gc;
    gc.set_event(gate::GateConnection::EVENT_DATA);
    gc.set_idx(m_mpConnections[pConn->cliClient.GetFD());    
    gc.set_uid(pConn->ullUid);            
    string msg;
    gc.SerializeToString(&msg);
    std::vector<Buffer> vBuff;
    vBuff.push_back(gc);
    vBuff.push_back(buffer);
    SendToAgent(pConn->iDst,vBuff);    
}
int         GateServerHandler::SendToAgent(int iDst,const std::vector<Buffer>  &  vBuff)
{
    int iLength = 0;
    for(int i = 0;i < vBuff.size(); ++i)
    {
        iLength += vBuff[i].iUsed;
    }   
    if(iLength > 0x7FFF)
    {
        LOG_ERROR("Buffer size is too much = %d",iLength);
        return -1;
    }
    uint16_t   nLen = htons((uint16_t)iLength);

    ChannelAgent  * pChannel = ChannelAgentMgr::Insntace().GetChannel(iDst);
    if(pChannel)
    {
        LOG_ERROR("channel agent is not ready !");
        return -1;
    }
    //////////////////////////////////////////////
    //buffer 
    //len
    //msg
        //ga_connection
        //data
    if(pChannel->GetAvaileSize() < iLength)
    {
        LOG_ERROR("channel has no more space");
        return -1;
    }
    pChannel->Write(Buffer((char*)&nLen,sizeof(nLen)));
    for(int i  = 0;i < vBuff.size(); ++i)
    {
        pChannel->Write(vBuff[i]);
    }
    return 0;
}
int         GateServerHandler::SendToAgent(int iDst,Buffer   buff)
{
    vector<Buffer>  v;
    v.push_back(buff);
    return SendToAgent(iDst,v);
}

#endif


