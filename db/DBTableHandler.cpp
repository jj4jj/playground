#include "DBTableHandler.h"
#include "base/Log.h"

#if 1
int 	DBTableHandler::OnGet(int ret,vector<DBTableField> & data,vector<uint8_t>  & cb)
{
    LOG_INFO("on get ret = %d cb = %s",ret,(char*)&cb[0]);    
    return 0;
}
int 	DBTableHandler::OnRemove(int ret,vector<uint8_t>  & cb)
{
    LOG_INFO("on remove ret = %d cb = %s",ret,(char*)&cb[0]);    
    return 0;
}
int 	DBTableHandler::OnUpdate(int ret,vector<uint8_t>  & cb)
{
    LOG_INFO("on update ret = %d cb = %s",ret,(char*)&cb[0]);    
    return 0;
}
int 	DBTableHandler::OnInsert(int ret,vector<uint8_t>  & cb)
{
    LOG_INFO("on insert ret = %d cb = %s",ret,(char*)&cb[0]);    
    return 0;
}
int     DBTableHandler::OnCreateTable(int ret,vector<uint8_t>  & cb)
{
    LOG_INFO("on create  table ret = %d cb = %s",ret,(char*)&cb[0]);    
    return 0;
}

int 	DBTableHandler::OnGetKeyRank(int ret,vector<DBTableField> & data,vector<uint8_t>  & cb)
{
    LOG_INFO("on get key rank ret = %d cb = %s",ret,(char*)&cb[0]);    
    return 0;
}
int 	DBTableHandler::OnGetRankKey(int ret,vector<DBTableField> & data,vector<uint8_t>  & cb)
{
    LOG_INFO("on get rank ret = %d cb = %s",ret,(char*)&cb[0]);    
    return 0;
}
int 	DBTableHandler::OnCount(int ret,vector<DBTableField> & data,vector<uint8_t>  & cb)
{
    LOG_INFO("on count ret = %d cb = %s",ret,(char*)&cb[0]);    
    return 0;
}
int 	DBTableHandler::OnMGet(int ret,vector<DBTableField> & data,vector<uint8_t>  & cb)
{
    LOG_INFO("on mget ret = %d cb = %s",ret,(char*)&cb[0]);    
    return 0;
}
#endif

