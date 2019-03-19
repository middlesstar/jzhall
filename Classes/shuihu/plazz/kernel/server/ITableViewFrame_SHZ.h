#ifndef SHZ_ITableViewFrame_H_
#define SHZ_ITableViewFrame_H_

#include "shuihu/plazz/Plazz_SHZ.h"

class SHZITableViewFrame;

//������ͼ
class SHZITableView
{
public:
	virtual ~SHZITableView(){};

	//���ܽӿ�
public:
	//��������
	virtual word GetNullChairCount(word & wNullChairID)=0;
	//���ú���
	virtual void InitTableView(word wTableID, word wChairCount, SHZITableViewFrame * pITableFrameView)=0;

	//�û��ӿ�
public:
	//��ȡ�û�
	virtual SHZIClientUserItem * GetClientUserItem(word wChairID)=0;
	//������Ϣ
	virtual bool SetClientUserItem(word wChairID, SHZIClientUserItem * pIClientUserItem)=0;

	//���ýӿ�
public:
	//������
	virtual void SetFocusFrame(bool bFocusFrame)=0;
	//����״̬ 
	virtual void SetTableStatus(bool bPlaying, bool bLocker)=0;

	//��ѯ�ӿ�
public:
	//��Ϸ��־
	virtual bool GetPlayFlag()=0;
	//�����־
	virtual bool GetLockerFlag()=0;
};

//���ӿ��
class SHZITableViewFrame
{
public:
	virtual ~SHZITableViewFrame(){};

	//���ýӿ�
public:
	//��������
	virtual bool ConfigTableFrame(word wTableCount, word wChairCount, dword dwServerRule, word wServerType, word wServerID)=0;

	//��Ϣ�ӿ�
public:
	//������Ŀ
	virtual word GetTableCount()=0;
	//������Ŀ
	virtual word GetChairCount()=0;

	//�û��ӿ�
public:
	//��ȡ�û�
	virtual SHZIClientUserItem * GetClientUserItem(word wTableID, word wChairID)=0;
	//������Ϣ
	virtual bool SetClientUserItem(word wTableID, word wChairID, SHZIClientUserItem * pIClientUserItem)=0;

	//״̬�ӿ�
public:
	//��Ϸ��־
	virtual bool GetPlayFlag(word wTableID)=0;
	//�����־
	virtual bool GetLockerFlag(word wTableID)=0;
	//������
	virtual void SetFocusFrame(word wTableID, bool bFocusFrame)=0;
	//����״̬ 
	virtual void SetTableStatus(word wTableID, bool bPlaying, bool bLocker)=0;

	//��ͼ����
public:
	//��������
	virtual bool FlashGameTable(word wTableID)=0;
	//��������
	virtual bool FlashGameChair(word wTableID, word wChairID)=0;

	//���ܽӿ�
public:
	//��������
	virtual bool UpdateTableView(word wTableID)=0;
	//��ȡ����
	virtual SHZITableView * GetTableViewItem(word wTableID)=0;
	//��������
	virtual word GetNullChairCount(word wTableID, word & wNullChairID)=0;
};

#endif // _ITableViewFrame_H_