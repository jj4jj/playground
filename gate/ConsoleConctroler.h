#pragma once

//CMD : <cmd> <arg...>

class ConsoleControler
{
public:
    virtual    int     OnCMD(const char* pszCMDName,...);    
    
public:
  ~ConsoleControler(){}  

};

