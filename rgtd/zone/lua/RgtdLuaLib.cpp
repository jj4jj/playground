
#include "base/Log.h"
#include "script/LuaAgent.h"
#include "ZoneAgent.h"
#include "DBProxy.h"
#include "account/Session.h"
#include "account/PlayerAgent.h"
#include "cs_handler/CSMsgLuaHandler.h"

/////////////////////////////////////////////////////////////////////////////////////
//static function export to lua state context

//rgtd_log("log","hello")
static int  rgtd_log(lua_State * pls);
//rgtd_register_cs_cmd(12590)
static int  rgtd_register_cs_cmd(lua_State * pls);




//rgtd_command(commanddata)
static int  rgtd_command(lua_State * pls);



//rgtd_session_send(session,msg)
static int  rgtd_session_send(lua_State * pls);
//rgtd_pack_msg(table)
static int  rgtd_pack_csmsg(lua_State * pls);
//rgtd_send_csmsg(session,cstable)
static int  rgtd_send_csmsg(lua_State * pls);
//rgtd_res_table_find("ResTable",table)
static int  rgtd_res_table_find(lua_State * pls);
//rgtd_knapsack_add_item(player,id,num)
static int  rgtd_knapsack_add_item(lua_State * pls);
//rgtd_knapsack_dec_item(player,id,num)
static int  rgtd_knapsack_dec_item(lua_State * pls);
//rgtd_
static int  rgtd_knapsack_create_item(lua_State * pls);

/////////////////////////////////////////////////////////////////////////////////////
void    CreateRgtdLuaLib(lua_State * pls)
{
    //register luastate
    EXPORT_FUNCTION(pls,rgtd_log);
    EXPORT_FUNCTION(pls,rgtd_register_cs_cmd);
    
    
}
/////////////////////////////////////////////////////////////////////////////////////
//function implementation

static int  rgtd_register_cs_cmd(lua_State * pls)
{
    int cmd = lua_tointeger(pls,-1);
    CSMsgLuaHandler*  luahandler = (CSMsgLuaHandler *)lua_touserdata(pls,-2);
    if(!luahandler)
    {
        LOG_ERROR("register cs cmd handler = null !");
        lua_pushinteger(pls,-1);
    }
    else
    {
        int ret = luahandler->RegisterCMD((uint32_t)cmd);
        lua_pushinteger(pls,ret);
    }
    return 1;
}

//rgtd_log("log","hello")
static int  rgtd_log(lua_State * pls)
{
    const char* content = lua_tostring(pls,-1);
    if(!content)
    {
        LOG_ERROR("rgtd log content is null !");
        return 0;
    }
    const char* level = lua_tostring(pls,-2);
    if(!level)
    {
        LOG_ERROR("rgtd log level is null !");
        return 0;
    }
    if(strcmp(level,"debug") == 0)
    {
        LOG_DEBUG(content);
    }
    else if(strcmp(level,"info") == 0)
    {
        LOG_INFO(content);
    }
    else if(strcmp(level,"warn") == 0)
    {
        LOG_WARN(content);
    }
    else if(strcmp(level,"error") == 0)
    {
        LOG_ERROR(content);
    }
    else if(strcmp(level,"fatal") == 0)
    {
        LOG_FATAL(content);
    }
    else
    {
        LOG_ERROR("rgtd log level = %s not support !",level);
    }    
    return 0;        
}








