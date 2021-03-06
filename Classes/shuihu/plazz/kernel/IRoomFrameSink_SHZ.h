#ifndef SHZ_IRoomFrameSink_H_
#define SHZ_IRoomFrameSink_H_

class SHZIRoomFrameSink
{
public:
	static SHZIRoomFrameSink* getInterface();

	virtual ~SHZIRoomFrameSink(){};
	
	//////////////////////////////////////////////////////////////////////////
	// 框架消息
public:
	//游戏已准备好
	virtual void OnGFGameReady()=0;
	//游戏关闭
	virtual void OnGFGameClose(int iExitCode)=0;

	//////////////////////////////////////////////////////////////////////////
	// Socket消息
public:
	// 发送数据
	virtual bool GFSendData(int main, int sub, void* data, int size)=0;
}; // IRoomFrameSink

#endif