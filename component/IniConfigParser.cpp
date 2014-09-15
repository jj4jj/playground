
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
    int ns = iniparser_getnsec(dic);
    for(int i = 0;i < ns; ++i)
    {
        ConfigValue sec;
        char* p = iniparser_getsecname(dic,i);
        sec.key = p;
        int nk = iniparser_getsecnkeys(dic,p);
        char** pp = iniparser_getseckeys(dic,p);
        ConfigValue kv;
        for(int j = 0 ;j < nk; ++j)
        {
            kv.key = pp[j];
            string kp = p;
            kp += ":";
            kp += pp[j];
            kv.value = iniparser_getstring(dic,kp.c_str(),"");
            //if(GetConfig(kv))
            //{
            //    continue;
            //}
            sec.dic.push_back(kv);
        }
        rootValue.dic.push_back(sec);
    }
    return 0 ;
}
int     IniConfigParser::GetConfig(ConfigValue  & v)
{
    #define  S_INI_INVALID_KEY  ((char*)-1)
    char * sv = iniparser_getstring(dic,v.key.c_str(),NULL);
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
    return iniparser_getint(dic, pszKey,defaultValue);
}
double IniConfigParser::GetConfigReal(const char* pszKey,double defaultValue )
{
    return iniparser_getdouble(dic, pszKey,defaultValue);
}
///////////////////////////////////////////////////////
int     IniConfigParser::CreateConfig(const ConfigValue & v,const char* pszDesc)
{
    //insert 
    return iniparser_set(dic,v.key.c_str(),v.value.c_str());
}
int     IniConfigParser::Create()
{
    assert(dic == NULL);
    dic = dictionary_new(0);
    return 0;
}
///////////////////////////////////////////////////////    
int     IniConfigParser::DumpConfig(const char* pszFileName)
{
    File file(pszFileName,"wb");
    if(file.GetFilePointer())
    {
        iniparser_dump(dic,file.GetFilePointer());
        return 0;
    }
    return -1;
}
#endif


