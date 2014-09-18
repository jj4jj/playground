#pragma once
#include "base/stdinc.h"
//CMD : <cmd> <arg...>

class ConsoleControler
{
public:
    virtual    string     OnCMD(const std::vector<string> & cmdLine);    
    
public:
  ~ConsoleControler(){}  
};

