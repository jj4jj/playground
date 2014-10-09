#include "base/Log.h"
#include "MemSerializer.h"
#include "RedisAgent.h"
#include "hiredis.h"
#include "DataAgent.h"

//#include "regex.h"
using namespace google::protobuf;


#define INSERT_CMD  ("INSERT")
#define GET_CMD     ("GET")
#define REMOVE_CMD  ("DEL")
#define UPDATE_CMD  ("SET")
#define DELIM1       (string(" "))
#define DELIM2       (string(":"))
#define DELIM3       (string("&"))
#define DELIM4       (string("|"))

#if 1
class DBAgentCommandListener : public MysqlCommandListener
{
private :
    DBAgent *     agent;
    /////////////////////////////////////////////////////////////
public :
    DBAgentCommandListener(DBAgent* _agent):agent(_agent){}
public :
    virtual  int     OnResponse(MysqlResponse & rsp)
    {
        return agent->DispatchResult(rsp);
    }
};
#endif


#if 1
int  DBAgent::DispatchResult(MysqlResponse & rsp)
{    
    int ret = rsp.ret;
    Buffer & cb = rsp.cb;
    if(ret ==  MYSQL_RESULT_NO_DATA)
    {
        ret = DATA_NO_EXISTS;
    }
    else if(ret == MYSQL_RESULT_TIMEOUT)
    {
        ret = DATA_TIME_OUT;
    }
    else if(ret == MYSQL_RESULT_ALREADY_EXIST)
    {
        ret = DATA_ALREADY_EXISTS;
    }
    else if(ret != MYSQL_RESULT_OK)
    {
        ret = DATA_MYSQL_ERR_START + ret;
    }
    string type = rsp.tblname;
    if(m_mpListener.find(rsp.tblname) != m_mpListener.end())
    {
        if(rsp.op == MYSQL_REQ_SELECT)
        {          
            void* pObj = NULL;
            if(MYSQL_RESULT_OK == ret)
            {
                ret = CreateObjectFromMysql(rsp,&pObj);
            }
            ret = m_mpListener[type]->OnGet(ret,pObj,cb);
        }
        else if(rsp.op == MYSQL_REQ_UPDATE)
        {
            ret = m_mpListener[type]->OnUpdate(ret,cb);
        }
        else if(rsp.op == MYSQL_REQ_INSERT)
        {
            ret = m_mpListener[type]->OnInsert(ret,cb);
        }
        else if(rsp.op == MYSQL_REQ_DELETE)
        {
            ret = m_mpListener[type]->OnRemove(ret,cb);
        }
        else
        {
            LOG_ERROR("not support cmd = %d",rsp.op);
            ret = -1;
        }
    }
    else
    {
        LOG_ERROR("type = %s listener not found !",type.c_str());
        ret = -1;
    }
    ///////////////////////////////////////////////////////////////
    cb.Destroy();
    return ret;
}
int  DBAgent::AddListener(string typeName,DataListenerPtr ptr)
{
    if(m_mpListener[typeName])
    {
        LOG_FATAL("repeat add cache listener = %s",typeName.c_str());
        return -1;
    }
    m_mpListener[typeName] = ptr;
    return 0;
}
int DBAgent::GenerateMysqlMetas(vector<MysqlMeta> & metas,const vector<string> & tables)
{
    assert(serializer);
    using google::protobuf::Descriptor;
    using google::protobuf::FieldDescriptor;
    metas.clear();
    for(uint i = 0;i < tables.size(); ++i)
    {
        MysqlMeta   rowMeta;
        rowMeta.tblname = tables[i];
        //////////////////////////////////
        const Descriptor * desc = serializer->GetDescriptor(tables[i]);
        if(!desc)
        {
            LOG_FATAL("not found table = %s descriptor !",tables[i].c_str());
            return -1;
        }
        vector<string>  pks = serializer->GetTypePrimaryKey(desc);
        if(pks.empty())
        {
            LOG_FATAL("table = %s has no primary key !",tables[i].c_str());
            return -1;
        }
        for(int j = 0;j < desc->field_count(); ++j)
        {
            MysqlFieldMeta  fieldMeta;     
            const FieldDescriptor * field = desc->field(j);
            fieldMeta.name = field->name();
            fieldMeta.maxlen = 0;
            fieldMeta.typeName = field->type_name();
            if(std::find(pks.begin(),pks.end(),fieldMeta.name) != pks.end())
            {
                fieldMeta.ispk = 1;
            }
            switch(field->cpp_type())
            {
                case FieldDescriptor::CPPTYPE_BOOL:
                case FieldDescriptor::CPPTYPE_ENUM:
                case FieldDescriptor::CPPTYPE_INT32:
                    fieldMeta.type = MysqlFieldMeta::VAL_TYPE_INT32;
                    break;
                case FieldDescriptor::CPPTYPE_INT64:
                    fieldMeta.type = MysqlFieldMeta::VAL_TYPE_INT64;
                    break;
                case FieldDescriptor::CPPTYPE_STRING:
                    fieldMeta.type = MysqlFieldMeta::VAL_TYPE_VARCHAR;
                    break;
                case FieldDescriptor::CPPTYPE_UINT32:
                    fieldMeta.type = MysqlFieldMeta::VAL_TYPE_UINT32;
                    break;
                case FieldDescriptor::CPPTYPE_UINT64:
                    fieldMeta.type = MysqlFieldMeta::VAL_TYPE_UINT64;
                    break;
                case FieldDescriptor::CPPTYPE_FLOAT:
                    fieldMeta.type = MysqlFieldMeta::VAL_TYPE_REAL32;
                    break;
                case FieldDescriptor::CPPTYPE_DOUBLE:
                    fieldMeta.type = MysqlFieldMeta::VAL_TYPE_REAL64;
                    break;
                case FieldDescriptor::CPPTYPE_MESSAGE:
                    fieldMeta.type = MysqlFieldMeta::VAL_TYPE_BLOB;                    
                    break;  
                default:
                    LOG_FATAL("unsupport type = %d",field->cpp_type());
                    return -1;
                    break;
            }//end switch filed type   
            rowMeta.cols.push_back(fieldMeta);
        }//end for fields    
        metas.push_back(rowMeta);
    }//end for table meta    
    return 0;
}
int  DBAgent::Init(const DBAgentOption  & cao,MemSerializer * seri)
{
    vector<MysqlMeta>    metas;
    serializer = seri;
    if(GenerateMysqlMetas(metas,cao.tableTypes))
    {
        LOG_ERROR("generate db table metas error !");
        return -1;
    }
    if(mysql.Init(cao.addrList,metas) ) 
    {
        LOG_ERROR("redis agent init error !");
        return -1;
    }            
    return 0;
}
int  DBAgent::Polling(int    iProcPerTick)
{
    return mysql.Polling(iProcPerTick);
}
int  DBAgent::Destroy()
{
    serializer = NULL;
    mysql.Destroy();    
    return 0;
}
#endif

#if 1
int  DBAgent::GenerateMysqlFields(vector<MysqlField> & data,void* obj,vector<string> * fields)
{
    MemSerializer::MetaObject * metaObj = (MemSerializer::MetaObject*)obj;
    int count = metaObj->GetDescriptor()->field_count();
    MysqlField    dbfield;
    for(int i = 0 ;i < count ; ++i)
    {
        dbfield.Init();
        const FieldDescriptor * field =  metaObj->GetDescriptor()->field(i);
        if(!fields ||
           fields->empty() ||
           std::find(fields->begin(),fields->end(),field->name() ) != fields->end() )
        {
            dbfield.name = field->name();
            switch(field->cpp_type())
            {
                case FieldDescriptor::CPPTYPE_INT32:
                    dbfield.type = MysqlFieldMeta::VAL_TYPE_INT32;
                    dbfield.u_data.i32 = \
                        metaObj->GetReflection()->GetInt32(*metaObj,field);                    
                    break;
                case FieldDescriptor::CPPTYPE_INT64:
                    dbfield.type = MysqlFieldMeta::VAL_TYPE_INT64;
                    dbfield.u_data.i64 = \
                        metaObj->GetReflection()->GetInt64(*metaObj,field);                    
                    break;
                case FieldDescriptor::CPPTYPE_UINT32:
                    dbfield.type = MysqlFieldMeta::VAL_TYPE_UINT32;
                    dbfield.u_data.u32 = \
                        metaObj->GetReflection()->GetUInt32(*metaObj,field);                    
                    break;
                case FieldDescriptor::CPPTYPE_UINT64:
                    dbfield.type = MysqlFieldMeta::VAL_TYPE_UINT64;
                    dbfield.u_data.u64 = \
                        metaObj->GetReflection()->GetUInt64(*metaObj,field);                    
                    break;
                case FieldDescriptor::CPPTYPE_FLOAT:
                    dbfield.type = MysqlFieldMeta::VAL_TYPE_REAL32;
                    dbfield.u_data.r32 = \
                        metaObj->GetReflection()->GetFloat(*metaObj,field);                    
                    break;
                case FieldDescriptor::CPPTYPE_DOUBLE:
                    dbfield.type = MysqlFieldMeta::VAL_TYPE_REAL64;
                    dbfield.u_data.r64 = \
                        metaObj->GetReflection()->GetDouble(*metaObj,field);                    
                    break;
                case FieldDescriptor::CPPTYPE_BOOL:
                    dbfield.type = MysqlFieldMeta::VAL_TYPE_INT32;
                    dbfield.u_data.i32 = \
                        metaObj->GetReflection()->GetBool(*metaObj,field)?1:0;                    
                    break;
                case FieldDescriptor::CPPTYPE_ENUM:
                    dbfield.type = MysqlFieldMeta::VAL_TYPE_INT32;
                    dbfield.u_data.i32 = \
                        metaObj->GetReflection()->GetEnum(*metaObj,field)->number();                    
                    break;
                case FieldDescriptor::CPPTYPE_STRING:
                {
                    dbfield.type = MysqlFieldMeta::VAL_TYPE_VARCHAR;
                    string str = metaObj->GetReflection()->GetString(*metaObj,field);                    
                    dbfield.buffer.resize(str.length()+1);
                    memcpy(&(dbfield.buffer[0]),str.c_str(),str.length()+1);
                }                    
                    break;
                case FieldDescriptor::CPPTYPE_MESSAGE:
                {
                    dbfield.type = MysqlFieldMeta::VAL_TYPE_BLOB;
                    const Message & msg = metaObj->GetReflection()->GetMessage(*metaObj,field);                    
                    dbfield.buffer.resize(msg.ByteSize());
                    if(!msg.SerializeToArray(&(dbfield.buffer[0]),dbfield.buffer.size()))
                    {
                        LOG_ERROR("obj type = %s filed = %s pack error !",
                                  metaObj->GetTypeName().c_str(),field->name().c_str());
                        return -1;
                    }
                }
                    break;
                default:
                    LOG_FATAL("unkown filed type = %d", field->cpp_type());
                    return -1;                    
            }            
        }
        else
        {
            continue;        
        }
    }
    return 0;
}
int  DBAgent::CreateObjectFromMysql(MysqlResponse & rsp,void ** ppObj)
{
    *ppObj = NULL;
    if(rsp.ret != MYSQL_RESULT_OK ||
       rsp.data.empty())
    {
        return DATA_INTERNAL_ERR;
    }
    /////////////////////////////////    
    MemSerializer::MetaObject* obj = serializer->NewObject(rsp.tblname);    
    MemSerializer::MetaObjectPtr   ptrObj = MemSerializer::MetaObjectPtr(obj);
    for(uint i = 0;i < rsp.data.size(); ++i)
    {
        MysqlField & mf = rsp.data[i];
        MysqlMeta   * meta = mysql.GetTableMeta(rsp.tblname);
        if(!meta)
        {
            LOG_ERROR("meta = %s not found in mysql meta list",rsp.tblname.c_str());
            return DATA_UNPACK_ERROR;
        }
        MysqlFieldMeta * fieldMeta = MysqlMeta::GetFieldMeta(*meta,mf.name);
        if(!fieldMeta)
        {
            LOG_ERROR("meta = %s not found field meta = %s ",
                rsp.tblname.c_str(),mf.name.c_str());
            return DATA_UNPACK_ERROR;
        }
               
        MemSerializer::MetaObject * fieldObj = NULL;        
        if(MysqlFieldMeta::VAL_TYPE_BLOB == mf.type)
        {
            Buffer buffer(&(mf.buffer[0]),mf.buffer.size());
            if(serializer->UnPack(fieldMeta->typeName.c_str(),buffer,(void**)&fieldObj))
            {
                LOG_ERROR("meta = %s field = %s type = %s unpack error !",
                         rsp.tblname.c_str(),
                         fieldMeta->name.c_str(),fieldMeta->typeName.c_str());
                return DATA_UNPACK_ERROR;
            }
            serializer->SetObjectFieldOBJ(obj,mf.name,fieldObj);
        }
        else if(MysqlFieldMeta::VAL_TYPE_INT32 == mf.type)
        {
            serializer->SetObjectFieldI32(obj,mf.name,mf.u_data.i32);
        }
        else if(MysqlFieldMeta::VAL_TYPE_INT64 == mf.type)
        {
            serializer->SetObjectFieldI64(obj,mf.name,mf.u_data.i64);
        }
        else if(MysqlFieldMeta::VAL_TYPE_UINT32 == mf.type)
        {
            serializer->SetObjectFieldU32(obj,mf.name,mf.u_data.u32);
        }
        else if(MysqlFieldMeta::VAL_TYPE_UINT64 == mf.type)
        {
            serializer->SetObjectFieldU64(obj,mf.name,mf.u_data.u64);
        }
        else if(MysqlFieldMeta::VAL_TYPE_REAL32 == mf.type)
        {
            serializer->SetObjectFieldR32(obj,mf.name,mf.u_data.r32);
        }
        else if(MysqlFieldMeta::VAL_TYPE_REAL64 == mf.type)
        {
            serializer->SetObjectFieldR64(obj,mf.name,mf.u_data.r64);
        }
        else if( MysqlFieldMeta::VAL_TYPE_VARCHAR == mf.type ||
                 MysqlFieldMeta::VAL_TYPE_TEXT == mf.type)
        {
            string  svar(&mf.buffer[0]);
            serializer->SetObjectFieldSTR(obj,mf.name,svar);
        }
        else if(MysqlFieldMeta::VAL_TYPE_DATETIME == mf.type)
        {
            serializer->SetObjectFieldU32(obj,mf.name,mf.u_data.u32);
        }
        else
        {
            continue;
        }
    }
    //ok
    *ppObj = ptrObj.get();
    string primarykey ;
    if(GetPrimaryKey(*ppObj,primarykey))
    {
        m_ptrTmpObj = ptrObj;
    }
    else
    {
        m_mpGetObjects[primarykey] = ptrObj;
    }
    return 0;   
}

int  DBAgent::GetPrimaryKey(void* obj,string & key)
{
    Message * msg = (Message*) obj;
    const Descriptor* desc = msg->GetDescriptor();
    std::vector<string> &   pks = serializer->GetTypePrimaryKey(desc);
    if(pks.empty())
    {
        return -1;
    }
    //key:type#key&key
    key = desc->name()+DELIM2;
    for(uint i = 0; i < pks.size(); ++i)
    {
        if(i > 0)
        {
            key += " AND ";
        }  
        ///////////////////
        //obj:pks[i]
        const FieldDescriptor*    field = desc->FindFieldByName(pks[i]);
        if(!msg->GetReflection()->HasField(*msg,field))
        {
            LOG_ERROR("msg = %s has no field = %s",desc->name().c_str(),field->name().c_str());
            return -1;
        }
        char keyBuffer[512];
        switch(field->cpp_type())
        {
            case FieldDescriptor::CPPTYPE_INT32:
            {
                int32_t v = msg->GetReflection()->GetInt32(*msg,field);
                snprintf(keyBuffer,sizeof(keyBuffer),"%d",v);                
                break;
            }
            case FieldDescriptor::CPPTYPE_INT64:
            {
                int64_t v = msg->GetReflection()->GetInt64(*msg,field);
                snprintf(keyBuffer,sizeof(keyBuffer),"%ld",v);                
                break;
            }
            case FieldDescriptor::CPPTYPE_ENUM:
            {
                const EnumValueDescriptor * evd  = msg->GetReflection()->GetEnum(*msg,field);
                int v = evd->number();
                snprintf(keyBuffer,sizeof(keyBuffer),"%d",v);                
                break;
            }
            case FieldDescriptor::CPPTYPE_UINT32:
            {
                uint32_t v = msg->GetReflection()->GetUInt32(*msg,field);
                snprintf(keyBuffer,sizeof(keyBuffer),"%u",v);                
                break;
            }
            case FieldDescriptor::CPPTYPE_UINT64:
            {
                uint64_t v = msg->GetReflection()->GetUInt64(*msg,field);
                snprintf(keyBuffer,sizeof(keyBuffer),"%lu",v);                
                break;
            }
            case FieldDescriptor::CPPTYPE_STRING:
            {
                snprintf(keyBuffer,sizeof(keyBuffer),"'%s'",msg->GetReflection()->GetString(*msg,field).c_str());                
                break;
            }                
            case FieldDescriptor::CPPTYPE_DOUBLE:
            case FieldDescriptor::CPPTYPE_FLOAT:
            default:
                LOG_ERROR("field = %s type = %d is not supported !",
                    field->name().c_str(),field->cpp_type());
                return -1;
                break;                    
        }
        key += pks[i] + " = " + keyBuffer;        
    }
    return 0;
}
MemSerializer::MetaObject*    DBAgent::FindObject(const string & key)
{
    if(m_mpGetObjects.find(key) != m_mpGetObjects.end())
    {
        return m_mpGetObjects[key].get();
    }
    return NULL;
}
void        DBAgent::FreeObject(const string & key)
{
    m_mpGetObjects.erase(key);
}
MemSerializer::MetaObject*    DBAgent::FindObject(MemSerializer::MetaObject * obj)
{
    string key;
    if(GetPrimaryKey(obj,key))
    {
        return NULL;
    }
    return FindObject(key);    
}
int  DBAgent::Get(void * obj,vector<string> & fields,const Buffer & cb,const char* pszWhereCond)
{
    //gen key
    string key ;
    if(GetPrimaryKey(obj,key))
    {
        if(pszWhereCond)
        {
            key = pszWhereCond;
        }
        else
        {
            return -1;
        }
    }
    string tableName = serializer->GetTypeName(obj);
    string cmdhdr = GET_CMD+DELIM1+tableName+DELIM2;
    for(uint i = 0;i < fields.size(); ++i)
    {
        if(i > 0)
        {
            cmdhdr += DELIM4;
        }
        cmdhdr += fields[i];        
    }
    Buffer ccb;
    ccb.Create(cb.iUsed + cmdhdr.length() + 1 );
    memcpy(ccb.pBuffer,cmdhdr.c_str(),cmdhdr.length()+1);
    memcpy(ccb.pBuffer + cmdhdr.length() + 1,cb.pBuffer,cb.iUsed);
    ccb.iUsed = ccb.iCap;
    ////////////////////////////////////////////////////////
    MysqlRequest req;    
    req.Init();
    req.cb = ccb;
    req.where = key;
    req.fields = fields;
    req.op = MYSQL_REQ_SELECT;
    req.tblname = tableName; 
    int ret = mysql.Request(req);
    if(ret)
    {
        ccb.Destroy();
        return -1;
    }
    return 0;
}
int  DBAgent::Remove(void * obj,const Buffer & cb)
{
    //gen key
    string key ;
    if(GetPrimaryKey(obj,key))
    {
        return -1;
    }
    string tableName = serializer->GetTypeName(obj);
    string cmdhdr = REMOVE_CMD+tableName+DELIM1+key;
    Buffer ccb;
    ccb.Create(cb.iUsed + cmdhdr.length() + 1 );
    memcpy(ccb.pBuffer,cmdhdr.c_str(),cmdhdr.length()+1);
    memcpy(ccb.pBuffer + cmdhdr.length() + 1,cb.pBuffer,cb.iUsed);
    ccb.iUsed = ccb.iCap;
    MysqlRequest req;    
    req.Init();
    req.cb = ccb;
    req.where = key;
    req.op = MYSQL_REQ_DELETE;
    req.tblname = tableName; 
    int ret = mysql.Request(req);
    if(ret)
    {
        ccb.Destroy();
        return -1;
    }
    return 0;
}
int  DBAgent::Update(void * obj,vector<string> & fields,const Buffer &  cb)
{
    //gen key
    string key ;
    if(GetPrimaryKey(obj,key))
    {
        return -1;
    }
    string tableName = serializer->GetTypeName(obj);
    string cmdhdr = UPDATE_CMD+tableName+DELIM1+key;
    for(uint i = 0;i < fields.size(); ++i)
    {
        if(i > 0)
        {
            cmdhdr += DELIM4;
        }
        cmdhdr += fields[i];        
    }
    Buffer ccb;
    if(ccb.Create(cb.iUsed + cmdhdr.length() + 1 ))
    {
        LOG_ERROR("callback create buffer error !");
        return -1;
    }
    memcpy(ccb.pBuffer,  cmdhdr.c_str(),cmdhdr.length()+1);
    memcpy(ccb.pBuffer + cmdhdr.length() + 1,cb.pBuffer,cb.iUsed);
    ccb.iUsed = ccb.iCap;
    /////////////////////////////////////////////////////////////
    MysqlRequest req;    
    req.Init();
    req.cb = ccb;
    req.where = key;
    req.fields = fields;
    req.op = MYSQL_REQ_UPDATE;
    req.tblname = tableName;
    GenerateMysqlFields(req.data,obj,&fields);
    int ret = mysql.Request(req);
    if(ret)
    {
        LOG_ERROR("update mysql error = %d !",ret);
        ccb.Destroy();
        return -1;
    }
    return ret;
}
int  DBAgent::Insert(void * obj,const Buffer &  cb)
{
    //gen key
    string key ;
    if(GetPrimaryKey(obj,key))
    {
        return -1;
    }
    string tableName = serializer->GetTypeName(obj);
    string cmdhdr = INSERT_CMD+tableName+DELIM1+key;
    Buffer ccb;
    if(ccb.Create(cb.iUsed + cmdhdr.length() + 1 ))
    {
        LOG_ERROR("callback create buffer error !");
        return -1;
    }
    memcpy(ccb.pBuffer,  cmdhdr.c_str(),cmdhdr.length()+1);
    memcpy(ccb.pBuffer + cmdhdr.length() + 1,cb.pBuffer,cb.iUsed);
    ccb.iUsed = ccb.iCap;
    MysqlRequest req;    
    req.Init();
    req.cb = ccb;
    req.where = key;
    req.op = MYSQL_REQ_INSERT;
    req.tblname = tableName;
    GenerateMysqlFields(req.data,obj,NULL);
    int ret = mysql.Request(req);
    if(ret)
    {
        LOG_ERROR("insert mysql error = %d !",ret);
        ccb.Destroy();
        return -1;
    }
    return ret;
}
#endif

