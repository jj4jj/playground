#include "net/TcpServer.h"
#include "GateChannelProxy.h"
#include "base/Log.h"
#include "component/IniConfigParser.h"
////////////////////////////////
#include "GateServerContext.h"


#if 1
int    GateServerContext::Init(const char * pszConfigFile)
{

    return 0;
}
int    GateServerContext::SetServer(TcpServer* pServer)
{

    gateServer = pServer;
    return 0;
}
#endif



