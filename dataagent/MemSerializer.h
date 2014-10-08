#pragma once
#include "base/stdinc.h"
#include "base/Buffer.h"

#include "google/protobuf/descriptor.h"
#include "google/protobuf/message.h"


class MemSerializer
{
public:
    int     Init(const char* pszNameSpace,const char* pszMetaFileName = NULL);
    int     GetPackSize(void* obj);
    int     Pack(void* obj,Buffer & buffer);
    int     UnPack(const char* pszMetaName,Buffer & buffer,void * * ppObj);
    void    Visual(void* pObj,string & s);

public:
    template<class T>
    void    FreeObj(T* pObj){delete pObj;}

public:
    vector<string> & GetTypePrimaryKey(const google::protobuf::Descriptor* desc);

private:
    string                                  m_strNameSpace;
    google::protobuf::DescriptorPool*       m_pDescriptorPool;
    google::protobuf::MessageFactory*       m_pObjFactory;
    std::map<const google::protobuf::Descriptor*,std::vector<string> >                   m_mpDescPrimaryKey;
};

