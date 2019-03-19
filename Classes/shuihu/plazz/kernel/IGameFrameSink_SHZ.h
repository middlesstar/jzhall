#ifndef SHZ_IGameFrameSink_H_
#define SHZ_IGameFrameSink_H_

class SHZIGameFrameSink
{
public:
	static SHZIGameFrameSink* getInterface();
	
	virtual ~SHZIGameFrameSink(){};

	//////////////////////////////////////////////////////////////////////////
	//���ýӿ�
public:
	//��������
	virtual void OnGFConfigServer(const tagUserAttribute& UserAttribute, const tagGameAttribute& GameAttribute, const tagServerAttribute& ServerAttribute )=0;
	//�������
	virtual void OnGFConfigFinish()=0;

	//////////////////////////////////////////////////////////////////////////
	//Socket�ӿ�
public:
	virtual bool OnGFEventSocket(int main, int sub, void* data, int dataSize)=0;
	
	//////////////////////////////////////////////////////////////////////////
	//�û��ӿ�
public:
	//�û�����
	virtual void OnGFUserEnter(SHZIClientUserItem* pIClientUserItem)=0;
	//�û�����
	virtual void OnGFUserScore(dword dwUserID, const tagUserScore& UserScore)=0;
	//�û�״̬
	virtual void OnGFUserStatus(dword dwUserID, const tagUserStatus& UserStatus)=0;
	//�û�����
	virtual void OnGFUserAttrib(dword dwUserID, const tagUserAttrib& UserAttrib)=0;
	//�û��Զ���ͷ��
	virtual void OnGFUserCustomFace(dword dwUserID,  dword dwCustomID, const tagCustomFaceInfo& CustomFaceInfo)=0;


}; // IGameFrameSink

#endif