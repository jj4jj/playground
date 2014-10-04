#pragma once
#include "base/stdinc.h"
#include "base/Buffer.h"

class MemSerializer
{
public:
    int     Init(const char* pszMetaFileName);
    int     GetPackSize(const char* pszMetaName,void* obj);
    int     Pack(const char* pszMetaName,void* obj,Buffer & buffer);
    int     UnPack(const char* pszMetaName,Buffer & buffer,void * * ppObj);
    void    FreeObj(void * pObj);
};

