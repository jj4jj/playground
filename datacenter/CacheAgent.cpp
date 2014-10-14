#include "base/CommonMacro.h"
#include "base/Log.h"
#include "MetaSerializer.h"
#include "RedisAgent.h"
#include "hiredis.h"
#include "DataCommon.h"
#include "CacheAgent.h"
//#include "regex.h"

using namespace google::protobuf;


#if 1
class CachAgentRedisCommandListener : public RedisCommandListener
{
private :
    CacheAgent *     agent;
    /////////////////////////////////////////////////////////////
public :
    CachAgentRedisCommandListener(CacheAgent* _agent):agent(_agent){}

public :
    int     OnCommand(redisReply *reply,Buffer & cb,bool timeout = false)
    {
        LOG_DEBUG("on command cb = %s time out = %d",cb.pBuffer,timeout?1:0);
        /////////////////////////////////////////////////////////////////////
        CacheAgentCallBack * pCallBack = (CacheAgentCallBack *)cb.pBuffer;
        CacheAgentCallBack::Head * pCallBackHead = &(pCallBack->head);
        assert(cb.iUsed >= pCallBackHead->length);
        Buffer ucb((char*)cb.pBuffer + pCallBackHead->length,
                   cb.iUsed - pCallBackHead->length);
        int ret = agent->DispatchResult(*pCallBack,reply,ucb,timeout);
        /////////////////////////////////////////////////////////////////////////////////////
        if(ret)
        {
            LOG_DEBUG("cmd processing error = %d",ret);
        }
        cb.Destroy();
        return 0;
    }
};
#endif



#define DELIM1  (":")
#define DELIM2  ("&")

#if 1
int  CacheAgent::DispatchResult(const CacheAgentCallBack & mcb,
                    redisReply* reply,Buffer & ucb,bool timeout)
{
    int ret = DATA_OK;
    if(!reply)
    {
        ret = timeout?DATA_TIME_OUT:ret;
    }
    else if(reply->type == REDIS_REPLY_ERROR)
    {
        LOG_ERROR("redis exec type = %d type key = %s error for = %s",
                   mcb.head.type,mcb.head.szTypeKey,(char*)reply->str);
        ret = DATA_REDIS_ERR_START + reply->type;
    }  
    else if(reply->type == REDIS_REPLY_NIL)
    {
        ret = DATA_NO_EXISTS;
    }
    string typeKey = string(mcb.head.szTypeKey);
    if(m_mpListener.find(typeKey) != m_mpListener.end())
    {
        if(mcb.head.cmd == CACHE_CMD_GET)
        {          
            if(ret)
            {
                return m_mpListener[typeKey]->OnGet(ret,NULL,ucb);
            }
            if(reply->type == REDIS_REPLY_STRING)
            {
                //
                int ret = DATA_OK;
                Buffer  buff(reply->str,reply->len);
                MetaSerializer::MetaObject* obj = NULL;
                string objType = GetMetaNameSpace()+"."+typeKey;
                if(serializer->UnPack(objType.c_str(),buff,(void**)&obj))
                {
                    LOG_FATAL("message unpack error !");
                    ret = DATA_UNPACK_ERROR;
                }
                else
                {
                    string key = "";
                    assert(0 == GetObjectKey(obj,key));
                    m_mpGetObjects[key] = shared_ptr<MetaSerializer::MetaObject>(obj);
                }
                return m_mpListener[typeKey]->OnGet(ret,obj,ucb);
            }
            else
            {   
                LOG_ERROR("get cache reply not support type = %d",reply->type);
                return m_mpListener[typeKey]->OnGet(DATA_INTERNAL_ERR,NULL,ucb);
            }
        }
        else if(mcb.head.cmd == CACHE_CMD_UPDATE)
        {
            return m_mpListener[typeKey]->OnUpdate(ret,ucb);
        }
        else if(mcb.head.cmd == CACHE_CMD_INSERT)
        {
            return m_mpListener[typeKey]->OnInsert(ret,ucb);
        }
        else if(mcb.head.cmd == CACHE_CMD_REMOVE)
        {
            return m_mpListener[typeKey]->OnRemove(ret,ucb);
        }
        else
        {
            LOG_ERROR("not support type = %d cmd = %d",mcb.head.type,mcb.head.cmd);
            return -1;
        }
    }
    else
    {
        LOG_ERROR("type = %s listener not found !",mcb.head.szTypeKey);
        return -1;
    }
    return 0;
}

int  CacheAgent::Init(const CacheAgentOption  & cao,MetaSerializer * seri)
{
    if(RedisAgent::Instance().Init(cao.addrList,
            RedisCommandListenerPtr(new CachAgentRedisCommandListener(this)) ) ) 
    {
        LOG_ERROR("redis agent init error !");
        return -1;
    }
            
    redis = RedisAgent::instance();
    serializer = seri;
    return 0;
}
int  CacheAgent::Polling(int    iProcPerTick)
{
    if(redis)
    {
        //polling redis client
        redis->Polling(iProcPerTick);
    }    
    else
    {
        return -1;
    }
    return 0;
}
int  CacheAgent::Destroy()
{
    serializer = NULL;
    redis = NULL;
    
    return 0;
}

int  CacheAgent::AddListener(string typeName,DataListenerPtr ptr)
{
    if(m_mpListener[typeName])
    {
        LOG_FATAL("repeat add cache listener = %s",typeName.c_str());
        return -1;
    }
    m_mpListener[typeName] = ptr;
    return 0;
}
#endif

#if 1
int  CacheAgent::GetObjectKey(void* obj,string & key)
{
    Message * msg = (Message*) obj;
    const Descriptor* desc = msg->GetDescriptor();
    std::vector<string> &   pks = serializer->GetTypePrimaryKey(desc);
    if(pks.empty())
    {
        return -1;
    }
    //key:type#key&key
    key = serializer->GetTypeName(obj)+DELIM1;
    for(uint i = 0; i < pks.size(); ++i)
    {
        if(i > 0)
        {
            key += DELIM2;
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
                snprintf(keyBuffer,sizeof(keyBuffer),"%s",msg->GetReflection()->GetString(*msg,field).c_str());                
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
        key += keyBuffer;        
    }
    return 0;
}
MetaSerializer::MetaObject*    CacheAgent::FindObject(const string & key)
{
    if(m_mpGetObjects.find(key) != m_mpGetObjects.end())
    {
        return m_mpGetObjects[key].get();
    }
    return NULL;
}
void        CacheAgent::FreeObject(const string & key)
{
    m_mpGetObjects.erase(key);
}
MetaSerializer::MetaObject*    CacheAgent::FindObject(MetaSerializer::MetaObject * obj)
{
    string key;
    if(GetObjectKey(obj,key))
    {
        return NULL;
    }
    return FindObject(key);    
}

int     CacheAgent::CreateCallBack(Buffer & resultCB,CacheCollectionType type,
            CacheCallBackTypeCmd cmd,
            const string & typeKey,
            const Buffer * pCustomCB,const char * pszObjKey )
{
    resultCB.Destroy();
    int iCustomCBSize = 0;
    int iObjKeyLen = 0;
    if(pszObjKey)
    {
        iObjKeyLen = strlen(pszObjKey);
        if(iObjKeyLen > CacheAgentCallBack::MAX_OBJ_KEY_SIZE)
        {
            LOG_ERROR("type = %u key = %s cmd = %d obj key lenth = %d is too much !",
                type,typeKey.c_str(),cmd,iObjKeyLen);
            return -1;
        }
    }
    if(pCustomCB)
    {
        iCustomCBSize = pCustomCB->iUsed;
    }
    ///////////////////////////////////////////////////////////////////
    int mcb_size = sizeof(CacheAgentCallBack::Head);
    if(iObjKeyLen > 0)
    {
        mcb_size += sizeof(CacheAgentCallBack::Body);
    }
    //extension future add
    


    ///////////////////////////////////////////////////////////////////    
    if(resultCB.Create(iCustomCBSize + mcb_size ))
    {
        LOG_ERROR("create call back error type = %d cmd = %k custom cb size = %d type key = %s",
            type,cmd,iCustomCBSize,typeKey.c_str());
        return -1;
    }
    CacheAgentCallBack* pCallBack = (CacheAgentCallBack*)resultCB.pBuffer;
    bzero(pCallBack,mcb_size);
    //-------------------------------------------------------------------
    pCallBack->head.length = mcb_size;
    pCallBack->head.type = type;
    pCallBack->head.cmd = cmd;
    STRNCPY(pCallBack->head.szTypeKey,
            sizeof(pCallBack->head.szTypeKey),
            typeKey.c_str());
    if(iObjKeyLen > 0)
    {
        STRNCPY(pCallBack->body.szObjKey,
                sizeof(pCallBack->body.szObjKey),
                pszObjKey);
    }
    if(iCustomCBSize > 0)
    {
        memcpy(resultCB.pBuffer + mcb_size,
                pCustomCB->pBuffer,
                iCustomCBSize);
    }
    resultCB.iUsed = resultCB.iCap;
    return 0;
}
void    CacheAgent::FreeCallBack(Buffer & cb)
{
    cb.Destroy();
}
#endif

#if 1
int  CacheAgent::Get(void * obj,const Buffer & cb)
{
    //gen key
    string key ;
    if(GetObjectKey(obj,key))
    {
        return -1;
    }
    string typeKey = serializer->GetTypeName(obj);
    Buffer ccb;
    if(CreateCallBack(ccb,CACHE_TYPE_KV,CACHE_CMD_GET,
            typeKey,&cb))   
    {
        LOG_ERROR("get key = %s create call back error !",
                   key.c_str());
        return -1;
    }
    /////////////////////////////////////////
    int ret = redis->Get(key,ccb);
    if(ret)
    {
        ccb.Destroy();
        return -1;
    }
    return 0;
}
int  CacheAgent::Remove(void * obj,const Buffer & cb)
{
    //gen key
    string key ;
    if(GetObjectKey(obj,key))
    {
        return -1;
    }
    Buffer ccb;
    if(CreateCallBack(ccb,CACHE_TYPE_KV,CACHE_CMD_REMOVE,
        serializer->GetTypeName(obj),&cb))
    {
        LOG_ERROR("get ");
        return -1;
    }
    int ret = redis->Remove(key,ccb);
    if(ret)
    {
        ccb.Destroy();
        return -1;
    }
    return 0;
}
int  CacheAgent::Update(void * obj,const Buffer &  cb)
{
    //gen key
    string key ;
    if(GetObjectKey(obj,key))
    {
        return -1;
    }
    Buffer buffer;
    if(buffer.Create(serializer->GetPackSize(obj)))
    {
        LOG_ERROR("get pack size  %d alloc error !",serializer->GetPackSize(obj));
        return -1;
    }
    if(serializer->Pack(obj,buffer))
    {
        LOG_ERROR("pack obj error !");
        return -1;
    }    
    Buffer ccb;    
    if(CreateCallBack(ccb,CACHE_TYPE_KV,CACHE_CMD_UPDATE,
        serializer->GetTypeName(obj),&cb))
    {
        buffer.Destroy();
        LOG_ERROR("get ");
        return -1;
    } 
    int ret = redis->Update(key,buffer,ccb);
    buffer.Destroy();
    if(ret)
    {
        LOG_ERROR("update redis error = %d !",ret);
        ccb.Destroy();
        return -1;
    }
    return ret;
}
#endif

