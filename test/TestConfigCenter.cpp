

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
    printf("get config hello:%s word:%s nihao:%p\n",cc.GetConfig("hello"),cc.GetConfig("word"),cc.GetConfig("nihao"));
    //
    int n = cc.GetConfigInt("num",233,"num");
    printf("get num = %d\n",n);
    cc.SetConfigInt("num",13, "num");
    n = cc.GetConfigInt("num",23,"num");
    printf("get num = %d\n",n);
    return -1;
}

