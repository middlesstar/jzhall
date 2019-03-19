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
	//抽奖后的金币数
	void setJinBi(int n);
	int getJinBi();

	//抽到的金币数
	void setLotteryScore(int n);
	int getLotteryScore();
	//抽到的元宝数
	void setLotteryBao(int n);
	int getlotteryBao();

	//抽奖后的经验数
	void setExp(int n);
	int getExp();

	/*--------------todosl---------------*/
	//记录弹出框的显示与取消
	void setFlag(bool select);
	bool getFlag();
	/*---------------------------------*/

	/*---------------判断进入选房间界面是否弹出充值--------------*/
	void setRechargeFlag(bool recharge){ rechargeFlag = recharge; };
	bool getRechargeFlag(){ return rechargeFlag; };

private:
	int index;
	int yuanbao;
	int jinbi;
	int exp;

	//记录弹出框的显示与取消
	bool flag;
	//记录是否要弹出充值界面
	bool rechargeFlag;

	//抽奖得到的元宝和金币
	int  lotteryCoin;
	int  lotteryBao;
	
};


#endif // _GetHouseIndex_H_