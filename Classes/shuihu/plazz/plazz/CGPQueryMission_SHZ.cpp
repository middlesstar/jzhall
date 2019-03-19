//
//  CGPQueryMission.cpp
//  XiYouJi
//
//  Created by rinex on 15-1-6.
//
//

#include "CGPQueryMission_SHZ.h"






SHZQueryMission::SHZQueryMission(const char* url, int port)
:CSocketMission(url,port)
{
    mQuerySink = 0;
    PLAZZ_PRINTF("CGPShareMission Construction: ip = %s ,port = %d ", url, port);
}


void SHZQueryMission::onEventTCPSocketLink()
{
    
    SHZCGlobalUserInfo   * pGlobalUserInfo = SHZCGlobalUserInfo::GetInstance();
    tag_SHZGlobalUserData * pGlobalUserData = pGlobalUserInfo->GetGlobalUserData();
    
    PACKET_AIDE_SIZE(64);
    packet.write4Byte(pGlobalUserData->dwGameID);
    
    send(MDM_GP_USER_SERVICE_SHZ, SUB_GP_QUERY_FIRST_CHARGE_SHZ, packet.getBuffer(), packet.getPosition());
    
    PLAZZ_PRINTF("share send data\n");
}


void SHZQueryMission::onQuery()
{
    if (mQuerySink) start();
    else PLAZZ_PRINTF("mShareSink = 0 ");
    
}


void SHZQueryMission::onEventTCPSocketError(int errorCode)
{
    PLAZZ_PRINTF("CGPShareMission-->Socket Error! errorCode = %d\n", errorCode);
}

void SHZQueryMission::onEventTCPSocketShut()
{
    PLAZZ_PRINTF("CGPshareMission-->Socket Shut\n");
}

bool SHZQueryMission::onEventTCPSocketRead(int main, int sub, void* data, int dataSize)
{
    
    if (main != MDM_GP_USER_SERVICE_SHZ)
    {
        return false;
    }
    
    
    if (SUB_GP_QUERY_RESULT_SHZ == sub)
    {
        PACKET_AIDE_DATA(data);
        uint32 uIState     = packet.read4Byte();
        mQuerySink->onQueryResult(uIState);
        
        PLAZZ_PRINTF("Receive data \n");
        
    }
    
    return false;
}