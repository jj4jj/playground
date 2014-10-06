#pragma once
#include "base/stdinc.h"
#include "base/Buffer.h"

class DataListener
{
public:
    virtual ~DataListener(){}
public :  
    //DataSetListener
    ///////////////////////////////////////////////////////////////
    virtual   int   OnInsert(int ret, const Buffer & cb);
    virtual   int   OnRemove(int ret,const Buffer & cb);
    virtual   int   OnFind(int ret,void* entry,const Buffer & cb);
    virtual   int   OnUpdate(int ret,const Buffer & cb);
};
typedef shared_ptr<DataListener>    DataListenerPtr;


