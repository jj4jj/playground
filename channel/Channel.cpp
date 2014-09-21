#include "base/stdinc.h"
#include "base/Log.h"
#include "base/CommonMacro.h"



///////////////////////////////////
#include "Channel.h"

#if 1
Channel::Channel():mode(CHANNEL_MODE_INVALID),sender(NULL),receiver(NULL)
{
}
Channel::~Channel()
{
    Destroy();
}
#endif

#if 1
int  Channel::Write(const Buffer & buffer)
{
    zmq_msg_t   sndMsg;
    zmq_msg_init(&sndMsg);
    zmq_msg_init_size(&sndMsg,buffer.iUsed);
    memcpy(zmq_msg_data(&sndMsg),buffer.pBuffer,buffer.iUsed);
    int iRet = zmq_msg_send(&sndMsg,sender,0);
    zmq_msg_close(&sndMsg);
    if(iRet < 0)
    {
        LOG_ERROR("zmq send msg error = %d for = %s",iRet,zmq_strerror(iRet));
        return -1;
    }
    return 0;
}
int  Channel::Read(Buffer & buffer)
{
    zmq_msg_t   rcvMsg;
    zmq_msg_init(&rcvMsg);        
    int iRet = zmq_msg_recv(&rcvMsg,receiver,0);
    if(iRet < 0 &&
       iRet != EINTR )
    {
        LOG_ERROR("zmq recv msg error = %d for = %s",iRet,zmq_strerror(iRet));
        zmq_msg_close(&rcvMsg);
        return -1;
    }

    int msglen = zmq_msg_size(&rcvMsg);
    //todo zero copy
    if(rcvBuffer.iCap < msglen)
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
    rcvBuffer.iUsed = msglen;

    zmq_msg_close(&rcvMsg);
    buffer = rcvBuffer;
    return 0;
}
int Channel::Create(int mode,void* ctx,const char* pszAddr,const char* name,int hwm )
{
    char senderName[32];
    char receiverName[32];
    assert(strlen(name) < 24);
    SNPRINTF(senderName,sizeof(senderName),"%s:snd:%d",name,mode);
    SNPRINTF(receiverName,sizeof(senderName),"%s:rcv:%d",name,mode);
    sender = receiver = NULL;
    /////////////////////////////////////////////////////////////////
    sender = zmq_socket(ctx,ZMQ_PUSH);                        
    if(!sender)
    {
        LOG_ERROR("zmq socket error  error = %s",zmq_strerror(zmq_errno()));
        return -1;
    }
    //set name
    zmq_setsockopt(sender,ZMQ_IDENTITY,senderName,strlen(senderName));            
    //set hwm
    zmq_setsockopt(sender,ZMQ_SNDHWM,&hwm,sizeof(hwm));
    zmq_setsockopt(sender,ZMQ_RCVHWM,&hwm,sizeof(hwm));

    int zret = 0;
    if(mode == CHANNEL_MODE_LOCAL && (zret = zmq_bind(sender,pszAddr)))
    {
        LOG_ERROR("zmq bind = %d error = %s",zret,zmq_strerror(zmq_errno()));
        Destroy();
        return -2;
    }
    if(mode == CHANNEL_MODE_REMOTE && (zret = zmq_connect(sender,pszAddr)))
    {
        LOG_ERROR("zmq connect = %d error = %s",zret,zmq_strerror(zmq_errno()));
        Destroy();
        return -2;
    }             
    //////////////////////////////////////////////////////////
    receiver = zmq_socket(ctx,ZMQ_PULL);                        
    if(!receiver)
    {
        LOG_ERROR("zmq socket error  error = %s",zmq_strerror(zmq_errno()));
        Destroy();
        return -3;
    }
    //set name
    zmq_setsockopt(receiver,ZMQ_IDENTITY,receiverName,strlen(receiverName));            
    //set hwm
    zmq_setsockopt(receiver,ZMQ_SNDHWM,&hwm,sizeof(hwm));
    zmq_setsockopt(receiver,ZMQ_RCVHWM,&hwm,sizeof(hwm));

    zret = zmq_connect(receiver,pszAddr);
    if(zret)
    {
        LOG_ERROR("zmq connect = %d error = %s",zret,zmq_strerror(zmq_errno()));
        Destroy();            
        return -4;
    }   
    /////////////////////////////////////////////////////////////

    return 0;
}
void  Channel::Destroy()
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
    LOG_DEBUG("close channel");
}

#endif




