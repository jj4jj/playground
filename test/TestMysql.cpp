#include "db/DBAgent.h"
#include "base/Log.h"
int main()
{
    DBProxy proxy;
    DBTableMeta meta;
    string tblname = "mtest";
    meta.tblname = tblname;
    DBTableFieldMeta entry;
    entry.ispk = true;
    entry.name = "gid";
    entry.type = DBTableFieldMeta::VAL_TYPE_UINT64;
    meta.cols.push_back(entry);
    entry.ispk = false;

    entry.name = "field_int";
    entry.type = DBTableFieldMeta::VAL_TYPE_INT32;
    meta.cols.push_back(entry);
    
    entry.name = "field_blob";
    entry.type = DBTableFieldMeta::VAL_TYPE_BLOB;
    meta.cols.push_back(entry);

    entry.name = "field_varchar";
    entry.type = DBTableFieldMeta::VAL_TYPE_VARCHAR;
    entry.maxlen = 32;
    meta.cols.push_back(entry);

    entry.maxlen = 0;
    entry.name = "field_text";
    entry.type = DBTableFieldMeta::VAL_TYPE_TEXT;
    meta.cols.push_back(entry);

    entry.name = "field_time";
    entry.type = DBTableFieldMeta::VAL_TYPE_DATETIME;
    meta.cols.push_back(entry);
    
    vector < DBTableMeta >  v;
    v.push_back(meta);
    if(proxy.Init(v))
    {
        LOG_FATAL("proxy init error !");
        return -1;
    }

    DBProxyOption option;
    option.cliflag = 0;
    option.dbname = "mtest";
    option.ip = "127.0.0.1";
    option.port = 3306;
    option.uname = "root";
    option.passwd = "123456";
    option.unisock = "";
    if(proxy.Connect(option))
    {
        LOG_FATAL("proxy conn error !");        
        return -1;
    }    
    char line[512];
    string sline;
    DBTableField col;
    vector < DBTableField > pks;
    col.name = "gid";
    col.u_data.u64 = 12345678;
    pks.push_back(col);
    vector < string >  selCols;
    selCols.clear();    

    
    std::vector<DBTableField> data;
    DBTableField d;
    d.name = "gid";
    d.u_data.u64 = 12345678;
    data.push_back(d);
    
    d.buffer.resize(32);

    d.name = "field_int";
    d.u_data.i64 = -1234;
    d.type = DBTableFieldMeta::VAL_TYPE_INT32;
    data.push_back(d);
    
    d.name = "field_blob";
    d.type = DBTableFieldMeta::VAL_TYPE_BLOB;
    memcpy(&d.buffer[0],"text blob",strlen("text block"));

    data.push_back(d);

    d.name = "field_varchar";
    d.type = DBTableFieldMeta::VAL_TYPE_VARCHAR;
    strncpy(&d.buffer[0],"varchar test",d.buffer.size());
    data.push_back(d);

    d.name = "field_text";
    d.type = DBTableFieldMeta::VAL_TYPE_TEXT;
    strncpy(&d.buffer[0],"text test",d.buffer.size());

    data.push_back(d);

    d.name = "field_time";
    d.type = DBTableFieldMeta::VAL_TYPE_DATETIME;
    d.u_data.time = time(NULL);
    data.push_back(d);

    using std::cout;
    using std::cin;
    using std::endl;
    while(true)
    {
        cout<<"mtest>";
        cin.getline(line,sizeof(line));
        sline = line;
        if(sline == "quit")
        {
            return 0;
        }
        int ret = 0;
        
        if(sline == "get")
        {
            vector < DBTableField > res;
            if(proxy.Select(tblname,pks,selCols,res))
            {
                LOG_FATAL("select errror !");
            }
            else
            {
                string vtext;
                vtext = meta.Visual(res);
                cout<<vtext<<endl;
            }            
        }
        if(sline == "insert" )
        {
            ret = proxy.Insert(tblname,data);
        }
        if(sline == "update" )
        {
            data[1].u_data.i32 = -1273 + rand();
            
            ret = proxy.Update(tblname,pks,data);
        }
        if(sline == "delete" )
        {
            ret = proxy.Delete(tblname,pks);
        }
        if(sline == "createdb" )
        {
            ret = proxy.CreateDB("mtest");
        }
        if(sline == "createtb" )
        {
            ret = proxy.CreateTable(meta);
        }
        if(ret)
        {
            LOG_FATAL("%s exec error ret = %d for %s",sline.c_str(),ret,proxy.GetLastErrStr());
        }
        else
        {
            LOG_INFO("%s exec ok !",sline.c_str());
        }
    }
    return 0;
}




