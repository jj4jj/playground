#pragma once
#include "base/stdinc.h"
#include "base/Buffer.h"

struct redisReply;
class DataListener
{
public:
    virtual ~DataListener(){}
public :  
    //DataSetListener
    ////////////////////////////cache & db//////////////////////////////////
    virtual   int   OnGet(int ret,void* obj,Buffer & cb);
    virtual   int   OnInsert(int ret,Buffer & cb);
    virtual   int   OnRemove(int ret,Buffer & cb);
    virtual   int   OnUpdate(int ret,Buffer & cb);
    
};
typedef shared_ptr<DataListener>    DataListenerPtr;


