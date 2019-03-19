 //
//  CGPRecordMission.cpp
//  ShuiHu
//
//  Created by rinex on 15-1-23.
//
//

#include "CGPRecordScoreMission_SHZ.h"

extern long long g_llWinScore_SHZ;

SHZCGPRecordScoreMission::SHZCGPRecordScoreMission(const char* url, int port)
: CSocketMission(url, port)
{
    
}

void SHZCGPRecordScoreMission::sendRecord(){
    
    start();
    
}
void SHZCGPRecordScoreMission::onEventTCPSocketLink(){
    
    
    PACKET_AIDE_SIZE(16);
    
    SHZCGlobalUserInfo   * pGlobalUserInfo = SHZCGlobalUserInfo::GetInstance();
    tag_SHZGlobalUserData * pUserData       = pGlobalUserInfo->GetGlobalUserData();
    
    
    unsigned long userId = pUserData->dwUserID;
    unsigned long gameId = pUserData->dwGameID;
    
    //char str[64];
    //
    //sprintf(str, "record_%lu",gameId);
    //
    //std::string strScore = UserDefault::getInstance()->getStringForKey("ACCOUNT", str, "");
    //
    //if(strScore.empty()){ return;}
    //
    //long long score = atoll(strScore.c_str());
    
	long long score = g_llWinScore_SHZ;


    packet.write4Byte(userId);
    packet.write4Byte(203);
    packet.write8Byte(score);

    send(MDM_GP_USER_SERVICE_SHZ,SUB_GP_RECORD_RANK_DATA_SHZ, packet.getBuffer(),packet.getPosition());
    
	g_llWinScore_SHZ = 0;

    /*UserDefault::getInstance()->setStringForKey("ACCOUNT", str, "");*/
}
void SHZCGPRecordScoreMission::onEventTCPSocketShut(){
    
}
void SHZCGPRecordScoreMission::onEventTCPSocketError(int errorCode){
    
}
bool SHZCGPRecordScoreMission::onEventTCPSocketRead(int main, int sub, void* data, int dataSize){

    return true;
}