#pragma once

#include "base/stdinc.h"
#include "base/Buffer.h"
#include "MysqlMeta.h"


struct MysqlProxyOption
{
    string  ip;
    int     port;    
    string  uname;
    string  passwd;
    string  dbname;
    string  unisock;
    long    cliflag;
    int     wait_timeout;
    int     intr_timeout;
public:
    MysqlProxyOption()
    {
        port = cliflag = wait_timeout = intr_timeout = 0;
        ip = uname = passwd = dbname = unisock = "";
    }
};


enum MysqlRequestType
{
    MYSQL_REQ_SELECT = 1,
    MYSQL_REQ_UPDATE = 2,
    MYSQL_REQ_DELETE = 3,
    MYSQL_REQ_INSERT = 4,
    MYSQL_REQ_CREATE_TB = 6,
};


enum    MysqlResponseResult
{
    MYSQL_RESULT_OK = 0,
    MYSQL_RESULT_NO_DATA = 1,
    MYSQL_RESULT_ALREADY_EXIST = 2,
    MYSQL_RESULT_TIMEOUT = 3,
    MYSQL_RESULT_GET_NO_PK = 4,
    MYSQL_RESULT_NO_META = 5,    
    MYSQL_RESULT_STORE_RESULT_ERR = 6,
    MYSQL_RESULT_SQL_GEN_ERR = 7,



    /////////////////////////////
    MYSQL_RESULT_UNKOWN      = 0x7F,
    MYSQL_RESULT_SQL_EXE_ERR = 0x7FFF,
    /////////////////////////////
};

struct MysqlRequest
{
    string tblname;
    int op;//op type : select insert update delete    
    string                where;
    vector<MysqlField>    data;
    vector<string>        fields;
    Buffer                cb;
    void Init()
    {
        tblname = "";
        op = 0;
        data.clear();
        fields.clear();
    }    
};
typedef shared_ptr<MysqlRequest>    MysqlRequestPtr;
////////////////////////////////////////////////////
struct MysqlResponse
{
    string tblname;
    int op;               //op type : select insert update delete    
    vector<MysqlField>  data;
    Buffer                cb;
    int ret;
};
typedef shared_ptr<MysqlResponse>    MysqlResponsePtr;


class MysqlAgent;

class MysqlProxy
{
public:
    int     Init(const std::vector<MysqlMeta> & metas);
    int     Connect(const MysqlProxyOption & opt);
    int     SelectDB(const string & name);
    int     CreateDB(const string & name);
    int     CreateTable(MysqlMeta & meta);
    int     DispatchReq(const MysqlRequest & req);
    int     Select(const string & tblname,const string & where,
                    const vector<string> & selCols, vector<MysqlField> & data);
    int     Insert(const string & table,const std::vector<MysqlField> & data);
    int     Update(const string & table,const string & where,const std::vector<MysqlField> & data);
    int     Delete(const string &  table,const string & where);
    int     ExecuteSQL(const string & sql);
    void    Destroy();
    inline  int   GetLastErrNo(){if(conn) return mysql_errno(conn);return 0;}
    inline  const char* GetLastErrStr(){if(conn)return mysql_error(conn);return NULL;}
    inline  MysqlResponse & GetRsp(){return rsp;}
    MysqlAgent* GetAgent(){return agent;}
protected:
    MysqlMeta*   GetTableMeta(const string & name);
public:
    MysqlProxy(MysqlAgent * p = NULL):agent(p){conn= NULL;tableMetas.clear();}
    ~MysqlProxy(){Destroy();}
public:
    enum{
        PROXY_STATE_INVALID = 0,
        PROXY_STATE_CONNECTING = 1,
        PROXY_STATE_READY = 2,
        PROXY_STATE_DOING = 3,
        PROXY_STATE_DONE = 4,
    };
private:
    //socket ? connector ?
    MYSQL   *conn;
    MysqlProxyOption   m_opt;
    std::map<string,MysqlMeta>   tableMetas;
    MysqlResponse       rsp;
    MysqlAgent*        agent;
};
typedef shared_ptr<MysqlProxy>    MysqlProxyPtr;    






