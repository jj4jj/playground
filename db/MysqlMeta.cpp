#include "MysqlMeta.h"
#include "base/Log.h"
#include "base/DateTime.h"

#if 1
const char* MysqlMeta::GetTypeString(uint8_t type)
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
    assert(type < MysqlFieldMeta::VAL_TYPE_MAX);
    return type2str[type];
}
int MysqlMeta::GetCreateTableSQL(string & sql)
{
/*
eg.
CREATE TABLE IF NOT EXISTS company_user(     
id int(10) UNSIGNED AUTO_INCREMENT NOT NULL,     
username VARCHAR(24) NOT NULL UNIQUE,     
password VARCHAR(8) NOT NULL,     
company_name VARCHAR(250) NOT NULL UNIQUE,     
company_type VARCHAR(30) NOT NULL,     
company_intro TEXT,     
office_image TEXT,     
register_date DATE NOT NULL,
PRIMARY KEY()
)ENGINE=InnoDB DEFAULT CHARSET utf8 COLLATE utf8_general_ci;
*/
    sql = "CREATE TABLE IF NOT EXISTS "+tblname + " (";
    char tmpBuffer[128];
    string pks = "";
    for(uint i = 0;i < cols.size(); ++i)
    {
        sql += cols[i].name + " "+ GetTypeString(cols[i].type);
        if(cols[i].maxlen > 0 && cols[i].type == MysqlFieldMeta::VAL_TYPE_VARCHAR)
        {
            snprintf(tmpBuffer,sizeof(tmpBuffer),"(%d)",cols[i].maxlen);
            sql += tmpBuffer;
        }
        sql += " NOT NULL";
        sql += ",\n";
    }
    for(uint i = 0;i < cols.size(); ++i)
    {
        if(cols[i].ispk)
        {
            if(i > 0)
            {
                pks += ",";
            }
            pks += cols[i].name;
        }
    }
    sql += "PRIMARY KEY (";
    sql += pks + ")\n";
    sql += ")ENGINE=InnoDB DEFAULT CHARSET utf8 COLLATE utf8_general_ci";
    return 0;
}
int MysqlMeta::Init(vector<MysqlField> & colvs)
{
    colvs.clear();
    MysqlField ec;
    for(uint i = 0;i < cols.size(); ++i)
    {
        ec.name = cols[i].name;
        ec.type = cols[i].type;
        colvs.push_back(ec);
    }
    return 0;
}
int MysqlMeta::GetUpdateTableSQL(const string & where,const vector<MysqlField> & data,string & sql,MYSQL   *conn)
{
    //eg. UPDATE <TABLE> SET C1=V1,C2=V2 WHERE 
    sql = "UPDATE ";
    sql += tblname + " SET ";
    int first = 0;
    for(uint i = 0; i < data.size(); ++i)
    {
        MysqlFieldMeta * field = GetFieldMeta(*this,data[i].name);
        if(field->ispk)
        {
            continue;
        }
        if(first != 0)
        {
            sql += ",";
        }
        sql += data[i].name ;
        sql += "=";        
        sql += GetFieldValueSQL(data[i],conn);
        ++first;
    }        
    sql += " WHERE ";
    sql += where;
    /*
    for(uint i = 0; i < pks.size(); ++i)
    {
        sql += pks[i].name + "=";
        sql += GetFieldValueSQL(pks[i],conn);
        if(i + 1 < pks.size())
        {
            sql += " AND ";
        }
    } 
    */
    return 0;
}
int MysqlMeta::GetInsertTableSQL(const vector<MysqlField> & cols,string & sql,MYSQL   *conn)
{
    //sql inject checking
    sql = "INSERT INTO ";
    sql += tblname + "(";
    for(uint i = 0; i < cols.size(); ++i)
    {
        if(i > 0)
        {
            sql += ",";
        }
        sql += cols[i].name ;
    }        
    sql += ") VALUES(";
    for(uint i = 0; i < cols.size(); ++i)
    {
        if(i > 0)
        {
            sql += ",";
        }
        sql += GetFieldValueSQL(cols[i],conn);
    }        
    sql += ")";//blabla
    return 0;
}
int MysqlMeta::GetDeleteTableSQL(const string & where,string & sql,MYSQL   *conn)
{
    //eg. DELETE FROM TABLE WHERE X=X
    sql = "DELETE FROM ";
    sql += tblname;
    sql += " WHERE ";
    sql += where;
    /*
    for(uint i = 0;i < pks.size(); ++i)
    {   
        if(pks[i].type == MysqlFieldMeta::VAL_TYPE_DATETIME)
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
    */
    return 0;
}
int MysqlMeta::GetSelectTableSQL(const string & where,const vector<string> & selCols,string & sql,MYSQL   *conn)
{
    //eg. SELECT FROM TABLE WHERE X=X
    sql = "SELECT ";
    vector<string>  realFieldSels;        
    if(selCols.empty())
    {
        sql += "*";
    }
    else
    {
        for(uint i = 0 ;i < cols.size(); ++i)
        {
            if(cols[i].ispk)
            {
                realFieldSels.push_back(cols[i].name);
            }
            else if(std::find(selCols.begin(),selCols.end(),cols[i].name) != selCols.end())
            {
                realFieldSels.push_back(cols[i].name);
            }
        }
        // --------------------------------------------------------------------------
        for(uint i = 0 ;i < realFieldSels.size(); ++i)
        {
            if(i > 0)
            {
                sql += ",";
            }
            sql += realFieldSels[i];
        }
    }
    sql += " FROM ";
    sql += tblname;
    sql += " WHERE ";
    sql += where;
    /*
    for(uint i = 0;i < pks.size(); ++i)
    {   
        if(pks[i].type == MysqlFieldMeta::VAL_TYPE_DATETIME)
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
    */
    return 0;        
}
string   MysqlMeta::GetFieldValueSQL(const MysqlField & col,MYSQL   *conn )
{
    //todo check string for prevent sql injection
    string sqlv = "";//"'";
    char digitbuffer[20];
    MysqlFieldMeta * fieldMeta = GetFieldMeta(*this,col.name);
    assert(fieldMeta);
    switch(fieldMeta->type)
    {
       case  MysqlFieldMeta::VAL_TYPE_INT32      :
           snprintf(digitbuffer,sizeof(digitbuffer),"%d",col.u_data.i32);
           break;
       case  MysqlFieldMeta::VAL_TYPE_INT64      :
           snprintf(digitbuffer,sizeof(digitbuffer),"%ld",col.u_data.i64);
           break;
       case  MysqlFieldMeta::VAL_TYPE_UINT32     :
           snprintf(digitbuffer,sizeof(digitbuffer),"%u",col.u_data.u32);
           break;
       case  MysqlFieldMeta::VAL_TYPE_UINT64     :
           snprintf(digitbuffer,sizeof(digitbuffer),"%lu",col.u_data.u64);
           break;
       case  MysqlFieldMeta::VAL_TYPE_REAL32     :
           snprintf(digitbuffer,sizeof(digitbuffer),"%f",col.u_data.r32);
           break;
       case  MysqlFieldMeta::VAL_TYPE_REAL64     :
           snprintf(digitbuffer,sizeof(digitbuffer),"%lf",col.u_data.r64);
           break;
       case  MysqlFieldMeta::VAL_TYPE_VARCHAR    ://255
       case  MysqlFieldMeta::VAL_TYPE_TEXT       ://64K
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
       case  MysqlFieldMeta::VAL_TYPE_BLOB       ://64K
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
       case  MysqlFieldMeta::VAL_TYPE_DATETIME   ://TIME
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
int MysqlMeta::SetFieldValue(MysqlField & field,char* data,uint64_t datalen)
{
    //LOG_DEBUG("set field value name = %s type = %d data = [%s] len = %lu",
    //         field.name.c_str(),field.type,data,datalen);

    MysqlFieldMeta * fieldMeta = GetFieldMeta(*this,field.name);
    if(!fieldMeta)
    {
        LOG_ERROR("no field = %s meta found !",field.name.c_str());
        return -1;
    }
    field.Init();
    field.name = fieldMeta->name;
    field.type = fieldMeta->type;    

    char numberBuffer[32];
    int minlen = std::min(datalen , sizeof(numberBuffer) - 1);
    memcpy(numberBuffer,data,minlen);
    numberBuffer[minlen] = 0;
    
    switch(field.type)
    {
       case  MysqlFieldMeta::VAL_TYPE_INT32      :
       case  MysqlFieldMeta::VAL_TYPE_INT64      :
           field.u_data.i64 = strtol(numberBuffer,NULL,10);
           break;
       case  MysqlFieldMeta::VAL_TYPE_UINT32     :
       case  MysqlFieldMeta::VAL_TYPE_UINT64     :
           field.u_data.i64 = strtoul(numberBuffer,NULL,10);
           break;
       case  MysqlFieldMeta::VAL_TYPE_REAL32     :
           field.u_data.r32 = atof(numberBuffer);
           break;
       case  MysqlFieldMeta::VAL_TYPE_REAL64     :
           field.u_data.r64 = atof(numberBuffer);
           break;
       case  MysqlFieldMeta::VAL_TYPE_VARCHAR    ://255
       case  MysqlFieldMeta::VAL_TYPE_TEXT       ://64K
       {
            field.buffer.resize(datalen + 1);
            memcpy(&field.buffer[0],data,datalen);
            field.buffer[datalen] = 0;
            break;
       }
       case  MysqlFieldMeta::VAL_TYPE_BLOB       ://64K
       {
            field.buffer.resize(datalen);
            memcpy(&field.buffer[0],data,datalen);
            break;
       }
       case  MysqlFieldMeta::VAL_TYPE_DATETIME   ://TIME
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
string    MysqlMeta::Visual(const std::vector<MysqlField>   & vc)
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
MysqlFieldMeta *   MysqlMeta::GetFieldMeta(MysqlMeta & table,const string & fieldName)
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

MysqlField * MysqlMeta::GetField(std::vector<MysqlField> & fields,const char* pszFieldName)
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



