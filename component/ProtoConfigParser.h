#pragma once
#include "base/Log.h"
#include "base/File.h"
#include "google/protobuf/descriptor.h"
#include "google/protobuf/message.h"


//protobuffer config parser
//init a config name(with proto name)
//read a proto buffer data to a message
//get the message to a custom Message

//mutable or const messag to referemce
class ProtoBufferConfigParser
{
public:    
    int     Init(const pszProtoName,char* pszConfigMetaName)
    {
        m_strProtoName = pszProtoName;
        m_strConfigMetaName = pszConfigMetaName;
        serilaizer.Init("config",pszConfigMetaName);
        return 0;
    }
    int     ReadConfigFile(const char* pszFileName)
    {
        int sz = File::GetFileSize(pszFileName);
        if(sz <= 0)
        {
            return -1;
        }
        Buffer  buffer;
        buffer.Create(sz);
        File    file(pszFileName,"r+");
        file.Read((char*)buffer.pBuffer,buffer.iCap);
        if(root)
        {
            serilaizer.FreeObj(root);
            root = NULL;
        }
        //unpack with string ?
        int ret = serilaizer.UnPack(m_strConfigMetaName.c_str(),buffer,&root);
        buffer.Destroy();
        if(ret)
        {
            LOG_ERROR("destroy ok !");
            return -1;
        }
        return 0;
    }
    const void*   GetConfig()
    {
        return root;
    }
    ///////////////////////////////////////////////////////    
    string    Visual()
    {
        return root->DebugString();
    }
    void*   mutableConfig()
    {
        return root;
    }
    void    DumpToFile()
    {
        //todo
    }
private:
    Message * root;
    MetaSerializer serilaizer;
    string  m_strProtoName ;
    string  m_strConfigMetaName ;

public:
    ProtoBufferConfigParser();
    ~ProtoBufferConfigParser();    
};






