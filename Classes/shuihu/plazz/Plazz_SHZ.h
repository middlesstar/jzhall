#ifndef SHZ_Plazz_H_
#define SHZ_Plazz_H_

#include "df/DF.h"

#include "libs/crypt/base/MD5.h"
#include "df/CCUserDefault.h"
#include "df/ServerListData.h"

#include "shuihu/plazz/data/GlobalUserInfo_SHZ.h"

#include "shuihu/plazz/data/OptionParameter_SHZ.h"
#include "shuihu/plazz/data/GlobalUnits_SHZ.h"

#include "Network/CSocketEngine.h"
#include "Network/CSocketMission.h"

//广场任务
#include "shuihu/plazz/plazz/GPMission_SHZ.h"

//房间服务
#include "shuihu/plazz/kernel/server/IServerItem_SHZ.h"
#include "shuihu/plazz/kernel/server/TableViewFrame_SHZ.h"

//游戏接口
#include "shuihu/plazz/kernel/game/IClientKernel_SHZ.h"

#include "libs/PacketAide.h"

#include "cocos2d.h"


#ifndef WIN32
#define _compiler_on_android_
#endif


#define PLAZZ_PRINTF cocos2d::log

#endif // _Plazz_H_