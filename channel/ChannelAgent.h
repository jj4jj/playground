#pragma once
#include "base/Buffer.h"
#include "base/CommonMacro.h"
#include "ChannelMessage.h"
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
    inline const char* GetBindName(){return szName;}
    void    SetBindName(const char* szName_){strncpy(szName,szName_,sizeof(szName)-1);}
    inline const char* GetIDName(){return szIDName;}
    void    SetIDName(const char* szName_){strncpy(szIDName,szName_,sizeof(szName)-1);}
    
    //return 0 get a message , otherwise , return error code
    int GetMessage(ChannelMessage & msg);
    //return 0 is ok , otherwise return an error code
    int PostMessage(const ChannelMessage & msg);
    inline Channel & GetChannel(){return channel;}
private:
    int         id;
    char        szName[128];
    char        szIDName[128];
    Channel     channel;    
};

/////////////////////////////////////////////////////////////////////////////////////


