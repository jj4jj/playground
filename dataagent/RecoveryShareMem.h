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
class RecoverShmNotifier
{
public:
    virtual  int    OnAttach(char* pData,size_t size,char chInited) = 0;
};


struct RecoveryShareMemReg
{
    string     name;
    size_t     zSize;
    RecoverShmNotifier* notifier;
};


#pragma pack(1)
struct  ShmModule
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
struct  RecoveryShmFmt
{
    char        magic[4];//RSHM
    char        reserve[16];//reserve
    size_t      dataLength;
    uint32_t    dataOffset;//----------------------|      
    uint16_t    modulesCount;
    ShmModule   modules[RECOVERYSHM_MAX_MOUDLES];
    //char        data[0];<------------------------|
};
#pragma pack()


class MemSerializer;
class RecoveryShareMem
{
public:
    int          Register(const RecoveryShareMemReg & reg);
    int          Init(const char* pszShmKeyPath);
    ShmModule*   FindModule(const char* pszName);
    Buffer       GetModleBuffer(const char* pszName);
 protected:
    void        InitRegModules();
    void        AppendRegModules();
    size_t      GetOldShmSize();
    int         AttachAllModules();
    int         Check();
    int         AddModule(const char* pszName);
    RecoveryShareMemReg * FindReg(const string & name);
private:
    MemSerializer * m_pSerializer;
    ShareMemory     m_shm;
    vector<RecoveryShareMemReg>     m_vecModules;
    RecoveryShmFmt* m_pFmtBase;
    char*           m_pDataBase;
    size_t          m_zTotalSize;
};


