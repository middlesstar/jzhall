#ifndef __GameUtil_H__
#define __GameUtil_H__

#include "cocos2d.h"
USING_NS_CC;
using namespace std;

#define ARRAY_LEN(array_name) ((sizeof(array_name))/(sizeof(array_name[0])))

/* -------------工具类------------- */
namespace GameUtil
{
	Rect CCRectFromString(const string& strRect);		//从一个字符串中得到一个Rect,字符串格式为(x,y,width,height)

	int getRandomNum(int low,int high);				//获取[low,high]区间的一个随机数
	void getMuiltRandomNum(int low,int high,int nCount,vector<int>& vec);		//获取[low,high]区间的nCount个随机数，结果放在vec里面

    unsigned int getMillisecond();      //获取现在的毫秒数

    string getComposeString(int* pIntArray,int nLen,char c);        //将数组pIntArray的元素加入分隔符组成字符串

	//根据分割符c从pSrc中取出数据,Eg:n|n|n|n,运行后vector中结果为nnnn
	void getSplitData(string strSrc,vector<int>& vec,char c);
	void getSplitData(string strSrc,vector<float>& vec,char c);
    void getSplitData(string strSrc,int* pIntArray,int nLen,char c);

	void parseCoordinateFromString(const char* pSrc,float& x,float& y);		//从pSrc中取出坐标(pSrc格式为[%f,%f])

	int toInt(const string &value);
	int toInt(const char* pText);

	float toFloat(const string &value);
	float toFloat(const char* pText);

	long long toLongLong(const string &value);
	long long toLongLong(const char* pText);

	/* 将T型转换成string型 */
	template<typename T>
	string toString(T value)
	{
		stringstream stream;
		stream << value;
		return stream.str();
	}


	//存储游戏数据
	void setData(const char* pKey, const std::string & value);
	void setData(const char* pKey, const char* pValue);
    void setData(const char* pKey, int nValue);

	std::string getData(const char* pKey);
	std::string getData(const char* pKey, const std::string & defaultValue);
	std::string getData(const char* pKey, const char* pDefaultValue);
    int getData(const char* pKey,int nDefaultValue);
	
	bool isNum(const std::string & strValue);		//判断字符串是否是数字
}

#endif // __GameUtil_H__
