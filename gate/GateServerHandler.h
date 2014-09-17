#pragma once
#include "net/TcpServerHandler.h"
#include "proto/gate/gate.pb.h"
//gate server report connection event to agent
//msg
//

class GateChannelProxy;
class GateServerHandler: public TcpServerHandler
{
public:
    int     OnNewConnection( TcpSocket   &   client);
    int     OnClientDataRecv( TcpSocket &   client,const Buffer & recvBuffer);
    int     OnConnectionClosed( TcpSocket &  client);
public:
    virtual ~GateServerHandler();
    GateServerHandler(GateChannelProxy * p,int iMaxConnections);

private:
    int     GetNextIdx();
    
private:
    //
    struct  Connection
    {
        enum    
        {
            STATE_INVALID       = 0,
            STATE_CONNECTED     = 1,
            STATE_AUTHING       = 2,
            STATE_AUTHORIZED    = 3,
        };
		enum
		{
			DEFAULT_RECV_BUFF_SIZE = (1<<16)+sizeof(uint32_t),
		};
        int         iIdx;
        TcpSocket   cliSocket;
        Buffer      recvBuffer;
        int         iDst;
        int         iMsgLen;
        int         bState;//invalid ? init ? authorized ? 
        uint64_t    ulUid;//
    public:
        Connection()
        {
            Init();
        }
        ~Connection()
        {
            recvBuffer.Destroy();
        } 
        void Init()
        {
            iIdx = -1;
            iDst = 0;
            iMsgLen = 0;
            bState = STATE_INVALID;
            ulUid = 0;
            recvBuffer.iUsed = 0;
        }
        void Close()
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
               
    };
public:
    int     OnClientMessage(Connection* pConn,char* pMsgBuffer,int iMsgLen);
    void        RemoveConnection(Connection* pConn,int iReason);
    Connection* GetConnectionByFD(int fd);    
    Connection* GetConnectionByIdx(int idx);
    int         ConnxToIndex(Connection* p);
    void        ReportEvent(Connection* pConn,int iEvent,int iParam);
    void        ForwardData(Connection* pConn,const Buffer& buffer);
    int         SendToClient(int iIdx,const Buffer & buff);
    int         SendToClient(Connection* pConn,const Buffer & buff);
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
    GateChannelProxy * m_pChannelProxy;
}; 

