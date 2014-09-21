#pragma once
#include "base/Buffer.h"
#include "base/CommonMacro.h"
#include "ChannelMessage.h"
#include "ChannelMessageHandler.h"
#include "Channel.h"


class ChannelAgent
{
public:
    ChannelAgent(int _id);
    virtual    ~ChannelAgent();
public:
    virtual    int    Init(void * ctx,int mode,const char * pszName,const char* pszAddr);
public:
    inline int GetID(){return id;}
    //return 0 get a message , otherwise , return error code
    int GetMessage(ChannelMessage & msg);
    //return 0 is ok , otherwise return an error code
    int PostMessage(const ChannelMessage & msg);
    inline Channel & GetChannel(){return channel;}
private:
    int         id;
    Channel     channel;    
};

/////////////////////////////////////////////////////////////////////////////////////


