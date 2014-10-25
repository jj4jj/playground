
#include "base/Log.h"
#include "CSRoleHandler.h"

void  CSRoleHandler::GetPlayerRoleBase(cs::CSRoleBase & rolebase,PlayerAgent & player)
{
    rolebase.set_uid(player.GetUID());
    rolebase.set_name(player.GetName());
    rolebase.set_level(player.GetLevel());
}
void  CSRoleHandler::GetPlayerRoleExt(cs::CSRoleExt & roleext,PlayerAgent & player )
{
    roleext.set_dummy(player.GetUID());
}
void  CSRoleHandler::GetPlayerRoleMisc(cs::CSRoleMisc & rolemisc,PlayerAgent & player)
{
    rolemisc.set_dummy(player.GetUID());
}
int   CSRoleHandler::OnPlayingMsg(Session & session,const cs::CSMsg & msg)
{
    if(!msg.has_role())
    {
        LOG_ERROR("session uin = %lu req package has no role package !",session.GetUID());
        return -1;
    }
    const cs::CSMsgRole & rolemsgreq =  msg.role();
    int ret = 0;
    cs::CSMsg rsp;
    rsp.set_cmd(cs::CSMsg::CSMSG_CMD_ROLE);
    switch(rolemsgreq.cmd())
    {
    case cs::CSMsgRole::CSMSG_ROLE_ALL_INFO_REQ:
    {
        cs::CSMsgRole & rolersp = *rsp.mutable_role();        
        rolersp.set_cmd(cs::CSMsgRole::CSMSG_ROLE_ALL_INFO_RSP);
        cs::CSRole & roleinfo  = * rolersp.mutable_allinforsp();
        cs::CSRoleBase & rolebase = *roleinfo.mutable_base();
        cs::CSRoleExt &  roleext = *roleinfo.mutable_ext();
        cs::CSRoleMisc &  rolemisc = *roleinfo.mutable_misc();
        GetPlayerRoleBase(rolebase,*session.GetPlayerAgent());
        GetPlayerRoleExt(roleext,*session.GetPlayerAgent());
        GetPlayerRoleMisc(rolemisc,*session.GetPlayerAgent());
        session.Send(rsp);
    }
        //send csrole
        break;
    case cs::CSMsgRole::CSMSG_ROLE_BASE_INFO_REQ:
    {
        cs::CSMsgRole & rolersp = *rsp.mutable_role();        
        cs::CSRole & roleinfo  = * rolersp.mutable_allinforsp();
        rolersp.set_cmd(cs::CSMsgRole::CSMSG_ROLE_BASE_INFO_RSP);
        cs::CSRoleBase &  rolebase = *roleinfo.mutable_base();
        GetPlayerRoleBase(rolebase,*session.GetPlayerAgent());
        session.Send(rsp);
    }
        break;
    case cs::CSMsgRole::CSMSG_ROLE_EXT_INFO_REQ:
    {
        cs::CSMsgRole & rolersp = *rsp.mutable_role();        
        cs::CSRole & roleinfo  = * rolersp.mutable_allinforsp();
        rolersp.set_cmd(cs::CSMsgRole::CSMSG_ROLE_EXT_INFO_RSP);
        cs::CSRoleExt &  roleext = *roleinfo.mutable_ext();
        GetPlayerRoleExt(roleext,*session.GetPlayerAgent());
        session.Send(rsp);
    }
        break;
    case cs::CSMsgRole::CSMSG_ROLE_MISC_INFO_REQ:
    {
        cs::CSMsgRole & rolersp = *rsp.mutable_role();        
        cs::CSRole & roleinfo  = * rolersp.mutable_allinforsp();
        rolersp.set_cmd(cs::CSMsgRole::CSMSG_ROLE_MISC_INFO_RSP);
        cs::CSRoleMisc &  rolemisc = *roleinfo.mutable_misc();
        GetPlayerRoleMisc(rolemisc,*session.GetPlayerAgent());
        session.Send(rsp);
    }
        break;
    }
    return ret;
}

