#pragma once

#include "base/Buffer.h"
#include "zmq.h"

class ChannelAgent;
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
    Channel(ChannelAgent* _agent);
    ~Channel();
public:
    int Write(const Buffer & buffer);
    int Read(int & rcvMsgID,ChannelMessage & msg);
    int Create(int mode,void* ctx,const char* pszAddr,const char* name,int hwm = 1000);
    void Destroy();
private:
    ChannelAgent*   agent;
    zmq_msg_t       msg_head;
};

