
#include "base/File.h"
#include "base/Log.h"


#include "IniConfigParser.h"

#if 1
IniConfigParser::IniConfigParser():dic(NULL){}
IniConfigParser::~IniConfigParser()
{
    if(dic)
    {
        iniparser_freedict(dic);
        dic = NULL;
    }
}
#endif

#if 1
int     IniConfigParser::ReadConfigFile(const char* pszFileName)
{
    dic = iniparser_load(pszFileName);
    if(!dic)
    {
        LOG_ERROR("load ini file = %s error !",pszFileName);
        return -1;
    }   
    GetConfig(rootValue);
    return 0 ;
}
int     IniConfigParser::GetConfig(ConfigValue  & v)
{
    //insert     
    string key = v.key;
    if(v.key != rootValue.key && v.key.find(':') == string::npos)
    {
        key = rootValue.key + ":" +v.key;
    }
        
    if(v.key.find(':') == string::npos && 
       iniparser_find_entry(dic,v.key.c_str()))
    {
        /////////////////////////////////////
        v.dic.clear();
        char ** pp = iniparser_getseckeys(dic,(char*)v.key.c_str());
        int nk = iniparser_getsecnkeys(dic,(char*)v.key.c_str());
        ConfigValue cv;
        for(int i = 0;i < nk; ++i)
        {
            cv.key = pp[i];
            if(!GetConfig(cv))
            {
                cv.key = cv.key.substr(v.key.length()+1);
                v.dic.push_back(cv);
            }                
        }        
        return 0;
    } 
    char * sv = iniparser_getstring(dic,key.c_str(),NULL);
    if(NULL == sv)
    {
        //not found        
        return -1;
    }
    v.value = sv;
    return 0;
}
int   IniConfigParser::GetConfigInt(const char* pszKey,int defaultValue )
{
    string key = pszKey;
    if( key  != rootValue.key && key.find(':') == string::npos )
    {
        key = rootValue.key + ":" + key;
    }    
    return iniparser_getint(dic, key.c_str(),defaultValue);
}
string  IniConfigParser::GetConfigString(const char* pszKey ,const char* pszDefault)
{
    string key = pszKey;
    if( key  != rootValue.key && key.find(':') == string::npos)
    {
        key = rootValue.key + ":" + key;
    } 
    const char* p = pszDefault?pszDefault:"";
    return string(iniparser_getstring(dic,key.c_str(),(char*)p));
}
double IniConfigParser::GetConfigReal(const char* pszKey,double defaultValue )
{
    string key = pszKey;
    if( key  != rootValue.key && key.find(':') == string::npos)
    {
        key = rootValue.key + ":" + key;
    }    
    return iniparser_getdouble(dic, key.c_str(),defaultValue);
}
///////////////////////////////////////////////////////
int     IniConfigParser::CreateConfig(const ConfigValue & v,const char* pszDesc)
{

    if(!v.dic.empty())
    {
        ConfigValue sv;
        for(int i = 0;i < (int)v.dic.size();++i)
        {
            sv = v.dic[i];
            sv.key = v.key + ":" + v.dic[i].key;
            CreateConfig(sv);
        }
        return 0;
    }

    //insert     
    string section = rootValue.key;
    string key = section;
    if(v.key.find(':') != string::npos)
    {
        int idx = v.key.find(':');
        section = v.key.substr(0,idx);        
    }
    else
    {
        key += ":";
        key += v.key;
    }
        
    if(!iniparser_find_entry(dic,section.c_str()))
    {
        iniparser_set(dic,section.c_str(),"");
    }    
    
    return iniparser_set(dic,key.c_str(),v.value.c_str());
}
int     IniConfigParser::Create(const char* pszRootName )
{
    assert(dic == NULL);
    dic = dictionary_new(0);
    if(!dic)
    {
        return -1;
    }
    SetRootName(pszRootName);
    return 0;
}
///////////////////////////////////////////////////////    
int     IniConfigParser::DumpConfig(const char* pszFileName)
{
    File file(pszFileName,"wb");
    if(file.GetFilePointer())
    {
        iniparser_dump_ini(dic,file.GetFilePointer());
        return 0;
    }
    return -1;
}
#endif


