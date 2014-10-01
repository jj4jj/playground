#pragma once

#include "base/stdinc.h"
#include "base/Buffer.h"
#include "MysqlDBMeta.h"


struct DBProxyOption
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
    DBProxyOption()
    {
        port = cliflag = wait_timeout = intr_timeout = 0;
        ip = uname = passwd = dbname = unisock = "";
    }
};


enum DBTableOpCode
{
    OP_SELECT = 1,
    OP_INSERT = 2,
    OP_UPDATE = 3,
    OP_DELETE = 4,
    OP_CREATE_TB = 6,
};


enum    DBTableOpResult
{
    OP_RESULT_OK = 0,
    OP_RESULT_NO_DATA = 1,
    OP_RESULT_ALREADY_EXIST = 2,
    OP_RESULT_TIMEOUT = 3,
    OP_RESULT_GET_NO_PK = 4,
    OP_RESULT_NO_META = 5,    
    OP_RESULT_STORE_RESULT_ERR = 6,
    OP_RESUTL_SQL_GEN_ERR = 7,



    /////////////////////////////
    OP_RESULT_UNKOWN      = 0x7F,
    OP_RESULT_SQL_EXE_ERR = 0x7FFF,
    /////////////////////////////
};

struct DBTableOpReq
{
    string tblname;
    int op;//op type : select insert update delete    
    vector<DBTableField>  data;
    vector<string>        fields;
    vector<uint8_t>       cb;
    void Init()
    {
        tblname = "";
        op = 0;
        data.clear();
        fields.clear();
        cb.clear();        
    }
    
};
typedef shared_ptr<DBTableOpReq>    DBTableOpReqPtr;
////////////////////////////////////////////////////
struct DBTableOpRsp
{
    string tblname;
    int op;//op type : select insert update delete    
    vector<DBTableField>  data;
    vector<uint8_t>             cb;
    int ret;
};
typedef shared_ptr<DBTableOpRsp>    DBTableOpRspPtr;


class MysqlAgent;

class MysqlProxy
{
public:
    int     Init(const std::vector<DBTableMeta> & metas);
    int     Connect(const DBProxyOption & opt);
    int     SelectDB(const string & name);
    int     CreateDB(const string & name);
    int     CreateTable(DBTableMeta & meta);
    int     DispatchReq(const DBTableOpReq & req);
    int     Select(const string & tblname,const vector<DBTableField> & pks,
                    const vector<string> & selCols, vector<DBTableField> & cols);
    int     Insert(const string & table,const std::vector<DBTableField> & data);
    int     Update(const string & table,const std::vector<DBTableField> & pks,const std::vector<DBTableField> & data);
    int     Delete(const string &  table,const std::vector<DBTableField> & pks);
    int     ExecuteSQL(const string & sql);
    void    Destroy();
    inline  int   GetLastErrNo(){if(conn) return mysql_errno(conn);return 0;}
    inline  const char* GetLastErrStr(){if(conn)return mysql_error(conn);return NULL;}
    inline  DBTableOpRsp & GetRsp(){return rsp;}
    MysqlAgent* GetAgent(){return agent;}
protected:
    DBTableMeta*   GetTableMeta(const string & name);
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
    DBProxyOption   m_opt;
    std::map<string,DBTableMeta>   tableMetas;
    DBTableOpRsp       rsp;
    MysqlAgent*        agent;
};
typedef shared_ptr<MysqlProxy>    MysqlProxyPtr;    






