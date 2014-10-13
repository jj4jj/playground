#pragma once
#include "base/stdinc.h"
#include "base/Buffer.h"
#include "DataListener.h"
#include "db/MysqlAgent.h"
#include "MetaSerializer.h"
#include "DataCommon.h"

struct DBAgentOption
{
    vector<MysqlProxyOption>   addrList;
    vector<string>             tableTypes;
};

class DBAgent
{
public:
    int  AddListener(string typeName,DataListenerPtr ptr);
    int  Init(const DBAgentOption  & cao,MetaSerializer * seri);
    int  GenerateMysqlMetas(vector<MysqlMeta> & metas,const vector<string> & tables);
    int  Polling(int    iProcPerTick);
    int  Destroy();
public:
    int  Get(void * obj,vector<string> & fields,const Buffer & cb,const char* pszWhereCond = NULL);
    int  Remove(void * obj,const Buffer & cb);
    int  Insert(void * obj,const Buffer &  cb);       
    int  Update(void * obj,vector<string> & fields,const Buffer &  cb);   
    int  Exists(void*  obj,const Buffer &  cb);
    int  CreateTable(const char* pszName);
public:
    int  DispatchResult(MysqlResponse & rsp); 
    MetaSerializer::MetaObject*    FindObject(const string & key);
    void           FreeObject(const string & key);
protected:
    int            GetPrimaryKey(void* obj,string & key);
    int            CreateObjectFromMysql(MysqlResponse & rsp,void ** ppObj);
    int            GenerateMysqlFields(vector<MysqlField> & data,void* obj,vector<string> * fields);
    MetaSerializer::MetaObject*    FindObject(MetaSerializer::MetaObject * obj);
public:
    DBAgent():serializer(NULL){}
private:
    MetaSerializer   *   serializer;
    MysqlAgent          mysql;
    unordered_map<string,MetaSerializer::MetaObjectPtr >  m_mpGetObjects;
    MetaSerializer::MetaObjectPtr                         m_ptrTmpObj;
    std::map<string,DataListenerPtr>      m_mpListener;
};


