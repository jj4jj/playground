#include "dataagent/RecoveryShareMem.h"
#include "base/Log.h"
#include "base/CommonMacro.h"
class TestShm2Notifier: public RecoverShmNotifier
{
public:
    virtual  int    OnAttach(char* pData,size_t size,char chInited)
    {
        pBaseBuffer = pData;
        LOG_INFO("test 2 shm notifier attach size = %u , init = %d",size,chInited);
        if(chInited == 0)
        {
            //is fresh , first time 
            //new shm 
            LOG_INFO("first init !");
            STRNCPY(pData,size,"Hello,This is test 2 !!!");
        }
        else
        {
            LOG_INFO("my content = %s",pBaseBuffer);
        }
        LOG_INFO("init ok!");
        return 0;
    }
    void print()
    {
        LOG_INFO("print my content = %s",pBaseBuffer);
    }

    char * pBaseBuffer;
    
};

int main(int argc,char* argv[])
{
    RecoveryShareMem    rsm;
    RecoveryShareMemReg rsmr1;
    RecoveryShareMemReg rsmr2;

    TestShm2Notifier    tsn2;

    rsmr1.name = "test1";
    rsmr1.zSize = 10240;
    rsmr1.notifier = NULL;
    
    rsmr2.name = "test2";
    rsmr2.zSize = 102400;
    rsmr2.notifier = &tsn2;

    rsm.Register(rsmr1);
    rsm.Register(rsmr2);
    if(!File::Exist("test.shm"))
    {
        File    file("test.shm","w+");
    }
    if(rsm.Init("test.shm"))
    {
        LOG_ERROR("init erro !");   
        return -1;
    }
    ////////////////////////////////
    tsn2.print();
    ShmModule * pMod =   rsm.FindModule("test1");
    assert(pMod);
    Buffer buffer = rsm.GetModleBuffer("test1");
    LOG_INFO("mod buffer = %s size = %d",buffer.pBuffer,buffer.iUsed);  
    return 0;
};



