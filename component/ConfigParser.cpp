
#include "ConfigParser.h"

#if 1
ConfigParser::ConfigParser()
{
    SetRootName();
}
ConfigParser::~ConfigParser()
{

}
#endif
#if 1
void     ConfigParser::SetRootName(const char* pszRootName )
{
    if(pszRootName)
    {
        rootValue.key = pszRootName;
    }
    else
    {
        rootValue.key = "root";
    }
}

void     ConfigParser::VisualConfig(string & s)
{
    VisualConfig(s,rootValue,0,"\t");
}
void     ConfigParser::VisualConfig(string & s,ConfigValue & v,int level,const char* prefix)
{
    if(v.dic.empty())
    {
        for(int j = 0 ;  j < level ;++j)
        {
            s+= prefix;
        }
        s+=v.key;
        s+=" <- [";
        s+=v.value;
        s+="]\n";
        return ;
    }
    else
    {
        for(int j = 0 ;  j < level ;++j)
        {
            s+= prefix;
        }
        s+=v.key;
        s+=" <- [";
        s+="]\n";
        for(int i = 0;i < (int)v.dic.size() ; ++i)
        {
            VisualConfig(s,v.dic[i],level+1,prefix);
        }
    }
}
#endif

