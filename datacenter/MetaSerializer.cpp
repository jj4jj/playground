#include "base/Log.h"
#include "MetaSerializer.h"

using namespace ::google::protobuf;

#if 1
int     MetaSerializer::Init()
{
    //assert(pszNameSpace);
    //loading meta
    //m_strNameSpace = pszNameSpace;
    m_pDescriptorPool = const_cast<DescriptorPool*>(DescriptorPool::generated_pool());
    assert(m_pDescriptorPool);    
    m_pObjFactory = MessageFactory::generated_factory();
    assert(m_pObjFactory);
    //////////////////////////////////////////////////////////////
    
    return 0;
}
int     MetaSerializer::GetPackSize(void* obj)
{
    Message*    msg  =  (Message*)obj;//m_pObjFactory->GetPrototype(meta);
    return    msg->ByteSize();
}
int     MetaSerializer::Pack(void* obj,Buffer & buffer)
{
    Message*    msg  =  (Message*)obj;//m_pObjFactory->GetPrototype(meta);
    //const Descriptor* meta =  msg->GetDescriptor();
    bool ret =  msg->SerializeToArray(buffer.pBuffer, buffer.iCap);
    if(!ret)
    {
        LOG_ERROR("meta name = %s pack error !",msg->GetDescriptor()->name().c_str());
        buffer.iUsed = 0;
        return -1;
    }
    buffer.iUsed = msg->ByteSize();
    return 0;
}
int     MetaSerializer::UnPack(const char* pszMetaName,Buffer & buffer,void * * ppObj)
{
    *ppObj = NULL;
    string typeName = string(pszMetaName);
    MetaObject* msg =  NewObject(typeName);
    if(!msg)
    {
        LOG_ERROR("meta name = %s new fail !",typeName.c_str());
        return -1;
    }
    if(!msg->ParseFromArray(buffer.pBuffer,buffer.iUsed))
    {
        LOG_ERROR("meta name = %s unpack error ! buffer sz = %d",typeName.c_str(),buffer.iUsed);
        FreeObj(msg);        
        return -1;
    }    
    *ppObj = msg;
    return 0;
}
void    MetaSerializer::Visual(void* pObj,string & s)
{
    Message*  msg = (Message*)pObj;
    s = msg->DebugString();
}
const   Descriptor*    MetaSerializer::GetDescriptor(const string & typeName)
{
    //string fullName = m_strNameSpace+"."+typeName;
    return m_pDescriptorPool->FindMessageTypeByName(typeName);
}
string     MetaSerializer::GetTypeName(void* obj)
{
    MetaObject * msg = (MetaObject*)obj;
    return    msg->GetDescriptor()->name();
    //return msg->GetTypeName();
}
vector<string> & MetaSerializer::GetTypePrimaryKey(const Descriptor* desc)
{    
    if(!desc)
    {
        LOG_ERROR("descriptor not found !");
        return m_mpDescPrimaryKey[desc];
    }
    
    if(!m_mpDescPrimaryKey[desc].empty())
    {
        return  m_mpDescPrimaryKey[desc];
    }

    //GET TYPE#KEY1&KEY2    VALUE
    const Descriptor  * pk = desc->FindNestedTypeByName(string("_PrimaryKey"));
    if(!pk)
    {
        LOG_ERROR("msg name = %s pk not found !",desc->name().c_str());
        return  m_mpDescPrimaryKey[desc];
    }
    const FieldDescriptor* fd = NULL;
    for(int i = 0 ;i < pk->field_count(); ++i)
    {
        fd = pk->field(i);
        if(fd)
        {
            LOG_DEBUG("add meta name = %s pk = %s",desc->name().c_str(),fd->name().c_str());
            m_mpDescPrimaryKey[desc].push_back(fd->name());
        }
    }
    return m_mpDescPrimaryKey[desc]; 
} 
int    MetaSerializer::GetFieldMaxLength(const google::protobuf::Descriptor* desc,const string & fieldName)
{
    if(!desc)
    {
        LOG_ERROR("descriptor not found !");
        return 0;
    }    
    //GET TYPE#KEY1&KEY2    VALUE
    const Descriptor  * pk = desc->FindNestedTypeByName(string("_MaxLength"));
    if(!pk)
    {
        LOG_ERROR("msg name = %s pk not found !",desc->name().c_str());
        return 0;
    }
    
    const FieldDescriptor* fd = NULL;
    for(int i = 0 ;i < pk->field_count(); ++i)
    {
        fd = pk->field(i);
        if(fd->name() == fieldName)
        {
            //default value
            return fd->default_value_uint32();
        }
    }
    return 0;
}

#endif

#if 1
MetaSerializer::MetaObject * MetaSerializer::NewObject(string & typeName)
{
    string metaName = typeName;
    const Descriptor  *   meta = m_pDescriptorPool->FindMessageTypeByName(metaName);
    if(!meta)
    {
        LOG_ERROR("meta name = %s not found !",metaName.c_str());
        return NULL;
    }
    const Message * prototype = m_pObjFactory->GetPrototype(meta);
    if(!prototype)
    {
        LOG_ERROR("meta name = %s proto not found !",metaName.c_str());
        return NULL;
    }
    Message * msg = prototype->New();
    return msg;
}

//-------------------------------------------------------------------------------
#ifndef GET_FIELD
#define GET_FIELD(obj,fieldName)   \
const FieldDescriptor * field = obj->GetDescriptor()->FindFieldByName(fieldName);\
if(!field)\
{\
    LOG_FATAL("find field error name = %s",fieldName.c_str());\
    return -1;\
}
#endif

int     MetaSerializer::SetObjectFieldI32(MetaObject* obj,string & fieldName,int32_t val)
{
    GET_FIELD(obj,fieldName)
    obj->GetReflection()->SetInt32(obj,field,val);
    return  0;
}
int     MetaSerializer::SetObjectFieldI64(MetaObject* obj,string & fieldName,int64_t val)
{
    GET_FIELD(obj,fieldName)
    obj->GetReflection()->SetInt64(obj,field,val);
    return  0;
}

int     MetaSerializer::SetObjectFieldU32(MetaObject* obj,string & fieldName,uint32_t val)
{
    GET_FIELD(obj,fieldName)
    obj->GetReflection()->SetUInt32(obj,field,val);
    return  0;
}
int     MetaSerializer::SetObjectFieldU64(MetaObject* obj,string & fieldName,uint64_t val)
{
    GET_FIELD(obj,fieldName)
    obj->GetReflection()->SetUInt64(obj,field,val);
    return  0;
}
int     MetaSerializer::SetObjectFieldR32(MetaObject* obj,string & fieldName,float val)
{
    GET_FIELD(obj,fieldName)
    obj->GetReflection()->SetFloat(obj,field,val);
    return  0;
}
int     MetaSerializer::SetObjectFieldR64(MetaObject* obj,string & fieldName,double val)
{
    GET_FIELD(obj,fieldName)
    obj->GetReflection()->SetDouble(obj,field,val);
    return  0;
}
int     MetaSerializer::SetObjectFieldSTR(MetaObject* obj,string & fieldName,string & val)
{
    GET_FIELD(obj,fieldName)
    obj->GetReflection()->SetString(obj,field,val);
    return  0;
}
int     MetaSerializer::SetObjectFieldOBJ(MetaObject* obj,string & fieldName,MetaObject * val)
{
    GET_FIELD(obj,fieldName)
    obj->GetReflection()->SetAllocatedMessage(obj,val,field);
    return  0;
}

//----------------------------------------------------------------------------------------

int     MetaSerializer::GetObjectFieldI32(MetaObject* obj,string & fieldName,int32_t & val)
{
    GET_FIELD(obj,fieldName)
    val = obj->GetReflection()->GetInt32(*obj,field);
    return 0;
}
int     MetaSerializer::GetObjectFieldI64(MetaObject* obj,string & fieldName,int64_t & val)
{
    GET_FIELD(obj,fieldName)
    val = obj->GetReflection()->GetInt64(*obj,field);
    return 0;
}
int     MetaSerializer::GetObjectFieldU32(const MetaObject* obj,string & fieldName,uint32_t & val)
{
    GET_FIELD(obj,fieldName)
    val = obj->GetReflection()->GetUInt32(*obj,field);
    return 0;
}
int     MetaSerializer::GetObjectFieldU64(MetaObject* obj,string & fieldName,uint64_t & val)
{
    GET_FIELD(obj,fieldName)
    val = obj->GetReflection()->GetUInt64(*obj,field);
    return 0;
}
int     MetaSerializer::GetObjectFieldR32(MetaObject* obj,string & fieldName,float & val)
{
    GET_FIELD(obj,fieldName)
    val = obj->GetReflection()->GetFloat(*obj,field);
    return 0;
}
int     MetaSerializer::GetObjectFieldR64(MetaObject* obj,string & fieldName,double & val)
{
    GET_FIELD(obj,fieldName)
    val = obj->GetReflection()->GetDouble(*obj,field);
    return 0;
}
int     MetaSerializer::GetObjectFieldSTR(MetaObject* obj,string & fieldName,string  & val)
{
    GET_FIELD(obj,fieldName)
    val = obj->GetReflection()->GetString(*obj,field);
    return 0;
}
int     MetaSerializer::GetObjectFieldOBJ(MetaObject* obj,string & fieldName,MetaObject * * val)
{
    GET_FIELD(obj,fieldName)
    *val = const_cast<MetaObject*>( &(obj->GetReflection()->GetMessage(*obj,field)) );
    return 0;
}

#endif


