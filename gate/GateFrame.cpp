#include "base/Buffer.h"
#include "GateFrame.h"


#if 1

GateFrame::GateFrame(char* p,uint16_t sz):pData(p),size(sz)
{

}
GateFrame::GateFrame(const Buffer & buf):pData((char*)(buf.pBuffer)),size(buf.iUsed)
{

}

#endif
