#pragma once

struct MysqlResponse;
class MysqlCommandListener
{
public:
    MysqlCommandListener(){}
    virtual ~MysqlCommandListener(){}
public:            
    virtual  int     OnResponse(MysqlResponse & rsp);
};
typedef shared_ptr<MysqlCommandListener>  MysqlCommandListenerPtr;
////////////////////////////////////////////////////////////

