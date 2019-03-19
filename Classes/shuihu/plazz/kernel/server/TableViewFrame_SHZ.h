#ifndef SHZ_TableViewFrame_H_
#define SHZ_TableViewFrame_H_

#include <vector>
#include "ITableViewFrame_SHZ.h"

//////////////////////////////////////////////////////////////////////////////////
//������ͼ
class SHZCTableView : public SHZITableView
{
	//��������
protected:
	//���ӱ�־
	bool							mIsLocker;							//�����־
	bool							mIsPlaying;							//��Ϸ��־
	bool							mIsFocusFrame;						//��ܱ�־

	//����״̬
	word							mWatchCount;						//�Թ���Ŀ
	dword							mTableOwnerID;						//��������

	//���Ա���
	word							mTableID;							//���Ӻ���
	word							mChairCount;						//������Ŀ
	SHZIClientUserItem *				mIClientUserItem[MAX_CHAIR];		//�û���Ϣ

	//����ӿ�
protected:
	SHZITableViewFrame *				mITableViewFrame;					//���ӽӿ�

	//��������
public:
	//���캯��
	SHZCTableView();
	//��������
	virtual ~SHZCTableView();

	//���ܽӿ�
public:
	//��������
	virtual word GetNullChairCount(word & wNullChairID);
	//���ú���
	virtual void InitTableView(word wTableID, word wChairCount, SHZITableViewFrame * pITableViewFrame);

	//�û��ӿ�
public:
	//��ȡ�û�
	virtual SHZIClientUserItem * GetClientUserItem(word wChairID);
	//������Ϣ
	virtual bool SetClientUserItem(word wChairID, SHZIClientUserItem * pIClientUserItem);

	//��ѯ�ӿ�
public:
	//��Ϸ��־
	virtual bool GetPlayFlag() { return mIsPlaying; }
	//�����־
	virtual bool GetLockerFlag() { return mIsLocker; }

	//״̬�ӿ�
public:
	//������
	virtual void SetFocusFrame(bool bFocusFrame);
	//����״̬ 
	virtual void SetTableStatus(bool bPlaying, bool bLocker);
};

//////////////////////////////////////////////////////////////////////////////////

//���鶨��
typedef std::vector<SHZCTableView *>		CTableViewArray_SHZ;					//��������

//���ӿ��
class SHZCTableViewFrame 
	: public SHZITableViewFrame
{
	//��Ԫ����
	friend class SHZCTableView;

	//���Ա���
protected:
	word							mTableCount;						//��Ϸ����
	word							mChairCount;						//������Ŀ
	dword							mServerRule;						//�������
	word							mServerType;						//��������

	//�ؼ�����
public:
	CTableViewArray_SHZ					mTableViewArray;					//��������

	//��������
public:
	dword							mMatchWaittingCount;				//�ȴ�����
	dword							mMatchTotalUser;					//��������
	dword							mMatchBestRank;						//��óɼ�
	dword							mMatchJoinCount;					//��������
	byte							mMatchStatus;						//����״̬
	tagMatchDesc					mMatchDesc;							//��Ϣ����

	//��������
public:
	//���캯��
	SHZCTableViewFrame();
	//��������
	virtual ~SHZCTableViewFrame();

	//���ýӿ�
public:
	//��������
	virtual bool ConfigTableFrame(word wTableCount, word wChairCount, dword dwServerRule, word wServerType, word wServerID);

	//��Ϣ�ӿ�
public:
	//������Ŀ
	virtual word GetTableCount() { return mTableCount; }
	//������Ŀ
	virtual word GetChairCount() { return mChairCount; }
	
	//�û��ӿ�
public:
	//��ȡ�û�
	virtual SHZIClientUserItem * GetClientUserItem(word wTableID, word wChairID);
	//������Ϣ
	virtual bool SetClientUserItem(word wTableID, word wChairID, SHZIClientUserItem * pIClientUserItem);

	//״̬�ӿ�
public:
	//��Ϸ��־
	virtual bool GetPlayFlag(word wTableID);
	//�����־
	virtual bool GetLockerFlag(word wTableID);
	//������
	virtual void SetFocusFrame(word wTableID, bool bFocusFrame);
	//����״̬ 
	virtual void SetTableStatus(word wTableID, bool bPlaying, bool bLocker);
	//����״̬ 
	virtual void SetTableStatus(bool bWaitDistribute);

	//��ͼ����
public:
	//���ӿ���
	virtual bool VisibleTable(word wTableID);
	//��������
	virtual bool FlashGameTable(word wTableID);
	//��������
	virtual bool FlashGameChair(word wTableID, word wChairID);

	//���ܽӿ�
public:
	//��������
	virtual bool UpdateTableView(word wTableID);
	//��ȡ����
	virtual SHZITableView * GetTableViewItem(word wTableID);
	//��������
	virtual word GetNullChairCount(word wTableID, word & wNullChairID);

	//��������
public:
	//����״̬
	void SetMatchStatus(byte cbMatchStatus);
};

//////////////////////////////////////////////////////////////////////////////////

#endif // _TableViewFrame_H_