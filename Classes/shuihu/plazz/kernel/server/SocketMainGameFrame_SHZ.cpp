#include "CServerItem_SHZ.h"

//游戏消息
//框架消息
bool SHZCServerItem::OnSocketMainGameFrame(int main,int sub, void* data, int dataSize)
{
	//效验数据
	ASSERT(dataSize<=SOCKET_TCP_PACKET);
	if (dataSize>SOCKET_TCP_PACKET) return false;
	
	if (!SHZIClientKernel::get() || !mIsGameReady)
		return true;
		
	return SHZIClientKernel::get()->OnGFEventSocket(main, sub, data, dataSize);
}
