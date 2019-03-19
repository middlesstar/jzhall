#ifndef SHZ_GetHouseIndex_H_
#define SHZ_GetHouseIndex_H_
class SHZGetHouseIndex
{
private:
	SHZGetHouseIndex(){ flag = false; rechargeFlag = false; }
	static SHZGetHouseIndex *m_Instance;

public:
	~SHZGetHouseIndex()
	{
		if (m_Instance != nullptr)
			delete m_Instance;
	}
	static SHZGetHouseIndex* getInstance();


	void setIndex(int n);
	int getIndex();
	//�齱��Ľ����
	void setJinBi(int n);
	int getJinBi();

	//�鵽�Ľ����
	void setLotteryScore(int n);
	int getLotteryScore();
	//�鵽��Ԫ����
	void setLotteryBao(int n);
	int getlotteryBao();

	//�齱��ľ�����
	void setExp(int n);
	int getExp();

	/*--------------todosl---------------*/
	//��¼���������ʾ��ȡ��
	void setFlag(bool select);
	bool getFlag();
	/*---------------------------------*/

	/*---------------�жϽ���ѡ��������Ƿ񵯳���ֵ--------------*/
	void setRechargeFlag(bool recharge){ rechargeFlag = recharge; };
	bool getRechargeFlag(){ return rechargeFlag; };

private:
	int index;
	int yuanbao;
	int jinbi;
	int exp;

	//��¼���������ʾ��ȡ��
	bool flag;
	//��¼�Ƿ�Ҫ������ֵ����
	bool rechargeFlag;

	//�齱�õ���Ԫ���ͽ��
	int  lotteryCoin;
	int  lotteryBao;
	
};


#endif // _GetHouseIndex_H_