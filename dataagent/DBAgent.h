#pragma once
#include "base/stdinc.h"
#include "base/Buffer.h"
#include "DataListener.h"
#include "db/MysqlAgent.h"
#include "MemSerializer.h"


class DBCommandListener
{
public:
	virtual int 	OnGet(int ret,vector<MysqlField> & data,Buffer & cb);
	virtual int 	OnRemove(string & name,int ret,Buffer & cb);
	virtual int 	OnUpdate(string & name,int ret,Buffer & cb);
	virtual int 	OnInsert(string & name,int ret,Buffer & cb);
    virtual int     OnCreateTable(string & name,int ret,Buffer & cb);


    virtual int 	OnGetKeyRank(string & name,int ret,vector<MysqlField> & data,Buffer & cb);
	virtual int 	OnGetRankKey(string & name,int ret,vector<MysqlField> & data,Buffer & cb);
	virtual int 	OnCount(string & name,int ret,vector<MysqlField> & data,Buffer & cb);
	virtual int 	OnMGet(string & name,int ret,vector<MysqlField> & data,Buffer & cb);    

};
typedef shared_ptr<DBCommandListener>   DBCommandListenerPtr;

struct DBAgentOption
{
    vector<MysqlProxyOption>   addrList;
    vector<string>             tableTypes;
};
enum    DataResultCode
{
    DATA_TIME_OUT      =  -1,
    DATA_OK            =   0,
    DATA_NO_EXISTS         ,
    DATA_ALREADY_EXISTS    ,
    DATA_UNPACK_ERROR      ,
    DATA_INTERNAL_ERR      ,
    DATA_REDIS_ERR_START   =    1000,
    DATA_MYSQL_ERR_START   =    2000,
};


class MemSerializer;

class DBAgent
{
public:
    int  AddListener(string typeName,DataListenerPtr ptr);
    int  Init(const DBAgentOption  & cao,MemSerializer * seri);
    int  GenerateMysqlMetas(vector<MysqlMeta> & metas,const vector<string> & tables);
    int  Polling(int    iProcPerTick);
    int  Destroy();
public:
    int  Get(void * obj,vector<string> & fields,const Buffer & cb,const char* pszWhereCond = NULL);
    int  Remove(void * obj,const Buffer & cb);
    int  Insert(void * obj,const Buffer &  cb);       
    int  Update(void * obj,vector<string> & fields,const Buffer &  cb);   
    int  Exists(void*  obj,const Buffer &  cb);
public:
    int  DispatchResult(MysqlResponse & rsp); 
    MemSerializer::MetaObject*    FindObject(const string & key);
    void           FreeObject(const string & key);
protected:
    int            GetPrimaryKey(void* obj,string & key);
    int            CreateObjectFromMysql(MysqlResponse & rsp,void ** ppObj);
    int            GenerateMysqlFields(vector<MysqlField> & data,void* obj,vector<string> * fields);
    MemSerializer::MetaObject*    FindObject(MemSerializer::MetaObject * obj);
public:
    DBAgent():serializer(NULL){}
private:
    MemSerializer   *   serializer;
    MysqlAgent          mysql;
    unordered_map<string,MemSerializer::MetaObjectPtr >  m_mpGetObjects;
    MemSerializer::MetaObjectPtr                         m_ptrTmpObj;
    std::map<string,DataListenerPtr>      m_mpListener;
};


