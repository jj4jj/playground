

#include "datacenter/ConfigCenter.h"

int main()
{

    ConfigCenter & cc = ConfigCenter::Instance();
    if(cc.Init())
    {
        printf("init cc error !");
        return -1;
    }
    cc.SetConfig("hello","1");
    cc.SetConfig("word","world");
    printf("get config hello:%s word:%s nihao:%p",cc.GetConfig("hello"),cc.GetConfig("word"),cc.GetConfig("nihao"));
    return -1;
}

