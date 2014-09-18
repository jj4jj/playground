#include "GateConsoleHandler.h"
#include "GateServerContext.h"
#include "base/Log.h"

#if 1
GateConsoleHandler::GateConsoleHandler(GateServerContext* p)
{
    ctx = p;
}
int GateConsoleHandler::SplitCMDLine(string & line,vector<string> & list,const char* pszDeli)
{
    list.clear();
    //placeholder
    line.append(pszDeli);
    int len = strlen(pszDeli);
    int start = 0;
    int end = line.find(pszDeli,start);
    while(start < (int)line.length() &&
          end != (int)string::npos )
    {
        string seg = line.substr(start,end-start);
        if(strcmp(seg.c_str(),pszDeli) && end > start)
        {
            list.push_back(seg); 
        }
        ///////////////////////////////////////////////
        start = end + len;
        end = line.find(pszDeli,start);
    }
    return 0;
}
string GateConsoleHandler::OnCMD(const std::vector<string> & cmdLine)
{
    if(cmdLine.empty())
    {
        return string("not comannds !");
    }
    //todo
    //echo
    char buffer[32];
    snprintf(buffer,sizeof(buffer),"echo:%d\n",(int)cmdLine.size());    
    string result = buffer;
    for(int i  = 0 ; i < (int)cmdLine.size(); ++i)
    {
        result +=  "[" + cmdLine[i] + "] ";
    }    
    return result;
}
int GateConsoleHandler::OnDataRecv(UdpSocket& udpSock,const Buffer & recvBuffer,const SockAddress& _addr)
{    
    SockAddress addr(_addr);
    char * p =  (char*)(recvBuffer.pBuffer);
    p[recvBuffer.iUsed] = 0;
    vector<string> sCMDLine;
    string line = p;
    SplitCMDLine(line,sCMDLine," ");
    string result = OnCMD(sCMDLine);
    udpSock.SendTo(Buffer(result.c_str()),addr);
    return 0;
}
#endif


