#include "base/Log.h"
#include "MemSerializer.h"
#include "RedisAgent.h"
#include "hiredis.h"
#include "CacheAgent.h"

//#include "regex.h"

using namespace google::protobuf;

#define INSERT_CMD  ("INSERT")
#define GET_CMD     ("GET")
#define REMOVE_CMD  ("DELETE")
#define UPDATE_CMD  ("SET")
#define DELIM1       (string(" "))
#define DELIM2       (string(":"))
#define DELIM3       (string("&"))


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
        int headerlen = strlen((char*)cb.pBuffer) + 1;
        string cmdhdr = (char*)cb.pBuffer;
        //regex_t reg;
        //const char* pszReg = "[A-Z]+ [a-zA-Z0-9]+:[a-zA-Z]+(&[a-zA-Z]+)*";
        
        size_t pos = cmdhdr.find(DELIM1);
        string cmd = "";
        if(pos != string::npos)
        {
            cmd = cmdhdr.substr(0,pos);
        }
        string cmdtype = "";
        size_t type = cmdhdr.find(DELIM2,pos+1);
        if(type != string::npos)
        {
            cmdtype = cmdhdr.substr(pos+1,type-pos-1);
        }        
        string keys = cmdhdr.substr(type+1);
        Buffer mcb((char*)cb.pBuffer+headerlen,cb.iUsed-headerlen);
        agent->DispatchResult(cmd,cmdtype,keys,reply,mcb,timeout);
        //////////////////////////////////////////////////////////
        cb.Destroy();
        return 0;
    }
};

int  CacheAgent::DispatchResult(string & cmd,string & type,string & key,
                    redisReply* reply,Buffer & cb,bool timeout)
{
    if(reply->type == REDIS_REPLY_ERROR)
    {
        LOG_ERROR("redis exec cmd = %s type = %s key = %s error for = %s",
                   cmd.c_str(),type.c_str(),key.c_str(),(char*)reply->str);
    }
    if(m_mpCacheListener.find(type) != m_mpCacheListener.end())
    {
        if(cmd == GET_CMD)
        {
            return m_mpCacheListener[type]->OnGet(reply,cb,timeout);
        }
        else if(cmd == UPDATE_CMD)
        {
            return m_mpCacheListener[type]->OnUpdate(reply,cb,timeout);
        }
        else if(cmd == INSERT_CMD)
        {
            return m_mpCacheListener[type]->OnInsert(reply,cb,timeout);
        }
        else if(cmd == REMOVE_CMD)
        {
            return m_mpCacheListener[type]->OnRemove(reply,cb,timeout);
        }
        else
        {
            LOG_ERROR("not support cmd = %s",cmd.c_str());
            return -1;
        }
    }
    else
    {
        LOG_ERROR("type = %s listener not found !",type.c_str());
        return -1;
    }
    return 0;
}

int  CacheAgent::Init(const CacheAgentOption  & cao,MemSerializer * seri)
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

int  CacheAgent::AddCacheListener(string typeName,DataListenerPtr ptr)
{
    if(m_mpCacheListener[typeName])
    {
        LOG_FATAL("repeat add cache listener = %s",typeName.c_str());
        return -1;
    }
    m_mpCacheListener[typeName] = ptr;
    return 0;
}

int  CacheAgent::GetKey(void* obj,string & key)
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
            key += DELIM3;
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
int  CacheAgent::Get(void * obj,const Buffer & cb)
{
    //gen key
    string key ;
    if(GetKey(obj,key))
    {
        return -1;
    }
    string cmd = GET_CMD+DELIM1+key;
    Buffer ccb;
    ccb.Create(cb.iUsed + cmd.length() + 1 );
    memcpy(ccb.pBuffer,cmd.c_str(),cmd.length()+1);
    memcpy(ccb.pBuffer + cmd.length() + 1,cb.pBuffer,cb.iUsed);
    ccb.iUsed = ccb.iCap;
    LOG_INFO("cache exec cmd = %s",cmd.c_str());
    //
    /////////////////////////////////////////
    return redis->Command(cmd.c_str(),ccb);
}
int  CacheAgent::Remove(void * obj,const Buffer & cb)
{
    //gen key
    string key ;
    if(GetKey(obj,key))
    {
        return -1;
    }
    string cmd = REMOVE_CMD+DELIM1+key;
    Buffer ccb;
    ccb.Create(cb.iUsed + cmd.length() + 1 );
    memcpy(ccb.pBuffer,cmd.c_str(),cmd.length()+1);
    memcpy(ccb.pBuffer + cmd.length() + 1,cb.pBuffer,cb.iUsed);
    ccb.iUsed = ccb.iCap;
    LOG_INFO("cache exec cmd = %s",cmd.c_str());
    return redis->Command(cmd.c_str(),ccb);
}
int  CacheAgent::Insert(void * obj,const Buffer &  cb)
{
    //gen key
    string key ;
    if(GetKey(obj,key))
    {
        return -1;
    }
    string cmd = INSERT_CMD+DELIM1+key;
    Buffer ccb;
    ccb.Create(cb.iUsed + cmd.length() + 1 );
    memcpy(ccb.pBuffer,cmd.c_str(),cmd.length()+1);
    memcpy(ccb.pBuffer + cmd.length() + 1,cb.pBuffer,cb.iUsed);
    ccb.iUsed = ccb.iCap;
    //buffer to string.       
    LOG_INFO("cache exec cmd = %s",cmd.c_str());
    return redis->Command(cmd.c_str(), ccb);
}
int  CacheAgent::Update(void * obj,const Buffer &  cb)
{
    //gen key
    string key ;
    if(GetKey(obj,key))
    {
        return -1;
    }
    string cmd = UPDATE_CMD+DELIM1+key;
    Buffer ccb;
    ccb.Create(cb.iUsed + cmd.length() + 1 );
    memcpy(ccb.pBuffer,cmd.c_str(),cmd.length()+1);
    memcpy(ccb.pBuffer + cmd.length() + 1,cb.pBuffer,cb.iUsed);
    ccb.iUsed = ccb.iCap;
    //buffer to string.    
    Buffer buffer(serializer->GetPackSize(obj));
    cmd += 
        serializer->Pack(obj,buffer);
    LOG_INFO("cache exec cmd = %s",cmd.c_str());
    return redis->Command(cmd.c_str(),ccb);
}

