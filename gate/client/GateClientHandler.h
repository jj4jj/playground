#pragma once

#include "proto/gate/gate.pb.h"
#include "TcpClientHandler.h"

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
class GateClientHandler : TcpClientHandler
{
public:    
    inline    int     GetState(){return m_iCurState;}
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
    int m_iCurState;
    int m_iMsgLen;
    //ring buffer todo
    Buffer m_rcvMsgBuffer;
};










