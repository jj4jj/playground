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
    //////////////////////////////////////////////////////////////////
    virtual   int   OnGet(redisReply* reply,Buffer & cb,bool timeout);
    virtual   int   OnInsert(redisReply* reply,Buffer & cb,bool timeout);
    virtual   int   OnRemove(redisReply* reply,Buffer & cb,bool timeout);
    virtual   int   OnUpdate(redisReply* reply,Buffer & cb,bool timeout);
 };
typedef shared_ptr<DataListener>    DataListenerPtr;


