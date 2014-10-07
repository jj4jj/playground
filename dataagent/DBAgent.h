#pragma once 
#include "base/stdinc.h"
#include "base/Buffer.h"


class DBAgent
{
public:
    int  Get(void * obj,const Buffer & cb);
    int  Remove(void * obj,const Buffer & cb);
    int  Insert(void * obj,const Buffer &  cb);        
    int  Update(void * obj,const Buffer &  cb);   
};


