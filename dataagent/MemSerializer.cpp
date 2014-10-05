#include "base/Log.h"
#include "MemSerializer.h"
#include "google/protobuf/descriptor.h"
#include "google/protobuf/message.h"


using namespace google::protobuf;

int     MemSerializer::Init(const char* pszMetaFileName)
{
    //loading meta
    
    return 0;
}
int     MemSerializer::GetPackSize(const char* pszMetaName,void* obj)
{
    return 0;
}
int     MemSerializer::Pack(const char* pszMetaName,void* obj,Buffer & buffer)
{
    return 0;
}
int     MemSerializer::UnPack(const char* pszMetaName,Buffer & buffer,void * * ppObj)
{
    return 0;
}
void    MemSerializer::FreeObj(void * pObj)
{
}
void    MemSerializer::Visual(const char* pszMetaName,void* pObj,string & s)
{
}

