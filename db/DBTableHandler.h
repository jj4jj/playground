#pragma once
#include "MysqlDBMeta.h"

class DBTableHandler
{
public:
    DBTableHandler(){}
    virtual ~DBTableHandler(){}
public:            
	virtual int 	OnGet(int ret,vector<DBTableField> & data,vector<uint8_t>  & cb);
	virtual int 	OnRemove(int ret,vector<uint8_t>  & cb);
	virtual int 	OnUpdate(int ret,vector<uint8_t>  & cb);
	virtual int 	OnInsert(int ret,vector<uint8_t>  & cb);
    virtual int     OnCreateTable(int ret,vector<uint8_t>  & cb);


    virtual int 	OnGetKeyRank(int ret,vector<DBTableField> & data,vector<uint8_t>  & cb);
	virtual int 	OnGetRankKey(int ret,vector<DBTableField> & data,vector<uint8_t>  & cb);
	virtual int 	OnCount(int ret,vector<DBTableField> & data,vector<uint8_t>  & cb);
	virtual int 	OnMGet(int ret,vector<DBTableField> & data,vector<uint8_t>  & cb);    
};
typedef shared_ptr<DBTableHandler>  DBTableHandlerPtr;
////////////////////////////////////////////////////////////

