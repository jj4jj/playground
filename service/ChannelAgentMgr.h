#pragma once

class ChannelAgentMgr
{
public:
    int           Init(const SockAddress & bindAddr,int iMaxChannel);
    ChannelAgent* CreateChannel(int id,const SockAddress & peer);
    int           DestroyChannel(ChannelAgent* agent);
    //recv msg , send msg
    int           UpdateTick();

};



