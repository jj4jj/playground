#include "base/DateTime.h"
#include "base/Log.h"
#include "base/StringUtil.h"

#include "MysqlAgent.h"
#include "MysqlProxy.h"





#if 1
int     MysqlProxy::Init(const std::vector<MysqlMeta> & metas)
{
    tableMetas.clear();
    for(uint i = 0;i < metas.size(); ++i)
    {
        tableMetas[metas[i].tblname] = metas[i];
    }
    /////////////////////////////////////////////
    return 0;
}
int     MysqlProxy::Connect(const MysqlProxyOption & opt)
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
int     MysqlProxy::SelectDB(const string & name)
{
    string sql = "USE "+name;
    return ExecuteSQL(sql);
}
int     MysqlProxy::CreateDB(const string & name)
{        
    //CREATE DATABASE IF NOT EXISTS yourdbname DEFAULT CHARSET utf8 COLLATE utf8_general_ci;
    string sql = "CREATE DATABASE IF NOT EXISTS "+name;
    sql += " DEFAULT CHARSET utf8 COLLATE utf8_general_ci";
    return ExecuteSQL(sql);
}
int     MysqlProxy::CreateTable(MysqlMeta & meta)
{                
    string sql = "";
    meta.GetCreateTableSQL(sql);
    return ExecuteSQL(sql);
}
int     MysqlProxy::DispatchReq(const MysqlRequest & req)
{
    rsp.tblname = req.tblname;
    rsp.op = req.op;
    rsp.cb = req.cb;
    rsp.ret = MYSQL_RESULT_UNKOWN;
    
    switch(req.op)
    {
        case MYSQL_REQ_SELECT:
            //
            rsp.ret =  Select(req.tblname,req.where,req.fields,rsp.data);
            break;
        case MYSQL_REQ_INSERT:
            rsp.ret =  Insert(req.tblname,req.data);
            break;
        case MYSQL_REQ_UPDATE:
            rsp.ret =  Update(req.tblname,req.where,req.data);
            break;
        case MYSQL_REQ_DELETE:
            rsp.ret =  Delete(req.tblname,req.where);
            break;
        case MYSQL_REQ_CREATE_TB:
        {
            MysqlMeta * p = GetTableMeta(req.tblname);
            if(!p)
            {
                LOG_ERROR("get table meta = %s error !",req.tblname.c_str());
            }
            else
            {
                rsp.ret = CreateTable(*p);
            }
        }
            break;
        default:
            assert(false);
            break;
    }
    //state = PROXY_STATE_DONE;
    if(agent)
    {
        agent->Response(rsp);    
    }
    return 0;
}

MysqlMeta*   MysqlProxy::GetTableMeta(const string & name)
{        
    if( tableMetas.find(name) != tableMetas.end())
    {
        return &(tableMetas[name]);
    }
    return NULL;
}
int     MysqlProxy::Select(const string & tblname,const string & where,
                    const vector<string> & selCols, vector<MysqlField> & data)
{
    MysqlMeta* meta = GetTableMeta(tblname);
    if(!meta)
    {
        return     MYSQL_RESULT_NO_META;
    }
    if(where.length() <= 0)
    {
        return     MYSQL_RESULT_GET_NO_PK;
    }
    string sql ;        
    int ret = meta->GetSelectTableSQL(where,selCols,sql,conn);
    if(ret)
    {
        return MYSQL_RESULT_SQL_GEN_ERR;
    }
    ret = ExecuteSQL(sql);
    if(ret)
    {
        return MYSQL_RESULT_SQL_EXE_ERR ;
    }
    //fetch role
    MYSQL_RES *res_set;
    //it can be instead of mysql_use_result
    res_set = mysql_store_result(conn);  
    if(res_set == NULL)
    {
        LOG_ERROR("mysql_store_result failed %d for %s",
            mysql_errno(conn),mysql_error(conn));
        return MYSQL_RESULT_STORE_RESULT_ERR;
    }        
    //get 
    int nfields = mysql_num_fields(res_set);
    MYSQL_ROW row = mysql_fetch_row(res_set);    
    unsigned long * lens = mysql_fetch_lengths(res_set);
    //meta->Init(cols);
    data.clear();
    ////////////////////////////////////////
    MysqlField col;
    if(row)
    {
        MYSQL_FIELD * field = NULL;
        LOG_DEBUG("select ok cols = %d",nfields);
        for(int i  = 0; i < nfields; i++)
        {
            field = mysql_fetch_field_direct(res_set,i);
            assert(field);
            //meta set col value
            col.name = field->name;
            LOG_DEBUG("get field name = %s data len = %d",field->name,lens[i]);
            meta->SetFieldValue(col,row[i],lens[i]);
            data.push_back(col);
        }
        ret = MYSQL_RESULT_OK;
    }
    else
    {
        ret =  MYSQL_RESULT_NO_DATA;
    }
    
    mysql_free_result(res_set);        
    return ret;        
}

int     MysqlProxy::Insert(const string & table,const std::vector<MysqlField> & data)
{

    MysqlMeta * meta = GetTableMeta(table);
    if(!meta)
    {
        LOG_ERROR("meta not found!");
        return MYSQL_RESULT_NO_META;
    }
    //INSERT INTO <TABLE> (C1,C2)   VALUES(V1,V2);
    string sql = "";
    int ret = meta->GetInsertTableSQL(data,sql,conn);
    if(ret)
    {
        return MYSQL_RESULT_SQL_GEN_ERR;
    }
    ret = ExecuteSQL(sql); 
    if(ret)
    {
        if(GetLastErrNo() == ER_DUP_ENTRY)
        {
            return MYSQL_RESULT_ALREADY_EXIST;
        }
        else
        {
            return MYSQL_RESULT_SQL_EXE_ERR + GetLastErrNo();
        }
    }
    return MYSQL_RESULT_OK;        
}
int     MysqlProxy::Update(const  string &  table,const string & where,const std::vector<MysqlField> & data)
{
    MysqlMeta * meta = GetTableMeta(table);
    if(!meta)
    {
        LOG_ERROR("meta not found!");
        return MYSQL_RESULT_NO_META;
    }
    string sql = "";
    int ret = meta->GetUpdateTableSQL(where, data,sql,conn);
    if(ret)
    {
        return MYSQL_RESULT_SQL_GEN_ERR;
    }
    ret = ExecuteSQL(sql);
    if(ret)
    {
        return MYSQL_RESULT_SQL_EXE_ERR;
    }

    
    return MYSQL_RESULT_OK;        
}
int     MysqlProxy::Delete(const string & table,const string & where)
{
    //delete from <table> where pk
    MysqlMeta * meta = GetTableMeta(table);
    if(!meta)
    {
        LOG_ERROR("meta not found!");
        return MYSQL_RESULT_NO_META;
    }
    //eg. DELETE FROM <TABLE> WHERE  PKS
    string sql = "";
    int ret = meta->GetDeleteTableSQL(where,sql,conn);
    if(ret)
    {
        return MYSQL_RESULT_SQL_GEN_ERR;
    }
    ret = ExecuteSQL(sql); 
    if(ret)
    {
        return MYSQL_RESULT_SQL_EXE_ERR;
    }
    return MYSQL_RESULT_OK;        
}
int     MysqlProxy::ExecuteSQL(const string & sql)
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
void    MysqlProxy::Destroy()
{
    if(conn)
    {
        mysql_close(conn);
    }
}

#endif
