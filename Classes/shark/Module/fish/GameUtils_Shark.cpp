#include "GameUtils_Shark.h"

cocos2d::Size GameUtils_Shark::winSize;

std::vector<std::string> GameUtils_Shark::splitStr(std::string str, std::string pattern)
{
	std::string::size_type pos;
	std::vector<std::string> result;
	str += pattern;
	std::string::size_type size = str.size();

	for (std::string::size_type i = 0; i < size; i++) {
		pos = str.find(pattern, i);
		if (pos < size) {
			std::string s = str.substr(i, pos - i);
			result.push_back(s);
			i = pos + pattern.size() - 1;
		}
	}
	return result;
}

void GameUtils_Shark::DealWithRectString(std::string strTemp, cocos2d::Rect &rect)
{
	/************************************************************************/
	// ����Χ�ַ���
	// 1. �ж��ַ����ĸ�ʽ�Ƿ���ȷ
	int nTemp = static_cast<int>(count(strTemp.begin(), strTemp.end(), ','));
	if (nTemp != 3)
		return;

	// 2. �滻�ַ����е�"(" , ")" , ","Ϊ�յ��ַ�
	replace(strTemp.begin(), strTemp.end(), '(', ' ');
	replace(strTemp.begin(), strTemp.end(), ')', ' ');
	replace(strTemp.begin(), strTemp.end(), ',', ' ');

	// 3. �������ַ����������Rect��ȥ
	float fTemp[4] = { 0 };
	std::istringstream stream(strTemp);
	for (int nIndex = 0; nIndex < 4; ++nIndex)
	{
		stream >> fTemp[nIndex];
	}

	rect = cocos2d::Rect(fTemp[0], fTemp[1], fTemp[2], fTemp[3]);
}

/**
* ��ȡָ�����ȵ����������
*
* @param resVec std::vector<int> & ���������
* @param arrayLen int ��������
*/
void GameUtils_Shark::getRandArray(std::vector<int> &resVec, int arrayLen) {
	resVec.clear();
	for (int i = 0; i < arrayLen; i++) {
		resVec.push_back(i + 1);
	}
	int temp1, temp2;
	for (int i = 0; i < arrayLen / 2; i++) {
		temp1 = resVec[i];
		temp2 = abs(rand()) % arrayLen;
		resVec[i] = resVec[temp2];
		resVec[temp2] = temp1;
	}
}
