#pragma once


enum    DataResultCode
{
    DATA_TIME_OUT      =  -1,
    DATA_OK            =   0,
    DATA_NO_EXISTS         ,
    DATA_ALREADY_EXISTS    ,
    DATA_UNPACK_ERROR      ,
    DATA_INTERNAL_ERR      ,
    DATA_REDIS_ERR_START   =    1000,
    DATA_MYSQL_ERR_START   =    2000,
};

///////////////////////////////////////////////////////////////
enum    CacheCallBackTypeCmd
{
    CACHE_CMD_INVALID   =   0,
    CACHE_CMD_GET       =   1,
    CACHE_CMD_UPDATE,
    CACHE_CMD_INSERT,
    CACHE_CMD_REMOVE,
    CACHE_CMD_MGET,
    CACHE_CMD_MUPDATE,
    CACHE_CMD_MINSERT,
    CACHE_CMD_MREMOVE,
    
    //////////////////////////
    CACHE_CMD_MAX,
};
enum    CacheCollectionType
{
    CACHE_TYPE_KV,
    CACHE_TYPE_KH,
    CACHE_TYPE_KZ,
    CACHE_TYPE_KL,    
    //////////////////////////
    CACHE_TYPE_MAX,
};

#pragma pack(1)
struct CacheAgentCallBack
{
    enum {MAX_TYPE_KEY_SIZE = 16,
          MAX_OBJ_KEY_SIZE  = 64,
          MAX_CALL_BACK_SIZE  = 128,
         };
    //========required==========
    struct Head
    {
        uint16_t    length;
        uint8_t     type;
        uint16_t    cmd;
        char        szTypeKey[MAX_TYPE_KEY_SIZE];
    } head;
    //=========optional=========
    struct Body
    {
        char        szObjKey[MAX_OBJ_KEY_SIZE];
        
    } body;
    //=========optional========= if it's exists , body must be exist ==============
    union
    {
        //EXTENTION here
        
        /////////////////////////////////////////////////////////////////////////////
        uint8_t reserve_cmd_data[MAX_CALL_BACK_SIZE - sizeof(Head) - sizeof(Body)];
    }   u_extension_data;
};
#pragma pack()
///////////////////////////////////////////////////////////////





