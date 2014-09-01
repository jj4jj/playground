#pragma once
#include "zmq.h"


struct  ChannelMessage
{
    uint32_t    dwSize;
    uint8_t *   pData;
};

struct Channel
{
    int   id;
    void* sender;
    void* receiver;    
};

class ChannelMessageHandler;

class ChannelAgent
{
public:
    ChannelAgent(){}
    virtual    ~ChannelAgent();
public:
    virtual    int    Init(ChannelMessageHandler * p){channel.Init();pHandler = p;return 0;}
public:
    int        CreateChannel(int id,void* ctx,
                    const char* pszLocalAddr,const char* pszRemoteAddr)
    {
        void* receiver = NULL,sender = NULL; 
        receiver = zmq_socket(ctx,zmq::ZMQ_PULL);
        zmq_bind(receiver ,pszLocalAddr);
        if(!receiver)
        {
            LOG_FATAL("receiver bind %s error !",pszLocalAddr);
            return -2;
        }        
        sender = zmq_socket(ctx,zmq::ZMQ_PUSH);
        zmq_bind(sender,pszRemoteAddr);
        if(!sender)
        {
            LOG_FATAL("sender bind %s error !",pszLocalAddr);
            zmq_close(receiver);
            return -3;
        }        
        channel.id = id;
        channel.sender = sender;
        channel.receiver = receiver;
        return 0;
    }
    void       DestroyChannel()
    {
        LOG_INFO("destroy channel id = %d",channel.id);
        zmq_close(channel.sender);
        zmq_close(channel.sender);
        channel.id = -1;
        channel.sender = NULL;
        channel.receiver = NULL;
    }
    //return 0 get a message , otherwise , return error code
    int GetMessage(ChannelMessage & msg)
    {
        //receive a message
        
    }
    //return 0 is ok , otherwise return an error code
    int PostMessage(const ChannelMessage & msg)
    {
        //post the message to channel
    }
    //
    inline int DispatchMessage(const ChannelMessage & msg)
    {
        return pHandler->OnRecvMessage(msg);
    }

private:
    Channel     channel;    
    ChannelMessageHandler*  pHandler;
};


class ChannelMessageHandler
{
public:    
    virtual   int OnRecvMessage(const ChannelMessage & );
public:
    void    SetAgent(ChannelAgent* p){m_pAgent = p;}
public:
    ChannelAgent*   m_pAgent;        
};


