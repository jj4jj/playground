#pragma once

#include "net/TcpClientHandler.h"

enum GateClientState
{
    GATE_CLI_STATE_CLOSED = 0,
    GATE_CLI_STATE_CONNECTING = 1,
    GATE_CLI_STATE_CONNECTED = 2,
    GATE_CLI_STATE_AUTHORIZING = 3,
    GATE_CLI_STATE_AUTHORIZED = 4,        
};

//gate client handler just help client connect to server
    //1.connect (try times , sync mode)
//and report server event 
    //1.connect success 
    //2.recv msg
    //3.connect close
class GateClientHandler : public TcpClientHandler
{
public:    
    inline    int     GetState(){return m_iCurState;}
    int       Authorize(int type,uint64_t uid,const char* pszToken);
    int       SendFrame(char* pBuffer,int iBuffLen);

public:
    template<class MsgType>
    int       SendMessage(const MsgType & msg)
    {
        int len = msg.ByteSize();
        if(m_packMsgBuffer.pBuffer == NULL ||
           m_packMsgBuffer.iCap < len)
        {
            m_packMsgBuffer.Destroy();
            m_packMsgBuffer.Create(len);
        }
        bool ret = msg.SerializeToArray(m_packMsgBuffer.pBuffer,len);
        if(!ret)
        {
            LOG_ERROR("pack msg error !");
            return -1;
        }
        m_packMsgBuffer.iUsed = len;
        return SendFrame((char*)m_packMsgBuffer.pBuffer,len);         
    }
public:
    virtual   int  OnConnect(bool bSuccess);
    virtual   int  OnNeedAuth();
    virtual   int  OnAuthResult(int result);
    virtual   int  OnMessage(char* pBuffer,int iBuffLen);
    virtual   int  OnClose(bool bByMyself);//server or me close        

protected:
    virtual     int     DispatchMessage(char* pBuffer,int iBuffLen);
    virtual     int     OnConnected( TcpSocket &   client);
    //readable
    virtual     int     OnDataRecv( TcpSocket &   client,const Buffer & recvBuffer);
    //peer close .
    virtual     int     OnDisconnected( TcpSocket &   client);
public:
    virtual ~GateClientHandler();
    GateClientHandler();
private:
    TcpSocket m_sock;
    int m_iCurState;
    int m_iMsgLen;
    //ring buffer todo
    Buffer m_rcvMsgBuffer;
    Buffer m_packMsgBuffer;
};










