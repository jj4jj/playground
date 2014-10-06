#include "base/Log.h"
#include "MemSerializer.h"

using namespace ::google::protobuf;

int     MemSerializer::Init(const char* pszMetaFileName)
{
    //loading meta
    const DescriptorPool * root =  DescriptorPool::generated_pool();
    assert(root);
    const FileDescriptor * fileDescriptor = root->FindFileByName(string(pszMetaFileName));
    if(!fileDescriptor)
    {
        LOG_FATAL("file descriptor = %s not found !",pszMetaFileName);
        return -1;
    }
    m_pDescriptorPool = const_cast<DescriptorPool*>(fileDescriptor->pool());
    assert(m_pDescriptorPool);
    m_pObjFactory = MessageFactory::generated_factory();
    assert(m_pObjFactory);
    //////////////////////////////////////////////////////////////
    
    return 0;
}
int     MemSerializer::GetPackSize(void* obj)
{
    Message*    msg  =  (Message*)obj;//m_pObjFactory->GetPrototype(meta);
    return    msg->ByteSize();
}
int     MemSerializer::Pack(void* obj,Buffer & buffer)
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
int     MemSerializer::UnPack(const char* pszMetaName,Buffer & buffer,void * * ppObj)
{
    *ppObj = NULL;
    const Descriptor  *   meta = m_pDescriptorPool->FindMessageTypeByName(string(pszMetaName));
    if(!meta)
    {
        LOG_ERROR("meta name = %s not found !",pszMetaName);
        return -1;
    }
    const Message * prototype = m_pObjFactory->GetPrototype(meta);
    if(!prototype)
    {
        LOG_ERROR("meta name = %s proto not found !",pszMetaName);
        return -1;
    }
    Message * msg = prototype->New();
    if(!msg)
    {
        LOG_ERROR("meta name = %s new fail !",pszMetaName);
        return -1;
    }
    if(!msg->ParseFromArray(buffer.pBuffer,buffer.iUsed))
    {
        LOG_ERROR("meta name = %s unpack error ! buffer sz = %d",pszMetaName,buffer.iUsed);
        
        
        return -1;
    }    
    *ppObj = msg;
    return 0;
}
void    MemSerializer::Visual(void* pObj,string & s)
{
    Message*  msg = (Message*)pObj;
    s = msg->DebugString();
}

vector<string> & MemSerializer::GetTypePrimaryKey(const Descriptor* desc)
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