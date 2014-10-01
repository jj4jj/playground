#include "db/MysqlAgent.h"
#include "base/Log.h"
using std::cout;
using std::cin;
using std::endl;
string tblname = "mtest";

class MTest1TableHandler:public DBTableHandler
{
public:
    MTest1TableHandler(const DBTableMeta * m)
    {
        meta = (DBTableMeta*) m;
    }
	virtual int 	OnGet(int ret,vector<DBTableField> & data,vector<uint8_t>  & cb)
	{
        LOG_DEBUG("on get cb size = %u = [%s] ret = %d",cb.size(),(char*)&cb[0],ret);
        if(!ret)
        {
            LOG_INFO("get result :\n%s",meta->Visual(data).c_str());            
        }
        else
        {
            LOG_FATAL("error = %d when get !",ret);
        }
        return 0;
    }
private:
    DBTableMeta* meta;
};

class MTest2TableHandler:public DBTableHandler
{
public:
    MTest2TableHandler(const DBTableMeta * m)
    {
        meta = (DBTableMeta*)m;
    }
	virtual int 	OnGet(int ret,vector<DBTableField> & data,vector<uint8_t>  & cb)
	{
        LOG_DEBUG("on get cb size = %u cb = %s ",cb.size(),(char*)&cb[0]);
        if(!ret)
        {
            LOG_INFO("get result :\n%s",meta->Visual(data).c_str());            
        }
        else
        {
            LOG_FATAL("error = %d when get !",ret);
        }
        return 0;
    }
private:
    DBTableMeta* meta;
};


int TestAgent(const  vector < DBProxyOption > & proxyList, 
        const std :: vector < DBTableMeta > & metas,
        std::vector<DBTableField> & data,vector < DBTableField > & pks)
{
    MysqlAgent  agent;
    if(agent.Init(proxyList,metas))
    {
        LOG_ERROR("init error !");
        return -1;
    }    
    agent.RegisterTableHandler("mtest1", DBTableHandlerPtr(new MTest1TableHandler(&(metas[0]))));
    agent.RegisterTableHandler("mtest2", DBTableHandlerPtr(new MTest2TableHandler(&(metas[1]))));
    if(agent.Start())
    {
        LOG_ERROR("start agent error !");
        return -1;
    }
    char line[512];
    string sline;
    DBTableOpReq  req;

    vector<uint8_t> cb;
    cb.resize(20);
    strncpy((char*)&(cb[0]),"this is a cb",cb.size()-1);
    while(true)
    {
        agent.Polling(2);
        /////////////////
        cout<<"TestAgent>";
        cin.getline(line,sizeof(line));
        sline = line;
        if(sline == "quit")
        {
            return 0;
        }
        if(sline.length() < 1)
        {
            continue;
        }
        int ret = 0;
        if(sline == "s1")
        {
            tblname = "mtest1";
        }
        if(sline == "s2")
        {
            tblname = "mtest2";
        }
        if(sline == "create")
        {
            req.op = OP_CREATE_TB;
            req.tblname = tblname;
            ret = agent.Request(req);
        }
        if(sline == "get")
        {
            req.Init();
            req.op = OP_SELECT;
            req.tblname = tblname;
            req.cb = cb;
            req.data = pks;
            req.fields.push_back(string("gid"));
            req.fields.push_back(string("field_blob"));
            req.fields.push_back(string("field_time"));
            ret = agent.Request(req);
        }
        if(sline == "insert" )
        {
            req.data = data;
            data[1].u_data.i32 = 80000;
            req.Init();
            req.op = OP_INSERT;
            req.cb = cb;
            req.tblname = tblname;
            req.data = data;
            ret = agent.Request(req);
        }
        if(sline == "update" )
        {            
            data[1].u_data.i32 = -1273 + rand();
            req.Init();
            req.op = OP_UPDATE;
            req.tblname = tblname;            
            req.cb = cb;
            req.data = data;
            ret = agent.Request(req);
            
        }
        if(sline == "delete" )
        {
            req.Init();
            req.op = OP_DELETE;
            req.tblname = tblname;            
            req.data = pks;
            req.cb = cb;
            ret = agent.Request(req);
        }
        LOG_DEBUG("request ret = %d",ret);

    }
    agent.Destroy();    
    //get 
    //remove
    //update
    //insert
    return 0;
}
int TestProxy(const DBProxyOption & option, vector < DBTableMeta > & v,
             std::vector<DBTableField> & data,vector < DBTableField > & pks,
             vector < string > & selCols)
{
    MysqlProxy proxy(NULL);
    DBTableMeta & meta = v[0];
    if(proxy.Init(v))
    {
        LOG_FATAL("proxy init error !");
        return -1;
    }
    if(proxy.Connect(option))
    {
        LOG_FATAL("proxy conn error !");        
        return -1;
    }
    char line[512];
    string sline;
    
    while(true)
    {
        cout<<"TestProxy>";
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
}
int main(int argc , char* argv[])
{
    if(argc < 2)
    {
        LOG_ERROR("usage:%s agent|proxy",argv[0]);
        return -1;
    }
    DBTableMeta meta;
    meta.tblname = tblname;
    DBTableFieldMeta field;
    field.ispk = true;
    field.name = "gid";
    field.type = DBTableFieldMeta::VAL_TYPE_UINT64;
    meta.cols.push_back(field);
    field.ispk = false;

    field.name = "field_int";
    field.type = DBTableFieldMeta::VAL_TYPE_INT32;
    meta.cols.push_back(field);
    
    field.name = "field_blob";
    field.type = DBTableFieldMeta::VAL_TYPE_BLOB;
    meta.cols.push_back(field);

    field.name = "field_varchar";
    field.type = DBTableFieldMeta::VAL_TYPE_VARCHAR;
    field.maxlen = 32;
    meta.cols.push_back(field);

    field.maxlen = 0;
    field.name = "field_text";
    field.type = DBTableFieldMeta::VAL_TYPE_TEXT;
    meta.cols.push_back(field);

    field.name = "field_time";
    field.type = DBTableFieldMeta::VAL_TYPE_DATETIME;
    meta.cols.push_back(field);
    
    vector < DBTableMeta >  tableMetas;
    tableMetas.push_back(meta);

    /////////////////////////////////////




    DBProxyOption option;
    option.cliflag = 0;
    option.dbname = "mtest";
    option.ip = "127.0.0.1";
    option.port = 3306;
    option.uname = "root";
    option.passwd = "123456";
    option.unisock = "";

    std::vector<DBProxyOption>  proxyList;
    proxyList.push_back(option);
    /////////////////////////////////////


    
    DBTableField col;
    vector < DBTableField > pks;
    col.name = "gid";
    col.u_data.u64 = 12345678;
    pks.push_back(col);
    /////////////////////////////////////


    
    vector < string >  selCols;
    selCols.clear();
    /////////////////////////////////////



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
    /////////////////////////////////////

    if(string(argv[1]) == "agent")
    {
        proxyList.push_back(option);
        tableMetas.clear();
        meta.tblname = "mtest1";
        tableMetas.push_back(meta);
        meta.tblname = "mtest2";
        tableMetas.push_back(meta);

        return TestAgent(proxyList,tableMetas,data,pks);
    }
    if(string(argv[1]) == "proxy")
    {
        return TestProxy(option,tableMetas,data,pks,selCols);
    }
    
    return 0;
}




