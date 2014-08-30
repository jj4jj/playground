#pragma once


struct ChannelMessage : public Buffer
{
    uint8_t* GetMsgPtr();
    int GetMsgLen();
};


class ChannelAgent
{
public:
    int GetMessage(ChannelMessage & msg);
    int PostMessage(const ChannelMessage & msg);
    
public:
    int GetID();
    int Write(const Buffer  & buffer);
    int Read(Buffer & buffer);
    int GetReadableSize();//read space
    int GetWriteableSize();//for write
private:
    //
    
    
};


class ChannelMessageHandler
{
public:
    int       Init(ChannelAgent * pAgent);
    virtual   int OnRecvMessage();
public:
    ChannelAgent*   m_pAgent;        
};


