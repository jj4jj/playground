
#include "base/stdinc.h"
#include "base/Log.h"
#include "ChannelAgent.h"

#if 1
ChannelAgent::ChannelAgent()
{

}
ChannelAgent::~ChannelAgent()
{

}
#endif

#if 1
int        ChannelAgent::Init(void * ctx,int mode,const char * pszName,const char* pszAddr,ChannelMessageHandler * p)
{
    int iRet = channel.Create(mode, ctx, pszAddr,pszName);
    if(iRet)
    {
        LOG_ERROR("channel create error = %d",iRet);
        return -1;
    }
    pHandler = p;
    return 0;
}

//return 0 get a message , otherwise , return error code
int ChannelAgent::GetMessage(ChannelMessage & msg)
{
    //receive a message
    Buffer buff;
    if(channel.Read(buff))
    {
        return -1;
    }
    msg.pData = buff.pBuffer;
    msg.dwSize = buff.iUsed;
    return 0;    
}
//return 0 is ok , otherwise return an error code
int ChannelAgent::PostMessage(const ChannelMessage & msg)
{
    //post the message to channel    
    Buffer buff((char*)msg.pData,msg.dwSize);
    return channel.Write(buff);
}
int ChannelAgent::DispatchMessage(const ChannelMessage & msg)
{
    return pHandler->OnRecvMessage(msg);
}
#endif



/////////////////////////////////////////////////

#if 1
ChannelMessageHandler::~ChannelMessageHandler()
{
    m_pAgent = NULL;    
}
#endif

#if 1 
int ChannelMessageHandler::OnRecvMessage(const ChannelMessage & msg )
{
    LOG_INFO("recv a channel msg size = %u!",msg.dwSize);
    return -1;
}
#endif








#if 1
Channel::Channel():mode(CHANNEL_MODE_INVALID),sender(NULL),receiver(NULL)
{
}
int  Channel::Write(const Buffer & buffer)
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
int  Channel::Read(Buffer & buffer)
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
int Channel::Create(int mode,void* ctx,const char* pszAddr,const char* name,int hwm )
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
}
Channel::~Channel()
{
    Destroy();
}

#endif









