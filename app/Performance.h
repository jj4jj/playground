#pragma once
#include "base/stdinc.h"

class Performance
{
public:
    double GetDownSpeed();
    double GetUpSpeed();
    double GetDownPkgSpeed();    
    double GetUpPkgSpeed();
public:
    uint32_t        m_dwOnlineNum;
    uint64_t        m_ulDownloadsize;
    uint64_t        m_ulUploadsize;
    uint64_t        m_ulReqpkgs;
    uint64_t        m_ulRsppkgs;
    uint64_t        m_ulDownpkgs;
    uint64_t        m_ulUppkgs;
    uint64_t        m_ulTransactions;
    uint64_t        m_ulTransactionMsTime;    
};


