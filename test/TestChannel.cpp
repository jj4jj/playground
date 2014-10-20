#include "channel/ChannelMessage.h"
#include "channel/ChannelMessageDispatcher.h"
#include "channel/ChannelAgent.h"
#include "channel/ChannelAgentMgr.h"
#include "base/Log.h"
static int channel_id = 1;
const char* pszAddr1 = "tcp://127.0.0.1:13672";
const char* pszAddr2 = "tcp://127.0.0.1:13673";
const char* pszName = "test";

class TestChannelHandler : public  ChannelMessageDispatcher
{
public:    
    virtual   int DispatchMessage(ChannelAgent & agent,const ChannelMessage & msg)
    {
        LOG_INFO("recv channel = %d src = %d message size = %u\n",agent.GetID(),msg.iSrc,msg.dwSize);        
        LOG_INFO("msg = [%s]",(char*)msg.pData);        
        return 0;
    }
};
void connect(int sel)
{
    if(1 == sel)
    {
        ChannelAgentMgr::Instance().AddChannel(channel_id,false,pszName,pszAddr1);
    }
    else
    {
        ChannelAgentMgr::Instance().AddChannel(channel_id,false,pszName,pszAddr2);
    }
}
void Close(int local)
{
    //ChannelAgentMgr::Instance().RemoveChannel(channel_id);
}
void write()
{
    ChannelAgent * p =  ChannelAgentMgr::Instance().GetChannel(channel_id);
    if(!p)
    {
        return ;
    }
    ChannelMessage msg;
    Buffer bf("channel message write test");
    p->PostMessage(ChannelMessage(bf));
}
void read(int local)
{
    ChannelAgent * p =  ChannelAgentMgr::Instance().GetChannel(channel_id);
    if(!p)
    {
        return ;
    }
    ChannelMessage msg;
    p->GetMessage(msg);
    LOG_INFO("recv message = %s size = %d",msg.pData,msg.dwSize);    
}
int main(int argc , char* argv[])
{
    /*
    printf("usage : %s <cmd>\n",argv[0]);
    puts("cmd\n\tlisten\t\t|\tcreate a channel name is test name is 'test' , liten on tcp://127.0.0.1:58850");
    puts("\tconnect\t\t|\tconnect to the channel");
    puts("\tclose\t\t|\tclose the channel");
    puts("\tread\t\t|\tread channel message from channel");
    puts("\twrite\t\t|\twrite message  to the channel");
    puts("\tquit\t\t|\tquit the console");

    struct 
    {
        const char* cmd;
        void (*pfn)(int);
    } options[] = {
        {"listen",listen},
        {"connect",connect},
        {"close",Close},
        {"write",write},
        {"read",read},
    };
    */
    ChannelMessageDispatcherPtr hdr(new TestChannelHandler());
    if(argc < 2)
    {
        return -1;
    }
    

    if(argv[1][0] == 'w')
    {
        if(ChannelAgentMgr::Instance().Init("ch",pszAddr2,hdr,2))
        {
            return -1;
        }    
        connect(1);
        write();
    }
    else
    {
        if(ChannelAgentMgr::Instance().Init("ch",pszAddr1,hdr,2))
        {
            return -1;
        }    
        connect(2);
    }

    /////////////////////////////////////////////////////////////////////////////////    
    while(true)
    {
        ChannelAgentMgr::Instance().Polling();        
    }    
    return 0;
}

