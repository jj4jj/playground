#pragma once
#include "base/Buffer.h"
#include "base/CommonMacro.h"
#include "ChannelMessage.h"
#include "Channel.h"

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


