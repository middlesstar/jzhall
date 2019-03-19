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


//�齱��Ľ����
void SHZGetHouseIndex::setJinBi(int n)
{
	jinbi = n;
}
int SHZGetHouseIndex::getJinBi()
{
	return jinbi;
}

//�齱��ľ�����
void SHZGetHouseIndex::setExp(int n)
{
	exp = n;
}
int SHZGetHouseIndex::getExp()
{
	return exp;
}

//��¼���������ʾ��ȡ��
void SHZGetHouseIndex::setFlag(bool select)
{
	flag = select;
}
bool SHZGetHouseIndex::getFlag()
{
	return flag;
}


//�鵽�Ľ����
void SHZGetHouseIndex::setLotteryScore(int n)
{
	lotteryCoin = n;
}
int SHZGetHouseIndex::getLotteryScore()
{
	return lotteryCoin;
}
//�鵽��Ԫ����
void SHZGetHouseIndex::setLotteryBao(int n)
{
	lotteryBao = n;
}
int SHZGetHouseIndex::getlotteryBao()
{
	return lotteryBao;
}