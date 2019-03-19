#ifndef __GameUtil_H__
#define __GameUtil_H__

#include "cocos2d.h"
USING_NS_CC;
using namespace std;

#define ARRAY_LEN(array_name) ((sizeof(array_name))/(sizeof(array_name[0])))

/* -------------������------------- */
namespace GameUtil
{
	Rect CCRectFromString(const string& strRect);		//��һ���ַ����еõ�һ��Rect,�ַ�����ʽΪ(x,y,width,height)

	int getRandomNum(int low,int high);				//��ȡ[low,high]�����һ�������
	void getMuiltRandomNum(int low,int high,int nCount,vector<int>& vec);		//��ȡ[low,high]�����nCount����������������vec����

    unsigned int getMillisecond();      //��ȡ���ڵĺ�����

    string getComposeString(int* pIntArray,int nLen,char c);        //������pIntArray��Ԫ�ؼ���ָ�������ַ���

	//���ݷָ��c��pSrc��ȡ������,Eg:n|n|n|n,���к�vector�н��Ϊnnnn
	void getSplitData(string strSrc,vector<int>& vec,char c);
	void getSplitData(string strSrc,vector<float>& vec,char c);
    void getSplitData(string strSrc,int* pIntArray,int nLen,char c);

	void parseCoordinateFromString(const char* pSrc,float& x,float& y);		//��pSrc��ȡ������(pSrc��ʽΪ[%f,%f])

	int toInt(const string &value);
	int toInt(const char* pText);

	float toFloat(const string &value);
	float toFloat(const char* pText);

	long long toLongLong(const string &value);
	long long toLongLong(const char* pText);

	/* ��T��ת����string�� */
	template<typename T>
	string toString(T value)
	{
		stringstream stream;
		stream << value;
		return stream.str();
	}


	//�洢��Ϸ����
	void setData(const char* pKey, const std::string & value);
	void setData(const char* pKey, const char* pValue);
    void setData(const char* pKey, int nValue);

	std::string getData(const char* pKey);
	std::string getData(const char* pKey, const std::string & defaultValue);
	std::string getData(const char* pKey, const char* pDefaultValue);
    int getData(const char* pKey,int nDefaultValue);
	
	bool isNum(const std::string & strValue);		//�ж��ַ����Ƿ�������
}

#endif // __GameUtil_H__
