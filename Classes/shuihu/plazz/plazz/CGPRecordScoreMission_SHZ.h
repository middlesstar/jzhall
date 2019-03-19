//
//  CGPRecordMission.h
//  ShuiHu
//
//  Created by rinex on 15-1-23.
//
//

#ifndef SHZ_ShuiHu_CGPRecordMission_h
#define SHZ_ShuiHu_CGPRecordMission_h
#include "shuihu/plazz/Plazz_SHZ.h"



class SHZCGPRecordScoreMission
 : public CSocketMission
{
    
public:
    SHZCGPRecordScoreMission(const char* url, int port);
    
public:
    virtual void onEventTCPSocketLink();
    virtual void onEventTCPSocketShut();
    virtual void onEventTCPSocketError(int errorCode);
    virtual bool onEventTCPSocketRead(int main, int sub, void* data, int dataSize);
    
public:
    void  sendRecord();
};
#endif
