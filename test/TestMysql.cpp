#include "db/MysqlAgent.h"
#include "base/Log.h"
using std::cout;
using std::cin;
using std::endl;
string tblname = "mtest";

class MTest1TableHandler:public MysqlCommandListener
{
public:
    MTest1TableHandler(const MysqlMeta * m)
    {
        meta = (MysqlMeta*) m;
    }
    virtual  int     OnResponse(MysqlResponse & rsp)
    {
        LOG_DEBUG("test1 on get cb size = %u = [%s] ret = %d",rsp.cb.iUsed,(char*)rsp.cb.pBuffer,rsp.ret);
        return 0;
    }
private:
    MysqlMeta* meta;
};

class MTest2TableHandler:public MysqlCommandListener
{
public:
    MTest2TableHandler(const MysqlMeta * m)
    {
        meta = (MysqlMeta*)m;
    }
    virtual  int     OnResponse(MysqlResponse & rsp)
    {
        LOG_DEBUG("test2 on get cb size = %u = [%s] ret = %d",rsp.cb.iUsed,(char*)rsp.cb.pBuffer,rsp.ret);
        return 0;
    }
private:
    MysqlMeta* meta;
};


int TestAgent(const  vector < MysqlProxyOption > & proxyList, 
        const std :: vector < MysqlMeta > & metas,
        std::vector<MysqlField> & data,vector < MysqlField > & pks)
{
    MysqlAgent  agent;
    if(agent.Init(proxyList,metas))
    {
        LOG_ERROR("init error !");
        return -1;
    }
    agent.SetListener(MysqlCommandListenerPtr(new MTest1TableHandler(&(metas[0]))));
    if(agent.Start())
    {
        LOG_ERROR("start agent error !");
        return -1;
    }
    char line[512];
    string sline;
    MysqlRequest  req;

    Buffer cb;
    cb.Create(20);
    strncpy((char*)&(cb.pBuffer[0]),"this is a cb",cb.iUsed-1);
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
            req.op = MYSQL_REQ_CREATE_TB;
            req.tblname = tblname;
            ret = agent.Request(req);
        }
        if(sline == "get")
        {
            req.Init();
            req.op = MYSQL_REQ_SELECT;
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
            req.op = MYSQL_REQ_INSERT;
            req.cb = cb;
            req.tblname = tblname;
            req.data = data;
            ret = agent.Request(req);
        }
        if(sline == "update" )
        {            
            data[1].u_data.i32 = -1273 + rand();
            req.Init();
            req.op = MYSQL_REQ_UPDATE;
            req.tblname = tblname;            
            req.cb = cb;
            req.data = data;
            ret = agent.Request(req);
            
        }
        if(sline == "delete" )
        {
            req.Init();
            req.op = MYSQL_REQ_DELETE;
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
int TestProxy(const MysqlProxyOption & option, vector < MysqlMeta > & v,
             std::vector<MysqlField> & data,vector < MysqlField > & pks,
             vector < string > & selCols)
{
    MysqlProxy proxy(NULL);
    MysqlMeta & meta = v[0];
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
            vector < MysqlField > res;
            if(proxy.Select(tblname,string("where 1"),selCols,res))
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
            
            ret = proxy.Update(tblname,string("where 1"),data);
        }
        if(sline == "delete" )
        {
            ret = proxy.Delete(tblname,string("where 1"));
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
    MysqlMeta meta;
    meta.tblname = tblname;
    MysqlFieldMeta field;
    field.ispk = true;
    field.name = "gid";
    field.type = MysqlFieldMeta::VAL_TYPE_UINT64;
    meta.cols.push_back(field);
    field.ispk = false;

    field.name = "field_int";
    field.type = MysqlFieldMeta::VAL_TYPE_INT32;
    meta.cols.push_back(field);
    
    field.name = "field_blob";
    field.type = MysqlFieldMeta::VAL_TYPE_BLOB;
    meta.cols.push_back(field);

    field.name = "field_varchar";
    field.type = MysqlFieldMeta::VAL_TYPE_VARCHAR;
    field.maxlen = 32;
    meta.cols.push_back(field);

    field.maxlen = 0;
    field.name = "field_text";
    field.type = MysqlFieldMeta::VAL_TYPE_TEXT;
    meta.cols.push_back(field);

    field.name = "field_time";
    field.type = MysqlFieldMeta::VAL_TYPE_DATETIME;
    meta.cols.push_back(field);
    
    vector < MysqlMeta >  tableMetas;
    tableMetas.push_back(meta);

    /////////////////////////////////////




    MysqlProxyOption option;
    option.cliflag = 0;
    option.dbname = "mtest";
    option.ip = "127.0.0.1";
    option.port = 3306;
    option.uname = "root";
    option.passwd = "123456";
    option.unisock = "";

    std::vector<MysqlProxyOption>  proxyList;
    proxyList.push_back(option);
    /////////////////////////////////////


    
    MysqlField col;
    vector < MysqlField > pks;
    col.name = "gid";
    col.u_data.u64 = 12345678;
    pks.push_back(col);
    /////////////////////////////////////


    
    vector < string >  selCols;
    selCols.clear();
    /////////////////////////////////////



    std::vector<MysqlField> data;
    MysqlField d;
    d.name = "gid";
    d.u_data.u64 = 12345678;
    data.push_back(d);
    
    d.buffer.resize(32);

    d.name = "field_int";
    d.u_data.i64 = -1234;
    d.type = MysqlFieldMeta::VAL_TYPE_INT32;
    data.push_back(d);
    
    d.name = "field_blob";
    d.type = MysqlFieldMeta::VAL_TYPE_BLOB;
    memcpy(&d.buffer[0],"text blob",strlen("text block"));

    data.push_back(d);

    d.name = "field_varchar";
    d.type = MysqlFieldMeta::VAL_TYPE_VARCHAR;
    strncpy(&d.buffer[0],"varchar test",d.buffer.size());
    data.push_back(d);

    d.name = "field_text";
    d.type = MysqlFieldMeta::VAL_TYPE_TEXT;
    strncpy(&d.buffer[0],"text test",d.buffer.size());

    data.push_back(d);

    d.name = "field_time";
    d.type = MysqlFieldMeta::VAL_TYPE_DATETIME;
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




