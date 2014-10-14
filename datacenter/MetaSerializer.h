#pragma once
#include "base/stdinc.h"
#include "base/Buffer.h"

#include "google/protobuf/descriptor.h"
#include "google/protobuf/message.h"

using google::protobuf::Descriptor;
using google::protobuf::FieldDescriptor;

class MetaSerializer
{
public:
    typedef     typename google::protobuf::Message   MetaObject;
    typedef     shared_ptr<MetaObject>               MetaObjectPtr;
    
public:
    int     Init();
    int     GetPackSize(void* obj);
    int     Pack(void* obj,Buffer & buffer);
    int     UnPack(const char* pszMetaName,Buffer & buffer,void * * ppObj);
    void    Visual(void* pObj,string & s);
public:
    MetaObject*    NewObject(string & typeName);

    //-------------------------------------------------------------------------------
    
    int     SetObjectFieldI32(MetaObject* obj,string & fieldName,int32_t val);
    int     SetObjectFieldI64(MetaObject* obj,string & fieldName,int64_t val);
    int     SetObjectFieldU32(MetaObject* obj,string & fieldName,uint32_t val);
    int     SetObjectFieldU64(MetaObject* obj,string & fieldName,uint64_t val);
    int     SetObjectFieldR32(MetaObject* obj,string & fieldName,float val);
    int     SetObjectFieldR64(MetaObject* obj,string & fieldName,double val);
    int     SetObjectFieldSTR(MetaObject* obj,string & fieldName,string & val);
    int     SetObjectFieldOBJ(MetaObject* obj,string & fieldName,MetaObject * val);

    //-------------------------------------------------------------------------------

    int     GetObjectFieldI32(MetaObject* obj,string & fieldName,int32_t & val);
    int     GetObjectFieldI64(MetaObject* obj,string & fieldName,int64_t & val);
    int     GetObjectFieldU32(MetaObject* obj,string & fieldName,uint32_t & val);
    int     GetObjectFieldU64(MetaObject* obj,string & fieldName,uint64_t & val);
    int     GetObjectFieldR32(MetaObject* obj,string & fieldName,float & val);
    int     GetObjectFieldR64(MetaObject* obj,string & fieldName,double & val);
    int     GetObjectFieldSTR(MetaObject* obj,string & fieldName,string & val);
    int     GetObjectFieldOBJ(MetaObject* obj,string & fieldName,MetaObject * * val);

    //-------------------------------------------------------------------------------
    const   Descriptor*    GetDescriptor(const string & typeName);
    string  GetTypeName(void * obj);
    
public:
    template<class T>
    void    FreeObj(T* pObj){delete pObj;}

public:
    vector<string> & GetTypePrimaryKey(const google::protobuf::Descriptor* desc);
    int    GetFieldMaxLength(const google::protobuf::Descriptor* desc,const string & fieldName);
private:
    google::protobuf::DescriptorPool*       m_pDescriptorPool;
    google::protobuf::MessageFactory*       m_pObjFactory;
    std::map<const google::protobuf::Descriptor*,std::vector<string> >                   m_mpDescPrimaryKey;
};

