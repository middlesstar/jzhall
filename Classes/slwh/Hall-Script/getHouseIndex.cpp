#include "getHouseIndex.h"

GetHouseIndex* GetHouseIndex::m_Instance = nullptr;
GetHouseIndex* GetHouseIndex::getInstance()
{
	if (m_Instance == nullptr)
		m_Instance = new GetHouseIndex();
	return m_Instance;
}
void GetHouseIndex::setIndex(int n)
{
	roomIndex = n;
}
int GetHouseIndex::getIndex()
{
	return roomIndex;
}

