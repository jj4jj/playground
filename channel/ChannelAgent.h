#pragma once
#include "zmq.h"
#include "base/Buffer.h"
#include "base/CommonMacro.h"

struct  ChannelMessage
{
    uint32_t    dwSize;
    uint8_t *   pData;
};

struct Channel
{
    enum
    {
        CHANNEL_MODE_INVALID = 0,
        CHANNEL_MODE_LOCAL = 1,
        CHANNEL_MODE_REMOTE = 2,
    };
    int   mode;
    void* sender;
    void* receiver; 
    Buffer rcvBuffer;
public:
    Channel():mode(CHANNEL_MODE_INVALID),sender(NULL),receiver(NULL)
    {
    }
    int Write(const Buffer & buffer)
    {
        zmq_msg_t   sndMsg;
        zmq_msg_init(&sndMsg);
        zmq_msg_init_size(&sndMsg,buffer.iUsed);
        memcpy(zmq_msg_data(&sndMsg),buffer.pBuffer,buffer.iUsed);
        int iRet = zmq_msg_send(&sndMsg,sender,0);
        zmq_msg_close(&sndMsg);
        if(iRet)
        {
            LOG_ERROR("zmq send msg error = %d",iRet);
            return -1;
        }
        return 0;
    }
    int Read(Buffer & buffer)
    {
        zmq_msg_t   rcvMsg;
        zmq_msg_init(&rcvMsg);        
        int iRet = zmq_msg_recv(&rcvMsg,receiver,0);
        if(iRet &&
           iRet != EINTR )
        {
            LOG_ERROR("zmq recv msg error = %d",iRet);
            zmq_msg_close(&rcvMsg);
            return -1;
        }

        int msglen = zmq_msg_size(&rcvMsg);
        //todo zero copy
        if(rcvBuffer.iUsed < msglen)
        {
            rcvBuffer.Destroy();
            if(buffer.Create(msglen))
            {
                LOG_ERROR("msg buffer create error !");
                zmq_msg_close(&rcvMsg);
                return -1;
            }
        }        
        memcpy(rcvBuffer.pBuffer,zmq_msg_data(&rcvMsg),rcvBuffer.iCap);        
        zmq_msg_close(&rcvMsg);
        buffer = rcvBuffer;
        return 0;
    }
    int Create(int mode,void* ctx,const char* pszAddr,const char* name,int hwm = 1000)
    {
        char senderName[32];
        char receiverName[32];
        SNPRINTF(senderName,sizeof(senderName),"%s:snd:%d",name,mode);
        SNPRINTF(receiverName,sizeof(senderName),"%s:rcv:%d",name,mode);
        sender = receiver = NULL;
        /////////////////////////////////////////////////////////////////
        sender = zmq_socket(ctx,ZMQ_PUSH);                        
        if(!sender)
        {
            return -1;
        }
        //set name
        zmq_setsockopt(sender,ZMQ_IDENTITY,senderName,strlen(senderName));            
        //set hwm
        zmq_setsockopt(sender,ZMQ_SNDHWM,&hwm,sizeof(hwm));
        zmq_setsockopt(sender,ZMQ_RCVHWM,&hwm,sizeof(hwm));

        if(mode == CHANNEL_MODE_LOCAL && zmq_bind(ctx,pszAddr))
        {
            Destroy();
            return -2;
        }
        if(mode == CHANNEL_MODE_REMOTE && zmq_connect(ctx,pszAddr))
        {
            Destroy();
            return -2;
        }             
        //////////////////////////////////////////////////////////
        receiver = zmq_socket(ctx,ZMQ_PULL);                        
        if(!receiver)
        {
            Destroy();
            return -3;
        }
        //set name
        zmq_setsockopt(receiver,ZMQ_IDENTITY,receiverName,strlen(receiverName));            
        //set hwm
        zmq_setsockopt(receiver,ZMQ_SNDHWM,&hwm,sizeof(hwm));
        zmq_setsockopt(receiver,ZMQ_RCVHWM,&hwm,sizeof(hwm));
        if(mode == CHANNEL_MODE_LOCAL && zmq_bind(ctx,pszAddr))
        {
            Destroy();
            return -4;
        }            
        if(mode == CHANNEL_MODE_REMOTE && zmq_connect(ctx,pszAddr))
        {
            Destroy();            
            return -4;
        }   
        /////////////////////////////////////////////////////////////

        return 0;
    }
    void Destroy()
    {
        if(sender)
        {
            zmq_close(sender);
        }
        if(receiver)
        {
            zmq_close(receiver);
        }
        sender = NULL;
        receiver = NULL;
        mode = CHANNEL_MODE_INVALID;
    }
    ~Channel()
    {
        Destroy();
    }
};

class ChannelMessageHandler;

class ChannelAgent
{
public:
    ChannelAgent();
    virtual    ~ChannelAgent();
public:
    virtual    int    Init(void * ctx,int mode,const char * pszName,const char* pszAddr,ChannelMessageHandler * p);
public:
    //return 0 get a message , otherwise , return error code
    int GetMessage(ChannelMessage & msg);
    //return 0 is ok , otherwise return an error code
    int PostMessage(const ChannelMessage & msg);
    int DispatchMessage(const ChannelMessage & msg);
private:
    Channel     channel;    
    ChannelMessageHandler*  pHandler;
};

/////////////////////////////////////////////////////////////////////////////////////

class ChannelMessageHandler
{    
public:    
    virtual   int OnRecvMessage(const ChannelMessage & );
    virtual   ~ChannelMessageHandler();
public:
    void    SetAgent(ChannelAgent* p){m_pAgent = p;}
public:
    ChannelAgent*   m_pAgent;        
};


