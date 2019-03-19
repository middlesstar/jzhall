#ifndef SHZ_IRoomFrameSink_H_
#define SHZ_IRoomFrameSink_H_

class SHZIRoomFrameSink
{
public:
	static SHZIRoomFrameSink* getInterface();

	virtual ~SHZIRoomFrameSink(){};
	
	//////////////////////////////////////////////////////////////////////////
	// �����Ϣ
public:
	//��Ϸ��׼����
	virtual void OnGFGameReady()=0;
	//��Ϸ�ر�
	virtual void OnGFGameClose(int iExitCode)=0;

	//////////////////////////////////////////////////////////////////////////
	// Socket��Ϣ
public:
	// ��������
	virtual bool GFSendData(int main, int sub, void* data, int size)=0;
}; // IRoomFrameSink

#endif