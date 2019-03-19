#include "getHouseIndex_SHZ.h"

SHZGetHouseIndex* SHZGetHouseIndex::m_Instance = nullptr;
SHZGetHouseIndex* SHZGetHouseIndex::getInstance()
{
	if (m_Instance == nullptr)
		m_Instance = new SHZGetHouseIndex();
	return m_Instance;
}
void SHZGetHouseIndex::setIndex(int n)
{
	index = n;
}
int SHZGetHouseIndex::getIndex()
{
	return index;
}


//抽奖后的金币数
void SHZGetHouseIndex::setJinBi(int n)
{
	jinbi = n;
}
int SHZGetHouseIndex::getJinBi()
{
	return jinbi;
}

//抽奖后的经验数
void SHZGetHouseIndex::setExp(int n)
{
	exp = n;
}
int SHZGetHouseIndex::getExp()
{
	return exp;
}

//记录弹出框的显示与取消
void SHZGetHouseIndex::setFlag(bool select)
{
	flag = select;
}
bool SHZGetHouseIndex::getFlag()
{
	return flag;
}


//抽到的金币数
void SHZGetHouseIndex::setLotteryScore(int n)
{
	lotteryCoin = n;
}
int SHZGetHouseIndex::getLotteryScore()
{
	return lotteryCoin;
}
//抽到的元宝数
void SHZGetHouseIndex::setLotteryBao(int n)
{
	lotteryBao = n;
}
int SHZGetHouseIndex::getlotteryBao()
{
	return lotteryBao;
}