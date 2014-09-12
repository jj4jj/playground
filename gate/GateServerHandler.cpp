<<<<<<< HEAD
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
    m_vecConnections[iIdx].bState = Connection::STATE_CONNECTED;
    m_vecConnections[iIdx].cliSocket = client;
    //authorizing
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
    client.Send(buf);  
    m_vecConnections[iIdx].bState = Connection::STATE_AUTHING;
    buf.Destroy();
    
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
            if(0 == pConn->iMsgLen && pConn->recvBuffer.iUsed > (int)sizeof(uint16_t))
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
            if(!ga.ParseFromArray(pConn->recvBuffer.pBuffer + sizeof(uint16_t),pConn->iMsgLen-sizeof(uint16_t)))
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
                    ga.cmd(),ga.authreq().id(),
                    ga.authreq().auth(),
                    ga.authreq().token().c_str() );
                //authorized
                pConn->recvBuffer.Destroy();
                pConn->bState = Connection::STATE_AUTHORIZED;
            }
            else
            {
                RemoveConnection(pConn,gate::GateConnection::CONNECTION_CLOSE_EXCEPTION);
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

#endif


=======
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
    m_vecConnections[iIdx].bState = Connection::STATE_CONNECTED;
    m_vecConnections[iIdx].cliSocket = client;
    //authorizing
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
    client.Send(buf);  
    m_vecConnections[iIdx].bState = Connection::STATE_AUTHING;
    buf.Destroy();
    
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
            if(0 == pConn->iMsgLen && pConn->recvBuffer.iUsed > (int)sizeof(uint16_t))
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
            if(!ga.ParseFromArray(pConn->recvBuffer.pBuffer + sizeof(uint16_t),pConn->iMsgLen-sizeof(uint16_t)))
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
                    ga.cmd(),ga.authreq().id(),
                    ga.authreq().auth(),
                    ga.authreq().token().c_str() );
                //authorized
                pConn->recvBuffer.Destroy();
                pConn->bState = Connection::STATE_AUTHORIZED;
            }
            else
            {
                RemoveConnection(pConn,gate::GateConnection::CONNECTION_CLOSE_EXCEPTION);
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

#endif


>>>>>>> 260f3888a8736281c9b1eb253c283df674e163a7
