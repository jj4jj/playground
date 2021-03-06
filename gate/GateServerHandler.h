#pragma once
#include "net/TcpServerHandler.h"
#include "proto/gen/gate/include.h"
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
    
public:
    //
    struct  Connection
    {
        int         iIdx;
        TcpSocket   cliSocket;
        Buffer      recvBuffer;
        int         iDstChannel;
        int         iMsgLen;
        int         bState;//invalid ? init ? authorized ? 
        uint64_t    ulUid;//uid
        string      sToken;
        int         iArea;
        time_t      tLastActTime;

        //every 5 s recv package num statistic
        uint32_t    dwRecvNum;
        uint32_t    dwSendNum;
        uint32_t    dwLastPeriodRecvNum;
        uint32_t    dwLastPeriodSendNum;
        time_t      tLastPeriodTime;        
        uint32_t    dwCurrentRecvSpeed;
        uint32_t    dwCurrentSendSpeed;

        //timer check free
        uint32_t    dwTimerID;
        time_t      tLastCheckTime;
    public:
        Connection();
        ~Connection();
        void Init();    
        void UpdateStat(bool recv);
        void Close();
    };
public:
    int         OnClientMessage(Connection* pConn,char* pMsgBuffer,int iMsgLen);
    void        RemoveConnection(Connection* pConn,int iReason);
    Connection* GetConnectionByFD(int fd);    
    Connection* GetConnectionByIdx(int idx);
    int         ConnxToIndex(Connection* p);
    void        ReportEvent(Connection* pConn,int iEvent,int iParam,const Buffer * data = NULL);
    void        ForwardData(Connection* pConn,const Buffer& buffer);
    int         SendFrameToClient(Connection* pConn,const GateFrame & frame);
    int         SendToClient(Connection* pConn,gate::GateCSMsg & ga);
    int         NotifyNeedAuth(Connection* pConn);
    int         Authorizing(Connection * pConn,const gate::AuthReq & auth);
    int         NotifyAuthResult(Connection* pConn,int iRet);
    int         OnClosing(int reason,int notifinum = 200);
private:
    uint32_t                m_dwLastClosingIdx;
    vector<Connection>      m_vecConnections;
    typedef unordered_map<int,int>  FD2IDXConnectionMap;
    typedef FD2IDXConnectionMap::iterator   FD2IDXConnectionMapItr;
    unordered_map<int,int>  m_mpConnections;
    int     m_iMaxConnection;
    int     m_iAlivedConnections;    
    int     m_iLastFreeIdx ;
    ChannelMsgProxy * m_pChannelProxy;
    int     m_closing;
public:
    uint32_t        iConnxMaxIdleTime;
    uint64_t        downloadsize;
    uint64_t        uploadsize;
    uint64_t        reqpkgs;
    uint64_t        rsppkgs;
    uint64_t        downpkgs;
    uint64_t        uppkgs;

}; 



