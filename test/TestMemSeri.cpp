#include "dataagent/MemSerializer.h"
#include "proto/meta/meta.pb.h"
#include "base/Log.h"

void SetRole(meta::Role * pR)
{



}

int main(int argc,char* argv[])
{
    MemSerializer   seri;
    if(seri.Init("meta.proto"))
    {
        LOG_ERROR("meta proto init error !");
        return -1;
    }
    meta::Role    role;

    SetRole(&role);
    string s;
    seri.Visual("Role",&role,s);


    Buffer buffer;
    int sz =  seri.GetPackSize("meta::Role",&role);
    buffer.Create(sz);
    seri.Pack("Role",&role,buffer);
    LOG_INFO("pack obj ok buffer size = %d",buffer.iUsed);
    File    file("role.dat","w");
    file.Write((char*)buffer.pBuffer,buffer.iUsed);
    file.Close();
    buffer.Destroy();

    file.Open("role.dat","r");
    int iBuff = file.GetFileSize("role.dat");
    buffer.Create(iBuff);
    file.Read((char*)buffer.pBuffer,buffer.iCap);
    file.Close();

    void * p;
    seri.UnPack("Role",buffer,&p);
    meta::Role* pRole = (meta::Role*)p;

    seri.Visual("Role",pRole,s);
        
    seri.FreeObj(p);
    
    return 0;
};

