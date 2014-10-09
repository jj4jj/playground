
#include "DataAgent.h"
#include "base/Log.h"
//entry is protobuffer type

DataSet::DataSet()
{
    //todo
}
DataSet::~DataSet()
{
    //todo
}
//return EINPROGRESS  if insert to remote
//return 0 is insert ok ( pure local cache)
//otherwise return error code
int     DataSet::Insert(void* entry,const Buffer & cb)
{

    if(Find(entry,cb,true))
    {
        LOG_ERROR("insert repeatly !");
        return -1;
    }

    if(DATA_TYPE_PURE_LOCAL_CACHE == option.setType)
    {
        //
        DataSetEntry    dse;
        dse.construct();
        dse.pData = entry;
        return  ht.Insert(&dse);
    }
    else
    if(DATA_TYPE_REMOTE_CACHE == option.setType ||
       DATA_TYPE_REMOTE_CACHE_AND_DB == option.setType )
    {
        //find in remote cache
        if(cacheAgent->Insert(entry,cb))
        {
            LOG_ERROR("cache get error !");
            return -2;
        }
        return RINPROGRESS;
    }
    else
    if(DATA_TYPE_PURE_DB == option.setType)
    {
        if(dbAgent->Insert(entry,cb))
        {
            return -2;
        }
        return RINPROGRESS;
    }
    else
    {
        LOG_FATAL("data type = %d is error !",option.setType);
        return -100;
    }
    return -1000;
}
int     DataSet::Remove(void* entry,const Buffer & cb)
{
    if(DATA_TYPE_PURE_LOCAL_CACHE == option.setType)
    {
        //            
        return ht.Remove(entry);
    }
    else
    if(DATA_TYPE_REMOTE_CACHE == option.setType ||
       DATA_TYPE_REMOTE_CACHE_AND_DB == option.setType )
    {
        //find in remote cache
        if(cacheAgent->Get(entry,cb))
        {
            LOG_ERROR("cache get error !");
            return -2;
        }
        if(DATA_TYPE_REMOTE_CACHE_AND_DB == option.setType)
        {
            if(dbAgent->Remove(entry,cb))
            {
                LOG_ERROR("cache get error !");
                return -2;
            }
        }
        return RINPROGRESS;
    }
    else
    if(DATA_TYPE_PURE_DB == option.setType)
    {
        if(dbAgent->Remove(entry,cb))
        {
            LOG_ERROR("cache get error !");
            return -2;
        }
        return RINPROGRESS;
    }
    else
    {
        LOG_FATAL("data type = %d is error !",option.setType);
        return -100;
    }
    return -1000;
}
int     DataSet::Update(void* entry ,const Buffer & cb,vector<string> * pFields )
{
    vector<string> fields ;
    if(pFields)
    {
       fields = *pFields;
    }

    if(DATA_TYPE_PURE_LOCAL_CACHE == option.setType)
    {
        //
        void* p = Find(entry,cb,true);
        if(p)
        {
            
            // TODO 
            //memcpy(p,entry,option.set)
            return 0;
        }
        else
        {
            LOG_ERROR("not exist !");
            return 10;
        }
    }
    else
    if(DATA_TYPE_REMOTE_CACHE == option.setType ||
       DATA_TYPE_REMOTE_CACHE_AND_DB == option.setType )
    {
        //find in remote cache
        if(cacheAgent->Update(entry,cb))
        {
            LOG_ERROR("cache get error !");
            return -2;
        }
        if(DATA_TYPE_REMOTE_CACHE_AND_DB == option.setType)
        {
            if(dbAgent->Update(entry,fields,cb))
            {
                return -2;
            }
        }
        return RINPROGRESS;
    }
    else
    if(DATA_TYPE_PURE_DB == option.setType)
    {
        if(dbAgent->Update(entry,fields,cb))
        {
            return -2;
        }
        return RINPROGRESS;
    }
    else
    {
        LOG_FATAL("data type = %d is error !",option.setType);
        return -100;
    }
    return -1000;        
}

//if local cache (pure)
//if finded return addr
//if remote return EINPROGRESS
//if not found , return NULL
void*   DataSet::Find(void* entry , const Buffer & cb,bool justFindLocal )
{
    if(DATA_TYPE_PURE_LOCAL_CACHE == option.setType ||
       justFindLocal )
    {
        //
        return ht.Find(entry);
    }
    else
    if(DATA_TYPE_REMOTE_CACHE == option.setType ||
       DATA_TYPE_REMOTE_CACHE_AND_DB == option.setType )
    {
        //find in remote cache
        if(cacheAgent->Get(entry,cb))
        {
            LOG_ERROR("cache get error !");
            return NULL;
        }
        if(DATA_TYPE_REMOTE_CACHE_AND_DB == option.setType)
        {
            vector<string>  fields;
            if(dbAgent->Get(entry,fields,cb))
            {
                return NULL;
            }
        }
        //TODO
        //void* p = RINPROGRESS;
        return NULL;
    }
    else
    if(DATA_TYPE_PURE_DB == option.setType)
    {
        vector<string>  fields;
        if(dbAgent->Get(entry,fields,cb))
        {
            //todo return error
            return NULL;
        }
        return NULL;
        //return RINPROGRESS;
    }
    else
    {
        LOG_FATAL("data type = %d is error !",option.setType);
        return NULL;
        //return -100;
    }
    //return -1000;  
    return NULL;
}




#if 1

const char* DataAgent::GetErrorString(int err)
{    
    return "nil(todo)";
}

#endif

