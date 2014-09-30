#pragma once
#include "base/stdinc.h"
#include "mysql.h"
//#include "my_global.h"
#include "base/Buffer.h"

struct DBTable;
class DBTableHandler
{
public:
    DBTableHandler(DBTable* _pTable):m_pTable(_pTable){}
    ~DBTableHandler(){}
public:        
	virtual int 	OnGet();
	virtual int 	OnMGet();
	virtual int 	OnRemove();
	virtual int 	OnUpdate();
	virtual int 	OnInsert();
	virtual int 	OnGetKeyRank();
	virtual int 	OnGetRankKey();
	virtual int 	OnCount();
private:
    DBTable * m_pTable;    
};
typedef shared_ptr<DBTableHandler>  DBTableHandlerPtr;
////////////////////////////////////////////////////////////

struct DBTable
{
    enum{
        DBTABLE_TYPE_KV    = 1,
        DBTABLE_TYPE_KHMAP = 2,
        DBTABLE_TYPE_KZSET = 3,        
    };
    string name;
    int    type;
    DBTableHandlerPtr   handler;    
};


struct DBTableField
{
    string name;
    int type ;
    union
    {        
        uint32_t time;
        int32_t  i32;
        int64_t  i64;
        uint32_t u32;
        uint64_t u64;
        float    r32;
        double   r64;
    } u_data;
    vector<char>   buffer;
    DBTableField()
    {
        u_data.u64 = 0L;
        buffer.clear();
    }
};



struct DBTableFieldMeta
{
    enum
    {
        VAL_TYPE_INVALID    = 0,
        VAL_TYPE_INT32      = 1,
        VAL_TYPE_INT64      ,
        VAL_TYPE_UINT32     ,
        VAL_TYPE_UINT64     ,
        VAL_TYPE_REAL32     ,
        VAL_TYPE_REAL64     ,
        VAL_TYPE_VARCHAR    ,//255
        VAL_TYPE_TEXT       ,//64K
        VAL_TYPE_BLOB       ,//64K
        VAL_TYPE_DATETIME   ,//TIME
        VAL_TYPE_MAX        ,
    };
    string  name;
    int     type; 
    int     maxlen;
    bool    ispk;
};
struct DBTableMeta
{
    string tblname;
    std::vector<DBTableFieldMeta>   cols;
public:
    const char*  GetTypeString(uint8_t type);
    int GetCreateTableSQL(string & sql);
    int Init(vector<DBTableField> & colvs);
    int GetSelectTableSQL(const vector<DBTableField> & pks,const vector<string> & selCols,string & sql,MYSQL   *conn);
    int GetUpdateTableSQL(const vector<DBTableField> & pks,const vector<DBTableField> & data,string & sql,MYSQL   *conn );
    int GetInsertTableSQL(const vector<DBTableField> & cols,string & sql,MYSQL   *conn);
    int GetDeleteTableSQL(const vector<DBTableField> & pks,string & sql,MYSQL   *conn);
    string GetFieldValueSQL(const DBTableField & field,MYSQL   *conn);
    int SetFieldValue(DBTableField & col,char* data,uint64_t datalen);
    string    Visual(const std::vector<DBTableField>   & vc);
public:
    static    DBTableFieldMeta * GetFieldMeta(DBTableMeta & table,const string & fieldName);
    static    DBTableField * GetField(std::vector<DBTableField> & fields,const char* pszFieldName);
};

enum DBTableOpCode
{
    OP_SELECT = 1,
    OP_INSERT = 2,
    OP_UPDATE = 3,
    OP_DELETE = 4,
};
enum    DBTableOpResult
{
    OP_RESULT_OK = 0,
    OP_RESULT_NO_DATA = 1,
    OP_RESULT_ALREADY_EXIST = 2,
    OP_RESULT_TIMEOUT = 3,
    OP_RESULT_GET_NO_PK = 4,
    OP_RESULT_NO_META = 5,    
    OP_RESULT_SQL_EXE_ERR = 6,
    OP_RESULT_STORE_RESULT_ERR = 7,
    OP_RESUTL_SQL_GEN_ERR,
    /////////////////////////////
    OP_RESULT_OTHER = 0xFFFF,
};
struct DBTableOpReq
{
    string tblname;
    int op;//op type : select insert update delete    
    vector<DBTableField>  data;
    vector<string>              fields;
    vector<uint8_t>             cb;
    
};
struct DBTableOpRsp
{
    string tblname;
    int op;//op type : select insert update delete    
    vector<DBTableField>  data;
    vector<uint8_t>             cb;
    int ret;
};


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


//it represents a connector to db server
//worker thread

/*
Agent(main thread)
    |_________ worker thread
                |
               proxy (connection)
*/
/*
main thread
    req
     |
     get a free conn
     |
     conn -> set req 
     conn -> notify -> conn -> busying (erase from free conn list)

    poll from ready list
      

worker thread
    wait for req
     | notify
   get req
     |
   exec req
     |
   exec ready

*/
//req - > sql
class DBProxy
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
protected:
    DBTableMeta*   GetTableMeta(const string & name);
public:
    DBProxy(){}
    ~DBProxy(){Destroy();}
public:
    enum{
        PROXY_STATE_INVALID = 0,
        PROXY_STATE_CONNECTING = 1,
        PROXY_STATE_READY = 2,
        PROXY_STATE_DOING = 3,
        PROXY_STATE_DONE = 4,
    };
private:
    int     state;//
    //socket ? connector ?
    MYSQL   *conn;
    DBProxyOption   m_opt;
    std::map<string,DBTableMeta>   tableMetas;
    DBTableOpReq    req;
    DBTableOpRsp    rsp;
};
typedef shared_ptr<DBProxy>    DBProxyPtr;    


////////////////////////////////////////////////////////////

class DBAgent
{
public:
    DBAgent();
    ~DBAgent();
protected:
    static uint32_t Hash(const char* pszTableName);    
public:
    //return the handle
    int     RegisterTable(const DBTable &   table);
    int     Init(const std::vector<DBProxyOption> & proxyList);	//proxy list
	int     Polling(long lTimeOutUs);	
    void    Destroy();
	//-------------------------------------------------------------------------
	int		Get(const char* pszTableName,const Buffer & key,const Buffer & cb);
	int     MGet(const char* pszTableName,const std::vector<Buffer> & keylist,const Buffer & cb);
	int     Remove(const char* pszTableName ,const Buffer & key,const Buffer & cb);	
	int     Update(const char* pszTableName ,const Buffer & key,const Buffer & value,const Buffer & cb);
	int     Insert(const char* pszTableName ,const Buffer & key,const Buffer & value,const Buffer & cb);
	int     GetKeyRank(const char* pszTableName ,const Buffer & key,const Buffer & cb);
	int     GetRankKey(const char* pszTableName ,int rank,const Buffer & cb);
	int     Count(const char* pszTableName ,const Buffer & cb);		
	//----------------------------------------------------------------------------------
	int     OnGet(const char* pszTableName ,int ret,const Buffer & key,const Buffer & value,const Buffer & cb);
	int     OnMGet(const char* pszTableName ,int ret,const std::vector<Buffer> & keylist,
                         const std::vector<Buffer> & valuelist,
                         const Buffer & cb,bool more);
	int     OnRemove(const char* pszTableName ,int ret,const Buffer & key,const Buffer & cb);
	int     OnUpdate(const char* pszTableName ,int ret,const Buffer & key,const Buffer & cb);
	int     OnInsert(const char* pszTableName ,int ret,const Buffer & key,const Buffer & cb);
	int     OnGetKeyRank(const char* pszTableName ,const Buffer & key,const Buffer & cb);
	int     OnGetRankKey(const char* pszTableName ,int rank,const Buffer & cb);
	int     OnCount(const char* pszTableName ,const Buffer & cb); 


private:
    //name hash -> table
    unordered_map<uint32_t,DBTable>  handlerMap;
    std::vector<DBProxyOption>       proxyAddrList;
    //ssdb handle 
    //map with -> proxy
    std::vector<DBProxyPtr>   clientProxyList;
};






