#pragma once
#include "net/TcpServerHandler.h"
#include "proto/gate/gate.pb.h"
#include "app/ChannelMsgProxy.h"
//gate server report connection event to agent
//msg
//
struct GateFrame;
class GateServerHandler: public TcpServerHandler
{
public:
    int     OnNewConnection( TcpSocket   &   client);
    int     OnClientDataRecv( TcpSocket &   client,const Buffer & recvBuffer);
    int     OnConnectionClosed( TcpSocket &  client);
public:
    virtual ~GateServerHandler();
    GateServerHandler(ChannelMsgProxy * p,int iMaxConnections);

private:
    int     GetNextIdx();
    
private:
    //
    struct  Connection
    {
        int         iIdx;
        TcpSocket   cliSocket;
        Buffer      recvBuffer;
        int         iDst;
        int         iMsgLen;
        int         bState;//invalid ? init ? authorized ? 
        uint64_t    ulUid;//
    public:
        Connection();
        ~Connection();
        void Init();        
        void Close();
    };
public:
    int         OnClientMessage(Connection* pConn,char* pMsgBuffer,int iMsgLen);
    void        RemoveConnection(Connection* pConn,int iReason);
    Connection* GetConnectionByFD(int fd);    
    Connection* GetConnectionByIdx(int idx);
    int         ConnxToIndex(Connection* p);
    void        ReportEvent(Connection* pConn,int iEvent,int iParam);
    void        ForwardData(Connection* pConn,const Buffer& buffer);
    int         SendFrameToClient(Connection* pConn,const GateFrame & frame);
    int         SendToClient(Connection* pConn,gate::GateAuth & ga);
    int         NotifyNeedAuth(Connection* pConn);
    int         Authorizing(Connection * pConn,const gate::AuthReq & auth);
    int         NotifyAuthResult(Connection* pConn,int iRet);
private:
    vector<Connection>      m_vecConnections;
    typedef unordered_map<int,int>  FD2IDXConnectionMap;
    typedef FD2IDXConnectionMap::iterator   FD2IDXConnectionMapItr;
    unordered_map<int,int>  m_mpConnections;
    int     m_iMaxConnection;
    int     m_iAlivedConnections;    
    int     m_iLastFreeIdx ;
    ChannelMsgProxy * m_pChannelProxy;
}; 

