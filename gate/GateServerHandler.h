#pragma once
#include "net/TcpServerHandler.h"

//gate server report connection event to agent
//msg
//

class GateServerHandler: public TcpServerHandler
{
public:
    int     OnNewConnection( TcpSocket   &   client);
    int     OnClientDataRecv( TcpSocket &   client,const Buffer & recvBuffer);
    int     OnConnectionClosed( TcpSocket &  client);
public:
    virtual ~GateServerHandler();
    GateServerHandler(int iMaxConnections);

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
			DEFAULT_RECV_BUFF_SIZE = 4096,
		};
        TcpSocket   cliSocket;
        Buffer      recvBuffer;
        //Buffer      sendBuffer;
        int         iDst;
        int         iMsgLen;
        int         bState;//invalid ? init ? authorized ? 
        uint64_t    ulUid;//
    public:
        Connection():bState(0),iDst(0),iMsgLen(0),ulUid(0)
        {
        }
        void Close()
        {
            switch(bState)
            {
                case STATE_CONNECTED:
                case STATE_AUTHING:
                //close
                cliSocket.Close();
                break;
                case STATE_AUTHORIZED:
                //report stop to dst server
                cliSocket.Close();
                break;
                case STATE_INVALID:
                break;
            }
            bState = STATE_INVALID;
        }
        ~Connection()
        {
            recvBuffer.Destroy();
            //sendBuffer.Destroy();
        }        
    };
public:
    void        RemoveConnection(Connection* pConn,int iReason);
    Connection* GetConnectionByFD(int fd);    
    Connection* GetConnectionByIdx(int idx);
    void        ReportEvent(Connection* pConn,int iEvent,int iParam);
    void        ForwardData(Connection* pConn,const Buffer& buffer);
    int         SendToAgent(int iDst,const std::vector<Buffer>  &  vBuff);
    int         SendToAgent(int iDst,Buffer   buff);
private:
    vector<Connection>      m_vecConnections;
    typedef unordered_map<int,int>  FD2IDXConnectionMap;
    typedef FD2IDXConnectionMap::iterator   FD2IDXConnectionMapItr;
    unordered_map<int,int>  m_mpConnections;
    int     m_iMaxConnection;
    int     m_iAlivedConnections;    
    int     m_iLastFreeIdx ;
}; 











