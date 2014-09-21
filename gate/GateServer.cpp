#include "GateServerHandler.h"
#include "net/TcpServer.h"
#include "base/Log.h"
#include "component/IniConfigParser.h"
#include "utility/Daemon.h"
#include "GateConsoleHandler.h"
#include "net/UdpDriver.h"
#include "GateServerContext.h"
#include "channel/ChannelAgentMgr.h"

#include "app/App.hpp"

class GateServer : public App
{
public:
    //return 0 is ok , otherwise exit prcess
    virtual int     OnInit()
    {
        GateServerContext * ctx  = (GateServerContext*)GetContext();
        IniConfigParser & parser = ctx->parser;
        /////////////////////////////////////////////////////////////
        string sGateListenIP = parser.GetConfigString("gate:ip");        
        int port = parser.GetConfigInt("gate:port");    
        int iMaxClient = parser.GetConfigInt("gate:max_clients");
        bool bUseNagle = parser.GetConfigInt("gate:use_nagle")>0 ? true:false;
        
        const char* pszIP = sGateListenIP.c_str();
        //////////////////////////////////////////////////////////////
        int ret = 0;
        
        TcpServer &  server = ctx->gateServer;
        //port is 1234
        SockAddress addr(port,pszIP);
        LOG_INFO("gate server will listen on %s\n",addr.ToString());
        ret = server.Init(addr,bUseNagle,iMaxClient);
        if(ret)
        {
            LOG_FATAL("tcp server init error = %d",ret);
            return -1;
        }       
        
        TcpServerHandlerPtr  ptrHandler = TcpServerHandlerPtr(new GateServerHandler(&GetChannelProxy(),iMaxClient));
        server.SetServerHandler(ptrHandler.get());
        ret = server.Start();
        if(ret !=0 )
        {
            LOG_FATAL("tcp server start error  = %d!",ret);
            return -1;  
        }        
        return 0;
    }
    //control command process
    virtual string     OnCtrl(const std::vector<string> & cmdLine)
    {
        return "cmd";
    }
    //tick 
    virtual int     OnTick(int64_t lElapseTime)
    {
        return 0;
    }
    //poll system
    virtual int     OnPoll(int iRecommendPollNum = 1)
    { 
        //check server
        GateServerContext & ctx  = *(GateServerContext*)GetContext();
        int iProcMsg = ctx.gateServer.Loop(iRecommendPollNum);        
        if(!ctx.channels.empty())
        {
            iProcMsg |= ChannelAgentMgr::Instance().Polling(0);
        }
        return iProcMsg;
    }
    //system will close for closing Reason
    virtual int     OnClosing(int closingReason)
    {
        return 0;
    }
    //destroy sth uninitializing
    virtual int     OnDestroy()
    {
        return 0;
    }
};




/////////////////////////////////////////////////////////////
int main(int argc , char * argv[])
{
    return App::main<GateServerContext,GateServer>(argc,argv);    
}

