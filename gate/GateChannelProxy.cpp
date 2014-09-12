#include "GateChannelProxy.h"
#include "base/Log.h"
#include "channel/ChannelAgentMgr.h"

#if 1

int     GateChannelProxy::Init()
{
    //todo
    //build all dst chennel , map id
    ChannelAgentMgr::Instance().Init();
    return 0;
}
int         GateChannelProxy::SendToAgent(int iDst,const std::vector<Buffer>  &  vBuff)
{
    int iLength = 0;
    for(int i = 0;i < (int)vBuff.size(); ++i)
    {
        iLength += vBuff[i].iUsed;
    }   
    if(iLength > 0x7FFF)
    {
        LOG_ERROR("Buffer size is too much = %d",iLength);
        return -1;
    }
    /*
    uint16_t   nLen = htons((uint16_t)iLength);
    ChannelAgent  * pChannel = ChannelAgentMgr::Instance().GetChannel(iDst);
    if(pChannel)
    {
        LOG_ERROR("channel agent is not ready !");
        return -1;
    }
    //////////////////////////////////////////////
    //buffer 
    //len
    //msg
        //ga_connection
        //data
    if(pChannel->GetAvaileSize() < iLength)
    {
        LOG_ERROR("channel has no more space");
        return -1;
    }
    pChannel->Write(Buffer((char*)&nLen,sizeof(nLen)));
    for(int i  = 0;i < vBuff.size(); ++i)
    {
        pChannel->Write(vBuff[i]);
    }
    */
    return 0;
}
int      GateChannelProxy::SendToAgent(int iDst,const Buffer & buff)
{
    vector<Buffer>  v;
    v.push_back(buff);
    return SendToAgent(iDst,v);
}

#endif

