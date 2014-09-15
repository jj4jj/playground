
#include "ConfigParser.h"

#if 1
ConfigParser::ConfigParser()
{
    rootValue.key = "root";
}
ConfigParser::~ConfigParser()
{

}
#endif
#if 1
void     ConfigParser::VisualConfig(string & s)
{
    VisualConfig(s,rootValue,0,"\t");
}
void     ConfigParser::VisualConfig(string & s,ConfigValue & v,int level,const char* prefix)
{
    if(v.dic.empty())
    {
        s+=string(prefix,level);
        s+=v.key;
        s+=":";
        s+=rootValue.value;
        s+="\n";
        return ;
    }
    else
    {
        s+=string(prefix,level);
        s+=v.key;
        s+=":";
        s+="\n";
        for(int i = 0;i < (int)v.dic.size() ; ++i)
        {
            VisualConfig(s,v.dic[i],level+1,prefix);
        }
    }
}
#endif

