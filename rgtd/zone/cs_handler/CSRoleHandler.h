
#include "CSMsgHandler.h"

class CSRoleHandler : public CSMsgHandler
{
public:
    virtual int   OnPlayingMsg(Session & session,const cs::CSMsg & msg);
protected:
    void  GetPlayerRoleBase(cs::CSRoleBase & rolebase ,PlayerAgent & player);
    void  GetPlayerRoleExt(cs::CSRoleExt & roleext,PlayerAgent & player );
    void  GetPlayerRoleMisc(cs::CSRoleMisc & rolemisc,PlayerAgent & player);
};



