
#include "base/Log.h"
#include "gate/proto/gate/gate.pb.h"
#include "GateClientHandler.h"


#if 1
GateClientHandler::~GateClientHandler()
{
    m_rcvMsgBuffer.Destroy();
    m_iCurState = GATE_CLI_STATE_CLOSED;
    m_iMsgLen = 0;
}
GateClientHandler::GateClientHandler()
{
    //64K
    m_rcvMsgBuffer.Create(1<<(sizeof(uint16_t)));
    m_iCurState = GATE_CLI_STATE_CLOSED;
    m_iMsgLen = 0;
}
#endif


#if 1
int  GateClientHandler::OnConnect(bool bSuccess)
{
    LOG_INFO("connect status = %d ",bSuccess);
    return 0;
}
int  GateClientHandler::OnNeedAuth()
{
    LOG_INFO("recv need auth result msg ");
    return 0;
}
int  GateClientHandler::OnAuthResult(int result)
{
    LOG_INFO("recv auth result msg len = %d",result);
    return 0;
}
int  GateClientHandler::OnMessage(char* pBuffer,int iBuffLen)
{
    LOG_INFO("recv proc msg len = %d");
    return 0;
}
int  GateClientHandler::OnClose(bool bByMyself)//server or me close        
{
    LOG_INFO("recv proc close by = %d",bByMyself);        
    return 0;
}
int     GateClientHandler::DispatchMessage(char* pBuffer,int iBuffLen)
{
    switch(m_iCurState)
    {
        case GATE_CLI_STATE_CONNECTED:
        case GATE_CLI_STATE_AUTHORIZING:
        {
            gate::GateAuth ga;
            if(!ga.ParseFromArray(pBuffer,iBuffLen))
            {
                LOG_FATAL("gate gate auth parse error ! state = %d",m_iCurState);
                return -1;
            }
            switch(ga.cmd())
            {
               case gate::GateAuth::GATE_NEED_AUTH:
                return OnNeedAuth();
               break;
               case gate::GateAuth::GATE_AUTH_RSP:
               {
                    return OnAuthResult(ga.authrsp().result());
               }
               break;
            }
        }
        break;
        case GATE_CLI_STATE_AUTHORIZED:
        return OnMessage( pBuffer, iBuffLen);
    }
    LOG_ERROR("error state = %d!",m_iCurState);
    assert(false);
    return -1;
}
int     GateClientHandler::OnConnected( TcpSocket &   client)
{
    m_iCurState = GATE_CLI_STATE_CONNECTED;
    return OnConnect(true);
}
//readable
int     GateClientHandler::OnDataRecv( TcpSocket &   client,const Buffer & recvBuffer)
{
    int iRet = 0;
    #define GATE_MESSAGE_PREFIX_LEN  (sizeof(uint16_t))
    //msg:lenth+data
    //current state must be no msg or part msg
    int iRcvBuffOffSet = 0;
    if( 0 == m_iMsgLen)
    {
        assert(m_rcvMsgBuffer.iUsed < (int)GATE_MESSAGE_PREFIX_LEN );
        //if has recv a message prefix length
        if(m_rcvMsgBuffer.iUsed + recvBuffer.iUsed >=  (int)GATE_MESSAGE_PREFIX_LEN)
        {
            iRcvBuffOffSet = GATE_MESSAGE_PREFIX_LEN - m_rcvMsgBuffer.iUsed ;
            memcpy(m_rcvMsgBuffer.pBuffer + m_rcvMsgBuffer.iUsed,
                   recvBuffer.pBuffer,
                   iRcvBuffOffSet);
            m_iMsgLen = ntohs(*((uint16_t*)(m_rcvMsgBuffer.pBuffer)));
            m_rcvMsgBuffer.iUsed = 0;
        }
        else
        {
            //copy rest buffer
            memcpy(m_rcvMsgBuffer.pBuffer + m_rcvMsgBuffer.iUsed,
                   recvBuffer.pBuffer + iRcvBuffOffSet,
                   recvBuffer.iUsed - iRcvBuffOffSet);                        
        }
    }
    //even a msg prefix length not received
    if( 0 == m_iMsgLen)        
    {
        //no message
        return 0;
    }
    /////////////////////////////////////////////////////////////
    //has a part msg , recv new msg        offset:iUsed[len]
    if(recvBuffer.iUsed + m_rcvMsgBuffer.iUsed >= m_iMsgLen)
    {
        //got a msg
        memcpy(m_rcvMsgBuffer.pBuffer+m_rcvMsgBuffer.iUsed,
                recvBuffer.pBuffer + iRcvBuffOffSet ,
                m_iMsgLen - m_rcvMsgBuffer.iUsed);
        iRcvBuffOffSet += (m_iMsgLen - m_rcvMsgBuffer.iUsed);
        ////////////////////////////////////////////////////////////////            
        int iMsgRet = DispatchMessage((char*)m_rcvMsgBuffer.pBuffer,m_iMsgLen);
        if(iMsgRet)
        {
            LOG_INFO("msg error = %d",iMsgRet);
            ++iRet;
        }     
        m_iMsgLen = 0;
        m_rcvMsgBuffer.iUsed = 0;
        iRet += OnDataRecv(client,Buffer((char*)recvBuffer.pBuffer+iRcvBuffOffSet,recvBuffer.iCap - iRcvBuffOffSet));
    }        
    return iRet;
}
//peer close .
int     GateClientHandler::OnDisconnected( TcpSocket &   client)
{
    m_iCurState = GATE_CLI_STATE_CLOSED;
    m_iMsgLen = 0;
    m_rcvMsgBuffer.iUsed = 0;
    return OnClose(false);
}
#endif










