
#include "base/stdinc.h"
#include "base/CommonMacro.h"
#include "base/Log.h"
#include "ChannelMessageDispatcher.h"
#include "ChannelAgent.h"

#if 1
ChannelAgent::ChannelAgent(int _id):id(_id),channel(this)
{
    szName[0]=0;
}
ChannelAgent::~ChannelAgent()
{
    LOG_DEBUG("channel agent destroy");
}
#endif

#if 1
int        ChannelAgent::Init(void * ctx,int mode,const char * pszName,const char* pszAddr)
{
    int iRet = channel.Create(mode, ctx, pszAddr,pszName);
    if(iRet)
    {
        LOG_ERROR("channel create error = %d",iRet);
        return -1;
    }
    LOG_INFO("create channel id = %d name = %s ok .",id,szName);
    return 0;
}

//return 0 get a message , otherwise , return error code
int ChannelAgent::GetMessage(ChannelMessage & msg)
{
    //receive a message
    Buffer buff;
    if(channel.Read(msg.iSrc,msg))
    {
        return -1;
    }
    return 0;    
}
//return 0 is ok , otherwise return an error code
int ChannelAgent::PostMessage(const ChannelMessage & msg)
{
    //post the message to channel    
    Buffer buff((char*)msg.pData,msg.dwSize);
    return channel.Write(buff);
}
#endif



/////////////////////////////////////////////////











