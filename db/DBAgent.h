#pragma once
#include "base/stdinc.h"
#include "dep/ssdb/api/cpp/SSDB.h"

struct DBTable;
class DBTableHandler
{
public:
    void    SetTable(DBTable* _pTable):m_pTable(_pTable){}
    ~DBTableHandler(){}
public:        
	virtual int 	OnGet();
	virtual int 	OnMGet();
	virtual int 	OnRemove();
	virtual int 	OnUpdate();
	virtual int 	OnInsert();
	virtual int 	OnGetKeyRank();
	virtual int 	OnGetRankKey();
	virtual int 	OnCount();
private:
    DBTable * m_pTable;    
};
typedef shared_ptr<DBTableHandler>  DBTableHandlerPtr;
////////////////////////////////////////////////////////////

struct DBTable
{
    enum{
        DBTABLE_TYPE_KV    = 1,
        DBTABLE_TYPE_KHMAP = 2,
        DBTABLE_TYPE_KZSET = 3,        
    };
    string name;
    int    type;
    DBTableHandlerPtr   handler;    
};

////////////////////////////////////////////////////////////

struct DBProxyAddr
{
    string ip;
    int    port;    
};

class DBAgent
{
public:
    DBAgent();
    ~DBAgent();
protected:
    static uint32_t Hash(const char* pszTableName);    
public:
    //return the handle
    int     RegisterTable(const DBTable &   table);
    int     Init(const std::vector<DBProxyAddr> & proxyList)	//proxy list
    {
        assert(clientProxyList.empty());
        ssdb::Client * cli;
        for(uint i = 0 ;i < proxyList.size(); ++i)
        {
            cli = ssdb::Client::connect(proxyList[i].ip, proxyList[i].port);
            if(!cli)
            {
                LOG_ERROR("proxy idx = %d (ip=%s,port=%d) connect error ! ",i,
                            proxyList[i].ip.c_str(),proxyList[i].port);
                continue;
            }            
            clientProxyList.push_back(cli);            
            
        }

    }
	int     Polling(long lTimeOutUs);	
    void    Destroy();
	//-------------------------------------------------------------------------
	int		Get(const char* pszTableName,const Buffer & key,const Buffer & cb);
	int     MGet(const char* pszTableName,const char* pszTableName ,
                    const std::vector<Buffer> & keylist,const Buffer & cb);
	int     Remove(const char* pszTableName ,const Buffer & key,const Buffer & cb);	
	int     Update(const char* pszTableName ,const Buffer & key,const Buffer & value,const Buffer & cb);
	int     Insert(const char* pszTableName ,const Buffer & key,const Buffer & value,const Buffer & cb);
	int     GetKeyRank(const char* pszTableName ,const Buffer & key,const Buffer & cb);
	int     GetRankKey(const char* pszTableName ,rank,const Buffer & cb);
	int     Count(const char* pszTableName ,const Buffer & cb);		
	//----------------------------------------------------------------------------------
	int     OnGet(hanlde,ret,const Buffer & key,const Buffer & value,const Buffer & cb);
	int     OnMGet(const char* pszTableName ,ret,const std::vector<Buffer> & keylist,
                         const std::vector<Buffer> & valuelist,
                         const Buffer & cb,bool more);
	int     OnRemove(const char* pszTableName ,ret,const Buffer & key,const Buffer & cb);
	int     OnUpdate(const char* pszTableName ,ret,const Buffer & key,const Buffer & cb);
	int     OnInsert(const char* pszTableName ,ret,const Buffer & key,const Buffer & cb);
	int     OnGetKeyRank(const char* pszTableName ,const Buffer & key,const Buffer & cb);
	int     OnGetRankKey(const char* pszTableName ,rank,const Buffer & cb);
	int     OnCount(const char* pszTableName ,const Buffer & cb); 


private:
    //name hash -> table
    unordered_map<uint32_t,DBTable>  handlerMap;
    std::vector<DBProxyAddr>         proxyAddrList;
    //ssdb handle 
    //map with -> proxy
    std::vector<ssdb::Client *>   clientProxyList;
};






