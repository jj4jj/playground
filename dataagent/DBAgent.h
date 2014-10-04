#pragma once 
#include "base/stdinc.h"
#include "base/Buffer.h"


class DBAgent
{
public:
    int  Get(string setName,void * obj,const Buffer & cb);
    int  Remove(string setName,void * obj,const Buffer & cb);
    int  Insert(string setName,void * obj,const Buffer &  cb);        
    int  Update(string setName,void * obj,const Buffer &  cb);   
};
