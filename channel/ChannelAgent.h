#pragma once
#include "zmq.h"
#include "base/Buffer.h"
#include "base/CommonMacro.h"

struct  ChannelMessage
{
    uint32_t    dwSize;
    uint8_t *   pData;
public:
    ChannelMessage():dwSize(0),pData(NULL){}
    ChannelMessage(const Buffer & buff):
            dwSize(buff.iUsed),
            pData(buff.pBuffer)
    {           
    }
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
    Channel();
    ~Channel();
public:
    int Write(const Buffer & buffer);
    int Read(Buffer & buffer);
    int Create(int mode,void* ctx,const char* pszAddr,const char* name,int hwm = 1000);
    void Destroy();
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


