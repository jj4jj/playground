
#include "channel/ChannelMessage.h"
#include "channel/ChannelMessageHandler.h"
#include "channel/ChannelAgent.h"
#include "channel/ChannelAgentMgr.h"
#include "base/Log.h"
static int channel_id = 0;
class TestChannelHandler : public  ChannelMessageHandler
{
public:    
    virtual   int OnRecvMessage(const ChannelMessage & msg)
    {
        printf("recv channel message size = %u\n",msg.dwSize);
        return 0;
    }
};
void connect()
{
    ChannelAgentMgr::Instance().AddChannel(channel_id,true,"test","tcp://127.0.0.1:58850",new TestChannelHandler);
}
void listen()
{
    ChannelAgentMgr::Instance().AddChannel(channel_id,false,"test","tcp://127.0.0.1:58850",new TestChannelHandler);
}
void close()
{
    ChannelAgentMgr::Instance().RemoveChannel(channel_id);
}
void write()
{
    ChannelAgent * p =  ChannelAgentMgr::Instance().GetChannel(channel_id);
    if(!p)
    {
        return ;
    }
    ChannelMessage msg;
    Buffer bf("channel message write");
    p->PostMessage(ChannelMessage(bf));
}
void read()
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
        void (*pfn)();
    } options[] = {
        {"listen",listen},
        {"connect",connect},
        {"close",close},
        {"write",write},
        {"read",read},
    };

    if(ChannelAgentMgr::Instance().Init())
    {
        return -1;
    }    
    char line[128];
    while(1)
    {
        ChannelAgentMgr::Instance().Polling();
        printf(">>");
        scanf("%s",line);
        bool bexe = false;
        for(int i = 0; i < (int)(sizeof(options)/sizeof(options[0]));++i)
        {
            if(strcmp(options[i].cmd,line) == 0 )
            {
                options[i].pfn();
                bexe = true;
                break;
            }
        }        
        if(0 == strcmp("quit",line))
        {
            return 0;
        }
        else if(!bexe)
        {
            printf("not found cmd = [%s]\n",line);
        }
    }
    return 0;
}

