#pragma once
#include "base/stdinc.h"
#include "mysql.h"
#include "mysqld_error.h"
#include "base/Buffer.h"

struct MysqlField
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
    MysqlField()
    {
        Init();
    }
    void    Init()
    {
        u_data.u64 = 0L;
        buffer.clear();
    }
};



struct MysqlFieldMeta
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
    string  typeName;
    int     maxlen;
    bool    ispk;
public:
    MysqlFieldMeta()
    {
        Init();
    }
    void    Init()
    {
        ispk = false;
        maxlen = 0;
        type = 0;
        name = typeName = "";
    }
};
struct MysqlMeta
{
    string tblname;
    std::vector<MysqlFieldMeta>   cols;
public:
    const char*  GetTypeString(uint8_t type);
    int GetCreateTableSQL(string & sql);
    int Init(vector<MysqlField> & colvs);
    int GetSelectTableSQL(const string & where,const vector<string> & selCols,string & sql,MYSQL   *conn);
    int GetUpdateTableSQL(const string & where,const vector<MysqlField> & data,string & sql,MYSQL   *conn );
    int GetInsertTableSQL(const vector<MysqlField> & data,string & sql,MYSQL   *conn);
    int GetDeleteTableSQL(const string & where,string & sql,MYSQL   *conn);
    string GetFieldValueSQL(const MysqlField & field,MYSQL   *conn);
    int SetFieldValue(MysqlField & col,char* data,uint64_t datalen);
    string    Visual(const std::vector<MysqlField>   & vc);

public:
    static    MysqlFieldMeta * GetFieldMeta(MysqlMeta & table,const string & fieldName);
    static    MysqlField * GetField(std::vector<MysqlField> & fields,const char* pszFieldName);
};


