#include "base/DateTime.h"
#include "base/Log.h"
#include "base/StringUtil.h"

#include "DBAgent.h"

#if 1
const char* DBTableMeta::GetTypeString(uint8_t type)
{        
    static const char* type2str[] = {
    "INVALID",
    "INTEGER",
    "BIGINT",
    "INTEGER UNSIGNED"  ,
    "BIGINT UNSIGNED"   ,
    "FLOAT"     ,
    "DOUBLE"     ,
    "VARCHAR",//255
    "TEXT"       ,//64K
    "BLOB"       ,//64K
    "DATETIME"   ,//TIME
    };
    assert(type < DBTableFieldMeta::VAL_TYPE_MAX);
    return type2str[type];
}
int DBTableMeta::GetCreateTableSQL(string & sql)
{
/*
eg.
CREATE TABLE IF NOT EXISTS company_user(     
id int(10) UNSIGNED AUTO_INCREMENT NOT NULL PRIMARY KEY,     
username VARCHAR(24) NOT NULL UNIQUE,     
password VARCHAR(8) NOT NULL,     
company_name VARCHAR(250) NOT NULL UNIQUE,     
company_type VARCHAR(30) NOT NULL,     
company_intro TEXT,     
office_image TEXT,     
register_date DATE NOT NULL
)ENGINE=InnoDB DEFAULT CHARSET utf8 COLLATE utf8_general_ci;
*/
    sql = "CREATE TABLE IF NOT EXISTS "+tblname + " (";
    char tmpBuffer[128];
    for(uint i = 0;i < cols.size(); ++i)
    {
        sql += cols[i].name + " "+ GetTypeString(cols[i].type);
        if(cols[i].maxlen > 0 && cols[i].type == DBTableFieldMeta::VAL_TYPE_VARCHAR)
        {
            snprintf(tmpBuffer,sizeof(tmpBuffer),"(%d)",cols[i].maxlen);
            sql += tmpBuffer;
        }
        sql += " NOT NULL";
        if(cols[i].ispk)
        {
            sql += " PRIMARY KEY ";
        }
        if(i + 1 < cols.size())
        {
            sql += ",\n";
        }
    }
    sql += " )ENGINE=InnoDB DEFAULT CHARSET utf8 COLLATE utf8_general_ci";
    return 0;
}
int DBTableMeta::Init(vector<DBTableField> & colvs)
{
    colvs.clear();
    DBTableField ec;
    for(uint i = 0;i < cols.size(); ++i)
    {
        ec.name = cols[i].name;
        ec.type = cols[i].type;
        colvs.push_back(ec);
    }
    return 0;
}
int DBTableMeta::GetUpdateTableSQL(const vector<DBTableField> & pks,const vector<DBTableField> & data,string & sql,MYSQL   *conn)
{
    //eg. UPDATE <TABLE> SET C1=V1,C2=V2 WHERE 
    sql = "UPDATE ";
    sql += tblname + " SET ";
    for(uint i = 0; i < data.size(); ++i)
    {
        if(i != 0)
        {
            sql += ",";
        }
        sql += data[i].name ;
        sql += "=";        
        sql += GetFieldValueSQL(data[i],conn);
    }        
    sql += " WHERE ";
    for(uint i = 0; i < pks.size(); ++i)
    {
        sql += pks[i].name + "=";
        sql += GetFieldValueSQL(pks[i],conn);
        if(i + 1 < pks.size())
        {
            sql += " AND ";
        }
    }  
    return 0;
}
int DBTableMeta::GetInsertTableSQL(const vector<DBTableField> & cols,string & sql,MYSQL   *conn)
{
    //sql inject checking
    sql = "INSERT INTO ";
    sql += tblname + "(";
    for(uint i = 0; i < cols.size(); ++i)
    {
        sql += cols[i].name ;
        if(i + 1 < cols.size())
        {
            sql += ", ";
        }
    }        
    sql += ") VALUES(";
    for(uint i = 0; i < cols.size(); ++i)
    {
        sql += GetFieldValueSQL(cols[i],conn);
        if(i + 1 < cols.size())
        {
            sql += ", ";
        }
    }        
    sql += ")";//blabla
    return 0;
}
int DBTableMeta::GetDeleteTableSQL(const vector<DBTableField> & pks,string & sql,MYSQL   *conn)
{
    //eg. DELETE FROM TABLE WHERE X=X
    sql = "DELETE FROM ";
    sql += tblname;
    sql += " WHERE ";
    for(uint i = 0;i < pks.size(); ++i)
    {   
        if(pks[i].type == DBTableFieldMeta::VAL_TYPE_DATETIME)
        {
            sql += "FROM_UNIXTIME(";
            sql += pks[i].name;
            sql += ")";
        }
        else
        {
            sql += pks[i].name ;
        }
        sql += "=";            
        sql += GetFieldValueSQL(pks[i],conn);
        if(i + 1 < pks.size())
        {
            sql += " AND ";
        }        
    }
    return 0;
}
int DBTableMeta::GetSelectTableSQL(const vector<DBTableField> & pks,const vector<string> & selCols,string & sql,MYSQL   *conn)
{
    //eg. SELECT FROM TABLE WHERE X=X
    sql = "SELECT ";
    if(selCols.empty())
    {
        sql += "* ";
    }
    else
    {
        for(uint i = 0 ;i < selCols.size(); ++i)
        {
            sql += selCols[i];
            if(i + 1 < selCols.size())
            {
                sql +=  ", ";
            }    
        }
    }
    sql += " FROM ";
    sql += tblname;
    sql += " WHERE ";
    for(uint i = 0;i < pks.size(); ++i)
    {   
        if(pks[i].type == DBTableFieldMeta::VAL_TYPE_DATETIME)
        {
            sql += "FROM_UNIXTIME(";
            sql += pks[i].name;
            sql += ")";
        }
        else
        {
            sql += pks[i].name ;
        }
        sql += "=";            
        sql += GetFieldValueSQL(pks[i],conn);
        if(i + 1 < pks.size())
        {
            sql += " AND ";
        }        
    }
    return 0;        
}
string   DBTableMeta::GetFieldValueSQL(const DBTableField & col,MYSQL   *conn )
{
    //todo check string for prevent sql injection
    string sqlv = "";//"'";
    char digitbuffer[20];
    DBTableFieldMeta * fieldMeta = GetFieldMeta(*this,col.name);
    assert(fieldMeta);
    switch(fieldMeta->type)
    {
       case  DBTableFieldMeta::VAL_TYPE_INT32      :
           snprintf(digitbuffer,sizeof(digitbuffer),"%d",col.u_data.i32);
           break;
       case  DBTableFieldMeta::VAL_TYPE_INT64      :
           snprintf(digitbuffer,sizeof(digitbuffer),"%ld",col.u_data.i64);
           break;
       case  DBTableFieldMeta::VAL_TYPE_UINT32     :
           snprintf(digitbuffer,sizeof(digitbuffer),"%u",col.u_data.u32);
           break;
       case  DBTableFieldMeta::VAL_TYPE_UINT64     :
           snprintf(digitbuffer,sizeof(digitbuffer),"%lu",col.u_data.u64);
           break;
       case  DBTableFieldMeta::VAL_TYPE_REAL32     :
           snprintf(digitbuffer,sizeof(digitbuffer),"%f",col.u_data.r32);
           break;
       case  DBTableFieldMeta::VAL_TYPE_REAL64     :
           snprintf(digitbuffer,sizeof(digitbuffer),"%lf",col.u_data.r64);
           break;
       case  DBTableFieldMeta::VAL_TYPE_VARCHAR    ://255
       case  DBTableFieldMeta::VAL_TYPE_TEXT       ://64K
       {
            const char * psz = &(col.buffer.at(0));
            uint len = strlen(psz);
            if(len >= col.buffer.size())
            {
                LOG_ERROR("buffer size = %d is smaller than text = %d [%s]!",
                    col.buffer.size(),len,psz);
                len = col.buffer.size() - 1;
            }

            sqlv = "'";
            Buffer buf;
            buf.Create(len*2+1);
            if(conn)
            {
                mysql_real_escape_string(conn,(char*)buf.pBuffer,psz,len);
            }
            else
            {
                mysql_escape_string((char*)buf.pBuffer,psz,len);
            }
            sqlv += (char*)buf.pBuffer;
            buf.Destroy();      
            sqlv += "'";
            return sqlv;
       }
       case  DBTableFieldMeta::VAL_TYPE_BLOB       ://64K
       {
            sqlv = "'";
            //blob to string
            Buffer buf;
            buf.Create(col.buffer.size()*2+1);
            if(conn)
            {
                mysql_real_escape_string(conn,(char*)buf.pBuffer,&(col.buffer[0]),col.buffer.size());

            }
            else
            {
                mysql_escape_string((char*)buf.pBuffer,&(col.buffer[0]),col.buffer.size());
            }


           sqlv += (char*)buf.pBuffer;
           buf.Destroy();     
           sqlv += "'";
           return sqlv;
       }
           break;
       case  DBTableFieldMeta::VAL_TYPE_DATETIME   ://TIME
       {
           snprintf(digitbuffer,sizeof(digitbuffer),"%u",col.u_data.time);
           sqlv = "FROM_UNIXTIME(";
           sqlv += digitbuffer;
           sqlv += ")";
           return sqlv;            
       }
           break;
       default:
        assert(false);
           return sqlv;
           break;
    }
    sqlv += digitbuffer;
    sqlv += "";//"'";
    return sqlv;
}
int DBTableMeta::SetFieldValue(DBTableField & field,char* data,uint64_t datalen)
{
    //LOG_DEBUG("set field value name = %s type = %d data = [%s] len = %lu",
    //         field.name.c_str(),field.type,data,datalen);

    char numberBuffer[32];
    int minlen = std::min(datalen , sizeof(numberBuffer) - 1);
    memcpy(numberBuffer,data,minlen);
    numberBuffer[minlen] = 0;
    
    switch(field.type)
    {
       case  DBTableFieldMeta::VAL_TYPE_INT32      :
       case  DBTableFieldMeta::VAL_TYPE_INT64      :
           field.u_data.i64 = strtol(numberBuffer,NULL,10);
           break;
       case  DBTableFieldMeta::VAL_TYPE_UINT32     :
       case  DBTableFieldMeta::VAL_TYPE_UINT64     :
           field.u_data.i64 = strtoul(numberBuffer,NULL,10);
           break;
       case  DBTableFieldMeta::VAL_TYPE_REAL32     :
           field.u_data.r32 = atof(numberBuffer);
           break;
       case  DBTableFieldMeta::VAL_TYPE_REAL64     :
           field.u_data.r64 = atof(numberBuffer);
           break;
       case  DBTableFieldMeta::VAL_TYPE_VARCHAR    ://255
       case  DBTableFieldMeta::VAL_TYPE_TEXT       ://64K
       {
            field.buffer.resize(datalen + 1);
            memcpy(&field.buffer[0],data,datalen);
            field.buffer[datalen] = 0;
            break;
       }
       case  DBTableFieldMeta::VAL_TYPE_BLOB       ://64K
       {
            field.buffer.resize(datalen);
            memcpy(&field.buffer[0],data,datalen);
            break;
       }
       case  DBTableFieldMeta::VAL_TYPE_DATETIME   ://TIME
       {
            string datetimes = data;
            field.u_data.time = Time::mkTimeStamp(datetimes.c_str());
       }
           break;
       default:
       assert(false);
           break;
    }   
    return 0;
}
string    DBTableMeta::Visual(const std::vector<DBTableField>   & vc)
{
    string vs = "";
    for(uint i = 0;i < vc.size(); ++i)
    {
        vs += vc[i].name + "\t";
    }
    vs += "\n";
    for(uint i = 0;i < vc.size(); ++i)
    {
        vs += GetFieldValueSQL(vc[i],NULL);
        vs += "\t";
    }
    vs += "\n";
    return vs;
}
DBTableFieldMeta *   DBTableMeta::GetFieldMeta(DBTableMeta & table,const string & fieldName)
{
    for(uint i = 0;i < table.cols.size(); ++i)
    {
        if(table.cols[i].name == fieldName)
        {
            return &(table.cols.at(i));
        }
    }
    return NULL;
}

DBTableField * DBTableMeta::GetField(std::vector<DBTableField> & fields,const char* pszFieldName)
{
    for(uint i = 0;i < fields.size(); ++i)
    {
        if(fields[i].name == pszFieldName)
        {
            return &(fields.at(i));
        }
    }
    return NULL;
}

#endif



#if 1
int     DBProxy::Init(const std::vector<DBTableMeta> & metas)
{
    tableMetas.clear();
    for(uint i = 0;i < metas.size(); ++i)
    {
        tableMetas[metas[i].tblname] = metas[i];
    }
    /////////////////////////////////////////////
    return 0;
}
int     DBProxy::Connect(const DBProxyOption & opt)
{        
    m_opt = opt;
    conn = mysql_init(NULL);
    if(!conn)
    {
        LOG_FATAL("mysql client init error = %d",mysql_errno(NULL));
        return -1;
    }

    char tmpset[255]="";
    if(opt.wait_timeout > 0)
    {
        sprintf(tmpset, "set wait_timeout=%d", opt.wait_timeout);
        mysql_options(conn, MYSQL_INIT_COMMAND, tmpset);
        mysql_options(conn,MYSQL_OPT_CONNECT_TIMEOUT,&opt.wait_timeout);
        mysql_options(conn, MYSQL_OPT_READ_TIMEOUT, &opt.wait_timeout);
        mysql_options(conn, MYSQL_OPT_WRITE_TIMEOUT, &opt.wait_timeout);
    }
    if(opt.intr_timeout > 0)
    {
        sprintf(tmpset, "set interactive_timeout=%d", opt.intr_timeout);
        mysql_options(conn, MYSQL_INIT_COMMAND, tmpset);
    }
    //auto reconnect
    bool auto_cnt = true;
    mysql_options(conn, MYSQL_OPT_RECONNECT, &auto_cnt);

    //connect to server
    if( !mysql_real_connect(conn,opt.ip.c_str(),opt.uname.c_str(),
        opt.passwd.c_str(),NULL,opt.port,
        opt.unisock.c_str(),opt.cliflag) )
    {                
        LOG_FATAL("mysql_real_connectn Error %u (%s)\n",
            mysql_errno(conn),mysql_error(conn));    
        goto FAIL_CONN;
    }    

    if(mysql_set_character_set(conn, "utf8"))
    {
        LOG_FATAL("set charset Error %u (%s)\n",
            mysql_errno(conn),mysql_error(conn));    
        goto FAIL_CONN;
    }
    if (mysql_autocommit(conn, 1))
    {
        LOG_FATAL("auto commit set  Error %u (%s)\n",
            mysql_errno(conn),mysql_error(conn));    
        goto FAIL_CONN;
    }
    if(CreateDB(opt.dbname))
    {
        LOG_FATAL("conn create db = %s  Error %u (%s)\n",
            opt.dbname.c_str(),mysql_errno(conn),mysql_error(conn));    
        goto FAIL_CONN;
    }
    SelectDB(opt.dbname);
    return 0;
    ////////////////////////////////////////////////
FAIL_CONN:
    conn = NULL;
    mysql_close(conn);
    return -1;
}
int     DBProxy::SelectDB(const string & name)
{
    string sql = "USE "+name;
    return ExecuteSQL(sql);
}
int     DBProxy::CreateDB(const string & name)
{        
    //CREATE DATABASE IF NOT EXISTS yourdbname DEFAULT CHARSET utf8 COLLATE utf8_general_ci;
    string sql = "CREATE DATABASE IF NOT EXISTS "+name;
    sql += " DEFAULT CHARSET utf8 COLLATE utf8_general_ci";
    return ExecuteSQL(sql);
}
int     DBProxy::CreateTable(DBTableMeta & meta)
{                
    string sql = "";
    meta.GetCreateTableSQL(sql);
    return ExecuteSQL(sql);
}
int     DBProxy::DispatchReq(const DBTableOpReq & req)
{
    rsp.tblname = req.tblname;
    rsp.op = req.op;
    rsp.cb = req.cb;
    rsp.ret = OP_RESULT_OTHER;
    
    switch(req.op)
    {
        case OP_SELECT:
            //
            rsp.ret =  Select(req.tblname,req.data,req.fields,rsp.data);
            break;
        case OP_INSERT:
            rsp.ret =  Insert(req.tblname,req.data);
            break;
        case OP_UPDATE:
            rsp.ret =  Update(req.tblname,req.data,req.data);
            break;
        case OP_DELETE:
            rsp.ret =  Delete(req.tblname,req.data);
            break;
        default:
            assert(false);
            break;
    }
    //rsp 
    //set ready
    state = PROXY_STATE_DONE;
    //
    //notify mgr
    
    return 0;
}

DBTableMeta*   DBProxy::GetTableMeta(const string & name)
{        
    if( tableMetas.find(name) != tableMetas.end())
    {
        return &(tableMetas[name]);
    }
    return NULL;
}
int     DBProxy::Select(const string & tblname,const vector<DBTableField> & pks,
                    const vector<string> & selCols, vector<DBTableField> & cols )
{
    DBTableMeta* meta = GetTableMeta(tblname);
    if(!meta)
    {
        return     OP_RESULT_NO_META;
    }
    if(pks.empty())
    {
        return     OP_RESULT_GET_NO_PK;
    }
    string sql ;        
    int ret = meta->GetSelectTableSQL(pks,selCols,sql,conn);
    if(ret)
    {
        return OP_RESUTL_SQL_GEN_ERR;
    }
    ret = ExecuteSQL(sql);
    if(ret)
    {
        return OP_RESULT_SQL_EXE_ERR ;
    }
    //fetch role
    MYSQL_RES *res_set;
    //it can be instead of mysql_use_result
    res_set = mysql_store_result(conn);  
    if(res_set == NULL)
    {
        LOG_ERROR("mysql_store_result failed %d for %s",
            mysql_errno(conn),mysql_error(conn));
        return OP_RESULT_STORE_RESULT_ERR;
    }        
    //get 
    int nfields = mysql_num_fields(res_set);
    MYSQL_ROW row = mysql_fetch_row(res_set);    
    unsigned long * lens = mysql_fetch_lengths(res_set);
    meta->Init(cols);
    ////////////////////////////////////////
    if(row)
    {
        MYSQL_FIELD * field = NULL;
        for(int i  = 0; i < nfields; i++)
        {
            field = mysql_fetch_field_direct(res_set,i);
            assert(field);
            DBTableField * col = meta->GetField(cols,field->name);
            assert(col);
            //meta set col value
            meta->SetFieldValue(*col,row[i],lens[i]);
        }
        ret = OP_RESULT_OK;
    }
    else
    {
        ret =  OP_RESULT_NO_DATA;
    }
    
    mysql_free_result(res_set);        
    return ret;        
}

int     DBProxy::Insert(const string & table,const std::vector<DBTableField> & data)
{

    DBTableMeta * meta = GetTableMeta(table);
    if(!meta)
    {
        LOG_ERROR("meta not found!");
        return OP_RESULT_NO_META;
    }
    //INSERT INTO <TABLE> (C1,C2)   VALUES(V1,V2);
    string sql = "";
    int ret = meta->GetInsertTableSQL(data,sql,conn);
    if(ret)
    {
        return OP_RESUTL_SQL_GEN_ERR;
    }
    ret = ExecuteSQL(sql); 
    if(ret)
    {
        return OP_RESULT_SQL_EXE_ERR;
    }
    return OP_RESULT_OK;        
}
int     DBProxy::Update(const  string &  table,const std::vector<DBTableField> & pks,const std::vector<DBTableField> & data)
{
    DBTableMeta * meta = GetTableMeta(table);
    if(!meta)
    {
        LOG_ERROR("meta not found!");
        return OP_RESULT_NO_META;
    }
    string sql = "";
    int ret = meta->GetUpdateTableSQL(pks, data,sql,conn);
    if(ret)
    {
        return OP_RESUTL_SQL_GEN_ERR;
    }
    ret = ExecuteSQL(sql);
    if(ret)
    {
        return OP_RESULT_SQL_EXE_ERR;
    }

    
    return OP_RESULT_OK;        
}
int     DBProxy::Delete(const string & table,const std::vector<DBTableField> & pks)
{
    //delete from <table> where pk
    DBTableMeta * meta = GetTableMeta(table);
    if(!meta)
    {
        LOG_ERROR("meta not found!");
        return OP_RESULT_NO_META;
    }
    //eg. DELETE FROM <TABLE> WHERE  PKS
    string sql = "";
    int ret = meta->GetDeleteTableSQL(pks,sql,conn);
    if(ret)
    {
        return OP_RESUTL_SQL_GEN_ERR;
    }
    ret = ExecuteSQL(sql); 
    if(ret)
    {
        return OP_RESULT_SQL_EXE_ERR;
    }
    return OP_RESULT_OK;        
}
int     DBProxy::ExecuteSQL(const string & sql)
{
    LOG_DEBUG("exec sql = \n[%s]\n",sql.c_str());
    if(mysql_query(conn,sql.c_str()))
    {
        LOG_ERROR("sql excute error = %d for = %s",
                mysql_errno(conn),mysql_error(conn));
        return -1;
    }
    return 0;
 }
void    DBProxy::Destroy()
{
    if(conn)
    {
        mysql_close(conn);
    }
}

#endif

////////////////////////////////////////////////////////////

#if 1
DBAgent::DBAgent()
{

}

DBAgent::~DBAgent()
{

}
uint32_t DBAgent::Hash(const char* pszTableName)    
{
    return 0;
}
//return the handle
int     DBAgent::RegisterTable(const DBTable &   table)
{
    return 0;
}
int     DBAgent::Init(const std::vector<DBProxyOption> & proxyList)	//proxy list
{
    assert(!clientProxyList.empty());
    
    return 0;
}
int     DBAgent::Polling(long lTimeOutUs)	
{
    return 0;
}
void    DBAgent::Destroy()
{
}
//-------------------------------------------------------------------------
int		DBAgent::Get(const char* pszTableName,const Buffer & key,const Buffer & cb)
{
    return 0;
}
int     DBAgent::MGet(const char* pszTableName,const std::vector<Buffer> & keylist,const Buffer & cb)
{
    return 0;
}
int     DBAgent::Remove(const char* pszTableName ,const Buffer & key,const Buffer & cb)	
{
    return 0;
}
int     DBAgent::Update(const char* pszTableName ,const Buffer & key,const Buffer & value,const Buffer & cb)
{
    return 0;
}
int     DBAgent::Insert(const char* pszTableName ,const Buffer & key,const Buffer & value,const Buffer & cb)
{
    return 0;
}
int     DBAgent::GetKeyRank(const char* pszTableName ,const Buffer & key,const Buffer & cb)
{
    return 0;
}
int     DBAgent::GetRankKey(const char* pszTableName ,int rank,const Buffer & cb)
{
    return 0;
}
int     DBAgent::Count(const char* pszTableName ,const Buffer & cb)		
{
    return 0;
}
//----------------------------------------------------------------------------------
int     DBAgent::OnGet(const char* pszTableName ,int ret,const Buffer & key,const Buffer & value,const Buffer & cb)
{
    return 0;
}
int     DBAgent::OnMGet(const char* pszTableName ,int ret,const std::vector<Buffer> & keylist,
                     const std::vector<Buffer> & valuelist,
                     const Buffer & cb,bool more)
{
    return 0;
}
int     DBAgent::OnRemove(const char* pszTableName ,int ret,const Buffer & key,const Buffer & cb)
{
    return 0;
}
int     DBAgent::OnUpdate(const char* pszTableName ,int ret,const Buffer & key,const Buffer & cb)
{
    return 0;
}
int     DBAgent::OnInsert(const char* pszTableName ,int ret,const Buffer & key,const Buffer & cb)
{
    return 0;
}
int     DBAgent::OnGetKeyRank(const char* pszTableName ,const Buffer & key,const Buffer & cb)
{
    return 0;
}
int     DBAgent::OnGetRankKey(const char* pszTableName ,int rank,const Buffer & cb)
{
    return 0;
}
int     DBAgent::OnCount(const char* pszTableName ,const Buffer & cb)
{
    return 0;
}
#endif


