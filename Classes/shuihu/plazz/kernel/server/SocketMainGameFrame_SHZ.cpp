#include "CServerItem_SHZ.h"

//��Ϸ��Ϣ
//�����Ϣ
bool SHZCServerItem::OnSocketMainGameFrame(int main,int sub, void* data, int dataSize)
{
	//Ч������
	ASSERT(dataSize<=SOCKET_TCP_PACKET);
	if (dataSize>SOCKET_TCP_PACKET) return false;
	
	if (!SHZIClientKernel::get() || !mIsGameReady)
		return true;
		
	return SHZIClientKernel::get()->OnGFEventSocket(main, sub, data, dataSize);
}
