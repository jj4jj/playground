#include "base/stdinc.h"
#include "base/Log.h"
#include "datacenter/ResTableAgent.h"
#include "proto/gen/res/include.h"

int main()
{
    ResTableAgent   rta;
    MetaSerializer  ser;
    if(ser.Init())
    {
        LOG_ERROR("meta serializer init error !");
        return -1;
    }
    if(rta.Init("../res",&ser))
    {
        LOG_ERROR("res init error !");
        return -1;
    }
    LOG_INFO("init ok !");
    /*
    {
        res::Meta   mt;
        mt.set_id(1);
        const res::Meta * p = (const res::Meta *)rta.Find("MetaTable",&mt);
        if(p)
        {
            string ds;
            ser.Visual(p,ds);
            LOG_INFO("find out ! :%s",ds.c_str());
        }
        else
        {
            LOG_INFO("not find out !");
        }

    } 
    */
    return 0;
}

