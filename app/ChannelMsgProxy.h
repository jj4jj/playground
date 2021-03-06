#pragma once
#include "base/Buffer.h"
#include "ChannelProxyDispatcher.h"
//just wrap channel 
//send dst id to a channel 
//init all channels

struct AppContext;
class ChannelMsgProxy
{
public:
    typedef unordered_map<int,ChannelMessageDispatcherPtr>    ChannelProxyMsgHandlerMap;
    typedef std::pair<int,int>  ChannelProxyMsgHandlerTableSegment;
    typedef std::pair<ChannelProxyMsgHandlerTableSegment , ChannelMessageDispatcherPtr> ChannelProxyMsgHandlerPair;
    typedef vector<ChannelProxyMsgHandlerPair> ChannelProxyMsgHandlerTable;
public:
    int      Init(AppContext * pCtx);
    int      Polling(int iPollTimeOutMs = 10);
    void     Destroy();
public:  
    int      SubscribeSingleMsg(int id,ChannelMessageDispatcherPtr hanlder);
    int      SubscribeContinuousMsg(int fromid , int endid,ChannelMessageDispatcherPtr hanlder);
    int      SubscribeScatterMsg(std::vector<int> & ids,ChannelMessageDispatcherPtr hanlder);
    ChannelMessageDispatcher *   FindHandler(int id);
    bool     MatchSegment(ChannelProxyMsgHandlerTableSegment & seg , int id);
    int      SendToAgent(int iDst,const std::vector<Buffer>  &  vBuff,int iHeadSize = 0);
    int      SendToAgent(int iDst,const Buffer &  buff,int iHeadSize = 0);
public:
    ChannelMsgProxy();
    virtual ~ChannelMsgProxy();

private:
    AppContext * m_pCtx;
    Buffer chnMsgSendBuffer;
    ChannelProxyMsgHandlerMap       m_mpHandlerMap; 
    ChannelProxyMsgHandlerTable     m_vecHandlers;
public:
    enum
    {
        PROXY_CHANNEL_MESSAGE_HEAD_SIZE =   sizeof(uint16_t),
        MAX_CHANNEL_MESSAGE_SIZE = 65536,
    };
};

