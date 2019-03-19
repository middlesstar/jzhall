//
//  CGPQueryMission.h
//  XiYouJi
//
//  Created by rinex on 15-1-6.
//
//

#ifndef SHZ__XiYouJi__CGPQueryMission__
#define SHZ__XiYouJi__CGPQueryMission__
#include "shuihu/plazz/Plazz_SHZ.h"
#include <stdio.h>

class SHZIQueryResult{
public:
    virtual void onQueryResult(int state)=0;
};



class SHZQueryMission: public CSocketMission
{
public:
    SHZQueryMission(const char* url, int port);
    
public:
    inline void setQueryResultSink(SHZIQueryResult* IQuerySink){ mQuerySink =IQuerySink; }
    
    //实现接口
public:
    virtual void onEventTCPSocketLink();
    virtual void onEventTCPSocketShut();
    virtual void onEventTCPSocketError(int errorCode);
    virtual bool onEventTCPSocketRead(int main, int sub, void* data, int dataSize);
    
public:
    void onQuery();
private:
    SHZIQueryResult* mQuerySink;
    
};
#endif /* defined(__XiYouJi__CGPQueryMission__) */
