#include "base/stdinc.h"
#include "base/Log.h"
#include "base/CommonMacro.h"
#include "ChannelAgent.h"
#include "ChannelAgentMgr.h"
#include "base/StringUtil.h"

///////////////////////////////////
#include "Channel.h"

#if 1
Channel::Channel(ChannelAgent * _agent):mode(CHANNEL_MODE_INVALID),
    sender(NULL),receiver(NULL),agent(_agent)
{
    zmq_msg_init(&msg_head);
    seqence = 0L;
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
    int part1 = 0,part2 = 0;
    

    if(0 == zmq_msg_size(&msg_head))
    {
        char szHeadMsg[64];
        snprintf(szHeadMsg,sizeof(szHeadMsg),"%s|%d",agent->GetIDName(),seqence);            
        int len = strlen(szHeadMsg)+1;
        zmq_msg_init_size(&msg_head,len);
        memcpy(zmq_msg_data(&msg_head),szHeadMsg,len);
        part1 = len;
    }

    //head:channel id
    int iRet = zmq_msg_send(&msg_head,sender,ZMQ_SNDMORE|ZMQ_DONTWAIT);    
    if(iRet < 0)
    {
        LOG_ERROR("zmq send msg head error = %d for = %s",iRet,zmq_strerror(iRet));
        return -1;
    }
    zmq_msg_init(&sndMsg);
    zmq_msg_init_size(&sndMsg,buffer.iUsed);
    memcpy(zmq_msg_data(&sndMsg),buffer.pBuffer,buffer.iUsed);
    part2 = buffer.iUsed;
    iRet = zmq_msg_send(&sndMsg,sender,ZMQ_DONTWAIT);
    zmq_msg_close(&sndMsg);
    if(iRet < 0)
    {
        LOG_ERROR("zmq send msg error = %d for = %s",iRet,zmq_strerror(iRet));
        return -2;
    }
    LOG_DEBUG("channel msg write ok id = %d part1 = %d part2 = %d seqence = %d iret = %d!",
        agent->GetID(),part1,part2,seqence,iRet);
    ++seqence;
    return 0;
}
int  Channel::Read(int & rcvMsgID,ChannelMessage & msg)
{
    rcvMsgID = -1;
    zmq_msg_t   rcvMsg;
    zmq_msg_init(&rcvMsg);    
    int iRet = 0;
    int part1 = 0, part2 = 0;

    //head part
    iRet = zmq_msg_recv(&rcvMsg,receiver,ZMQ_DONTWAIT);
    if( iRet < 0)
    {
        LOG_ERROR("zmq recv msg error = %d for = %s",iRet,zmq_strerror(zmq_errno()));
        return -1;
    }  
    part1 = zmq_msg_size(&rcvMsg);
    char* pszPart1Msg = (char*)zmq_msg_data(&rcvMsg);
    int nums[4] = {-1,-1,-1,-1};
    string heads = pszPart1Msg;
    vector<string>  vecheads = StringUtil::SplitString(heads,"|");
    string chname =  vecheads[0];
    msg.seqence = rcvMsgID = strtol(vecheads[1].c_str(),NULL,10);
    msg.iSrc = ChannelAgentMgr::Instance().GetChannelID(chname);
    zmq_msg_close(&rcvMsg);

    //------------------------------------------------------------------------------

    int rcvmore = 0;
    size_t more_size = sizeof(rcvmore);
    iRet = zmq_getsockopt (receiver, ZMQ_RCVMORE, &rcvmore,&more_size);
    if(0 != iRet || 0 == rcvmore)
    {
        LOG_ERROR("zmq recv msg id = %d has no part msg !",rcvMsgID);
        return -1;
    }
    zmq_msg_init(&rcvMsg);
    iRet = zmq_msg_recv(&rcvMsg,receiver,ZMQ_DONTWAIT);
    if(iRet < 0 )
    {
        LOG_ERROR("zmq recv msg error = %d for = %s",iRet,zmq_strerror(zmq_errno()));
        return -1;
    } 
    int msglen = zmq_msg_size(&rcvMsg);
    part2 = msglen;
    //todo zero copy
    if(rcvBuffer.iCap < msglen)
    {
        rcvBuffer.Destroy();
        if(rcvBuffer.Create(msglen))
        {
            LOG_ERROR("msg buffer create error !");
            zmq_msg_close(&rcvMsg);
            return -1;
        }
    }        
    memcpy(rcvBuffer.pBuffer,zmq_msg_data(&rcvMsg),msglen);        
    rcvBuffer.iUsed = msglen;
    zmq_msg_close(&rcvMsg);
    
    msg.dwSize = rcvBuffer.iUsed;
    msg.pData = rcvBuffer.pBuffer;

    LOG_DEBUG("channel msg read ok id = %d part1 = %d part2 = %d seqence = %d iret = %u!",
        msg.iSrc,part1,part2,msg.seqence,iRet);
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
    int zret = 0;
    /////////////////////////////////////////////////////////////////
    if(CHANNEL_MODE_LOCAL == mode )    
    {
        //listen
        //////////////////////////////////////////////////////////
        receiver = zmq_socket(ctx,ZMQ_PULL);                        
        if(!receiver)
        {
            LOG_ERROR("zmq socket error  error = %s",zmq_strerror(zmq_errno()));
            Destroy();
            return -1;
        }
        //set name
        zmq_setsockopt(receiver,ZMQ_IDENTITY,receiverName,strlen(receiverName));            
        //set hwm
        //zmq_setsockopt(receiver,ZMQ_SNDHWM,&hwm,sizeof(hwm));
        zmq_setsockopt(receiver,ZMQ_RCVHWM,&hwm,sizeof(hwm));

        zret = zmq_bind(receiver,pszAddr);
        if(zret)
        {
            LOG_ERROR("zmq connect = %d error = %s",zret,zmq_strerror(zmq_errno()));
            Destroy();            
            return -2;
        }  
        agent->SetBindName(receiverName);
    }
    else
    {
        sender = zmq_socket(ctx,ZMQ_PUSH);                        
        if(!sender)
        {
            LOG_ERROR("zmq socket error  error = %s",zmq_strerror(zmq_errno()));
            return -3;
        }
        //set name
        zmq_setsockopt(sender,ZMQ_IDENTITY,senderName,strlen(senderName));            
        //set hwm
        //ZMQ_RECONNECT_IVL_MAX

        int reconnect_interval = 400; //400 ms
        zmq_setsockopt(sender,ZMQ_RECONNECT_IVL,&reconnect_interval,sizeof(reconnect_interval));
        int reconnect_interval_max = 30000;//30s
        zmq_setsockopt(sender,ZMQ_RECONNECT_IVL_MAX,&reconnect_interval_max,sizeof(reconnect_interval_max));

        zmq_setsockopt(sender,ZMQ_SNDHWM,&hwm,sizeof(hwm));
        //zmq_setsockopt(sender,ZMQ_RCVHWM,&hwm,sizeof(hwm));

        zret = zmq_connect(sender,pszAddr);
        if(zret)
        {
            LOG_ERROR("zmq connect = %d error = %s",zret,zmq_strerror(zmq_errno()));
            Destroy();
            return -4;
        }
        agent->SetBindName(senderName);
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
    zmq_msg_close(&msg_head);
}

#endif




