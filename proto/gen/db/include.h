#pragma once
//generate by generate.sh do not edit this file
//generate time : Mon Oct 20 15:26:21 CST 2014
#include "Account.pb.h"
#include "LoginHistory.pb.h"
#include "Role.pb.h"

namespace db
{
	inline void init_proto_dummy()
	{
		protobuf_AddDesc_db_2fAccount_2eproto();
		protobuf_AddDesc_db_2fLoginHistory_2eproto();
		protobuf_AddDesc_db_2fRole_2eproto();
	}
};
 
