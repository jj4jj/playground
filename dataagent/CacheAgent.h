#pragma once

#include "base/stdinc.h"
#include "base/Buffer.h"

struct CacheAgentOption
{


};

class MemSerializer;

class CacheAgent
{
public:
    int  Init(const CacheAgentOption  & cao,MemSerializer * seri);
public:
    int  Get(string setName,void * obj,const Buffer & cb);
    int  Remove(string setName,void * obj,const Buffer & cb);
    int  Insert(string setName,void * obj,const Buffer &  cb);       
    int  Update(string setName,void * obj,const Buffer &  cb);   
private:
    MemSerializer   *   serializer;
    //redis batch client
    
};
