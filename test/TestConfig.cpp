
#include "component/IniConfigParser.h"

#include "base/stdinc.h"


#include <iostream>

using namespace std;
int main(int argc ,char* argv[])
{

    IniConfigParser parser;

    
    const char* pszFile = "TestConfig.cfg";
    if(parser.ReadConfigFile(pszFile))
    {
        parser.Create();
        ConfigValue v;
        char key[32];
        char value[128];
        for(int i = 0;i < 10; ++i)
        {
            snprintf(key,sizeof(key),"key-%d",i);
            snprintf(value,sizeof(value),"value-%d",i*i*i);
            v.key = key;
            v.value = value;
            parser.CreateConfig(v);
        }
        parser.DumpConfig(pszFile);
    }
    else
    {
        char key[32];
        ConfigValue v;
        for(int i = 0 ;i < 12; ++i)
        {
            snprintf(key,sizeof(key),"key-%d",i);
            v.key = key;
            parser.GetConfig(v);
            //cout<<key<<":"<<v.value<<endl;                 
        }
    }
    string s;
    parser.VisualConfig(s);
    cout<<s<<endl;
    return 0;
}


