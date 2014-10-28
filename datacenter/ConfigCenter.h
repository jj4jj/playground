#pragma once
#include "base/Singleton.hpp"
#include "base/stdinc.h"
#include "base/Buffer.h"

struct redisContext ;
struct redisReply ;

class ConfigCenter : public Singleton < ConfigCenter >
{
private:    
    ConfigCenter();
    ~ConfigCenter();
    DeclareSingltonSupport(ConfigCenter)
public:
    //1.5
    int     Init(const char* pszHostName = "127.0.0.1",int port = 6379,int timeout = 1500);
    void    Destroy();
    int     HeartBeat();
    const   char* GetConfig(const char* pszKey,const char* pszConfigName = "");    
    int     SetConfig(const char* pszKey,const char* pszValue,const char* pszConfigName = "");    
    int     SetConfigInt(const char* pszKey,int value,const char* pszConfigName = "");
    int     GetConfigInt(const char* pszKey,int defaultvalue = 0,const char* pszConfigName = "");
    int     RemoveConfig(const char* pszKey,const char* pszConfigName = "");
private:    
    redisContext * m_ctx;
    redisReply * m_reply;    
    Buffer       m_buffer;
};




