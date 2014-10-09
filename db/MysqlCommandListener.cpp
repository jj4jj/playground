
#include "base/Log.h"
#include "MysqlProxy.h"
#include "MysqlCommandListener.h"

#if 1
int     MysqlCommandListener::OnResponse(MysqlResponse & rsp)
{
    LOG_INFO("on mysql response ret = %d table = %s op = %d",rsp.ret,rsp.tblname.c_str(),rsp.op);    
    return 0;
}
#endif

