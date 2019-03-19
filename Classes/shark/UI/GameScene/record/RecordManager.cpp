#include "RecordManager.h"

RecordManager::RecordManager() 
 : pMainNode(nullptr)
 , newNum(0)
{

}

RecordManager::~RecordManager()
{
}

void RecordManager::setContent()
{
	pMainNode = dynamic_cast<Node*>(_seekNodeByName(m_pNode, "resultShow"));
	CC_ASSERT(pMainNode != NULL);

	char str[20];
	for (size_t i = 0; i < 12; i++)
	{
		sprintf(str, "%d", i);
		Sprite* board = dynamic_cast<Sprite*>(_seekNodeByName(m_pNode, str));
		m_oneofRecord[i].dependNode(board);
	}
}


void RecordManager::judgePos(int pos)
{
	if (pos == 0 || pos == 1)
	{
		newNum = 9;
	}

	else if (pos == 3 || pos == 4)
	{
		newNum = 10;
	}
	else if (pos == 6 || pos == 7 || pos == 8)
	{
		newNum = 11;
	}
	else if (pos == 10 || pos == 11 || pos == 12)
	{
		newNum = 4;
	}
	else if (pos == 14 || pos == 15)
	{
		newNum = 5;
	}
	else if (pos == 17 || pos == 18)
	{
		newNum = 6;
	}
	else if (pos == 20 || pos == 21 || pos == 22)
	{
		newNum = 7;
	}
	else if (pos == 24 || pos == 25 || pos == 26)
	{
		newNum = 8;
	}
	else if (pos == 2 || pos == 9 || pos == 16 || pos == 23)
	{
		newNum = 1;
	}
	else if (pos == 5 || pos == 13 || pos == 19 || pos == 27)
	{
		newNum = 2;
	}
}

void RecordManager::updateBoard(int code)
{
	log("-------------updateBoard---------------");
	auto gameInfo = GameInfo::GetInstance();

	
	int *index = gameInfo->getResRecord();
	

	int newIndex[12];
	if (1 == code)
	{
		for (size_t i = 0; i < 12; i++)
		{
			int n = index[i];
			m_oneofRecord[i].setImage(n);
			if (i == 0)
			{
				//设置转动开始的位置
				int stopIndex = gameInfo->getcircleFlag();
				stopIndex = n;
				gameInfo->setcircleFlag(stopIndex);
			}
		}
		
	}
	
	else
	{
		//设置newNum值
		judgePos(gameInfo->getopenRelt().resultIndex[0]);

		//记录转动停止的第一个位置
		int stopIndex = gameInfo->getcircleFlag();
		stopIndex = gameInfo->getopenRelt().resultIndex[0];
		gameInfo->setcircleFlag(stopIndex);

		for (size_t i = 0; i < 12; i++)
		{
			if (0 == i)
			{
				newIndex[i] = newNum;
			}
			else
			{
				newIndex[i] = index[i - 1];
			}

			int n = newIndex[i];
			m_oneofRecord[i].setImage(n);
		}

		gameInfo->setResRecord(newIndex);
	}

}

