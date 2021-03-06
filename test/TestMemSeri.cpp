#include "datacenter/MetaSerializer.h"
#include "proto/gen/test/meta.pb.h"
#include "base/Log.h"

using namespace test;
void SetRole(test::Role * pR)
{
    pR->set_rid(12345678L);
    pR->set_name(string("seanpeng"));
    pR->mutable_base()->set_gendor(RoleBase::MALE);
    pR->mutable_hero()->add_heros()->set_id(0);
    pR->mutable_pack()->set_maxgrid(1);
    pR->mutable_grow()->set_level(1);
    pR->mutable_grow()->set_exp(0);

    pR->mutable_stage()->set_curstage(1);
    pR->mutable_sns()->set_maxfrinds(1);
    pR->mutable_ext()->set_fightcap(0);
}

int main(int argc,char* argv[])
{
    MetaSerializer   seri;
    if(seri.Init())
    {
        LOG_ERROR("meta proto init error !");
        return -1;
    }
    test::Role    role;

    SetRole(&role);



    string s;
    seri.Visual(&role,s);

    
    LOG_DEBUG("dump:\n%s",s.c_str());

    Buffer buffer;
    int sz =  seri.GetPackSize(&role);
    buffer.Create(sz);
    seri.Pack(&role,buffer);
    LOG_INFO("pack obj ok buffer size = %d",buffer.iCap);
    
    File    file("role.dat","w");
    file.Write((char*)buffer.pBuffer,buffer.iUsed);
    file.Close();
    buffer.Destroy();

    file.Open("role.dat","r");
    int iBuff = file.GetFileSize("role.dat");
    buffer.Create(iBuff);
    buffer.iUsed = iBuff;
    file.Read((char*)buffer.pBuffer,buffer.iCap);
    file.Close();
    LOG_INFO("read data file size = %d",buffer.iCap);
    
    void * p = NULL;
    seri.UnPack("test.Role",buffer,&p);
    test::Role* pRole = (test::Role*)p;

    seri.Visual(pRole,s);
    LOG_DEBUG("dump:\n%s",s.c_str());

    buffer.Destroy();            
    seri.FreeObj(pRole);
    
    return 0;
};

