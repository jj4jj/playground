#include "app/App.hpp"

struct  AgentContext : AppContext
{
public:    
    virtual void    OnGenerateDefaultConfig()
    {
    }
    virtual int     OnInit()
    {
        return 0;
    }
};
class AgentApp : public App
{

private:
    private:
    AgentApp(){}
    ~AgentApp(){}
    DeclareSingltonSupport(AgentApp)
        
public:
    //return 0 is ok , otherwise exit prcess
    virtual int     OnInit()
    {
        //log

        //mysql

        //redis

        //channel proxy

        //3rd platform service

        //rank
        
        //table

        //script

        return 0;        
    }
    //control command process
    virtual string  OnCtrl(const std::vector<string> & cmdLine)
    {
        string recvCMDS;
        for(uint i = 0;i < cmdLine.size(); ++i)
        {
            if(i == 1)
            {
                recvCMDS += " : ";
            }
            else if(i > 1)
            {
                recvCMDS += " & ";
            }
            recvCMDS += cmdLine[i];
        }
        LOG_INFO("recv cmd [%s]",recvCMDS.c_str());
        return string("todo");
    }
    //tick 
    virtual int     OnTick(int64_t lElapseTime)
    {
        //sth need tick
        return 0;
    }
    //poll system
    virtual int     OnPoll(int iRecommendPollNum = 1)
    {
        //datagent

        
        
        return 0;
    }
    //system will close for closing Reason
    virtual int     OnClosing(int closingReason)
    {
        LOG_INFO("agent is closing for reason = %d ...",closingReason);
        if(closingReason == 1)
        {
            //restart 
            //backup memory 
            return 0;
        }
        else
        {
            return 0;
        }
    }
    //destroy sth uninitializing
    virtual int     OnDestroy()
    {
        //sth need close .
        return 0;
    }    
};
typedef Singleton<AgentApp>  AgentServer;
///////////////////////////////////////////////////////////


DeclareAppMain(AgentContext,AgentApp)
/*
App* GetApp()
{
    return AgentServer::instance();
}
///////////////////////////////////////////////////////////
int main(int argc,char* argv[])
{
    return    App::main<AgentContext>(argc,argv);
}
*/


