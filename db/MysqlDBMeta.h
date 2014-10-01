#pragma once
#include "base/stdinc.h"
#include "mysql.h"
#include "mysqld_error.h"
#include "base/Buffer.h"

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
        Init();
    }
    void    Init()
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


