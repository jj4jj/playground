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



