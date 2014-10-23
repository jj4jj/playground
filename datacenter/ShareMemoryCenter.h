#pragma once
#include "base/stdinc.h"
#include "base/Buffer.h"
#include "ipc/ShareMemory.h"

//it's a share memory management system
//it's supports a share memory space and serialization when
//system is restart or coredump .
//so :
//1. u need tell it  what size of your shm 
//2. what's the type of your shm collection (how to regnize)
//3. when system is close (before saving them) , what you want to do.
//4. when system is start (after recovery or fresh memory allocated) , what you want  to do.
class ShareMemoryModuleNotifier
{
public:
    virtual  int    OnAttach(char* pData,size_t size,char chInited) = 0;
};


struct ShareMemoryModuleReg
{
    string     name;
    size_t     zSize;
    ShareMemoryModuleNotifier* notifier;
};


#pragma pack(1)
struct  ShareMemoryModule
{
    char    szName[32];
    char    chInited;//0
    size_t  dataOffset;
    size_t  zSize;
};
//FMT
//HEAD
//ENTRY INDEX
//DATA
enum {
    RECOVERYSHM_MAX_MOUDLES     =   256,
};
struct  ShareMemoryCenterDataFmt
{
    char        magic[4];//RSHM
    char        reserve[16];//reserve
    size_t      dataLength;
    uint32_t    dataOffset;//----------------------|      
    uint16_t    modulesCount;
    ShareMemoryModule   modules[RECOVERYSHM_MAX_MOUDLES];
    //char        data[0];<------------------------|
};
#pragma pack()


class MetaSerializer;
class ShareMemoryCenter
{
public:
    ShareMemoryCenter();
public:
    int          Register(const ShareMemoryModuleReg & reg);
    int          Init(const char* pszShmKeyPath);
    ShareMemoryModule*   FindModule(const char* pszName);
    Buffer       GetModleBuffer(const char* pszName);
protected:
    void        InitRegModules();
    void        AppendRegModules();
    size_t      GetOldShmSize();
    int         AttachAllModules();
    int         Check(bool bExcludeProcess = true);
    int         AddModule(const char* pszName);
    ShareMemoryModuleReg * FindReg(const string & name);
public:    
    //don't call it generally , unless you know what you are doing !!
    void         DeleteSHM();
private:
    MetaSerializer * m_pSerializer;
    ShareMemory     m_shm;
    vector<ShareMemoryModuleReg>     m_vecModules;
    ShareMemoryCenterDataFmt* m_pFmtBase;
    char*           m_pDataBase;
    size_t          m_zTotalSize;
};


