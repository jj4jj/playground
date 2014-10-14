#pragma once
#include "base/stdinc.h"
#include "datacenter/MetaSerializer.h"
class   ResTableAgent
{
public:
    int       Init(const char* pszResDataDir,MetaSerializer * meta);
    int       ReLoad();
public:
    const void*     Find(const char* pszName,void*  obj);
    const void*     LowerBound(const char* pszName,void*    obj);
    const void*     UpperBound(const char* pszName,void*    obj);     
    const void*     Random(const char* pszName);
    vector<const void*>  & GetAll(const char* pszName);

    //////////////////////////////////////////////////////
    uint64_t  GetResTableEntryKey(const void*   pObj);
    string    GetMetaNameSpace(){return "res";}
private:
    //obj key -> mm table entry
    typedef std::map<uint64_t,const void*>                                MapResTable;
    typedef MapResTable::iterator                                   MapResTableItr;
    //name -> mem table
    typedef unordered_map<string,MapResTable>                       MapNameResTable;
    typedef MapNameResTable::iterator                               MapNameResTableItr;
    //name -> raw table
    typedef unordered_map<string,MetaSerializer::MetaObjectPtr>     MapRawMetaResTable;
    typedef unordered_map<string, vector<const void*> >                   MapNameCacheTable;
    typedef MapNameCacheTable::iterator                             MapNameCacheTableItr;
    MapRawMetaResTable      m_mpRawMetaResTable;
    MapNameResTable         m_mpAllNameTables;
    string                  m_strResDataDir;
    MetaSerializer      *   m_metaSeri;
    int                     m_iLoadResNum;
    MapNameCacheTable       m_mpNameCacheTable;
    //////////////////////////////////////
    
};



