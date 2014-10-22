#pragma once
#include "app/App.hpp"

class GateServer : public App
{
private:
    DeclareDefaultPrivateConstructor(GateServer)
    DeclareSingltonSupport(GateServer)
public:
    //return 0 is ok , otherwise exit prcess
    virtual int     OnInit();
    virtual int        OnStart();
    //control command process
    virtual string     OnCtrl(const std::vector<string> & cmdLine);
    //tick 
    virtual int     OnTick(int64_t lElapseTime);
    //poll system
    virtual int     OnPoll(int iRecommendPollNum = 1);
    //system will close for closing Reason
    virtual int     OnClosing(int closingReason);
    //destroy sth uninitializing
    virtual int     OnDestroy();
};


