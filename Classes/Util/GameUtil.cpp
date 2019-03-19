#include "GameUtil.h"
//#include "Encrypt.h"

//定义下面的宏就会加密
//#define ENCRYPT_KEY         "RollBall"

namespace GameUtil
{
	Rect CCRectFromString(const string& strRect)		//从一个字符串中得到一个CCRect,字符串格式为(x,y,width,height)
	{
		int x = 0;
		int y = 0;
		int width = 0;
		int height = 0;

		sscanf(strRect.c_str(),"%d,%d,%d,%d",&x,&y,&width,&height);

		return Rect(x,y,width,height);
	}

	int getRandomNum(int low,int high)
	{
        int nRandomNum = low + CCRANDOM_0_1() * (high - low + 1);
        nRandomNum = MAX(nRandomNum,low);
        nRandomNum = MIN(nRandomNum,high);
        
		return nRandomNum;
	}

	void getMuiltRandomNum(int low,int high,int nCount,vector<int>& vec)
	{
		vector<int> vArray;
		for (int i = low; i <= high; i++)
		{
			vArray.push_back(i);
		}

		if (nCount >= vArray.size())
		{
			vec = vArray;
		}
		else
		{
			random_shuffle(vArray.begin(), vArray.end());

			for (int i = 0; i < nCount; i++)
			{
				vec.push_back(vArray.at(i));
			}
		}
	}

    unsigned int getMillisecond()
    {
        struct timeval now;
        gettimeofday(&now, nullptr);

        return now.tv_sec * 1000 + now.tv_usec / 1000;
    }

    string getComposeString(int* pIntArray,int nLen,char c)
    {
        string strRet;

        if (nLen > 0)
        {
            strRet = toString(pIntArray[0]);
            if (nLen > 1)
            {
                int nIndex = 1;

                while(nIndex < nLen)
                {
                    strRet += (c + toString(pIntArray[nIndex]));
                    nIndex++;
                }
            }
        }
       

        return strRet;
    }

	void getSplitData(string strSrc,vector<int>& vec,char c)
	{
		vec.clear();
		size_t pos = strSrc.find_first_of(c);
		while (pos != string::npos)
		{
			vec.push_back(toInt(strSrc.substr(0,pos)));

			strSrc = strSrc.substr(pos + 1);
			pos = strSrc.find_first_of(c);
		}
        if (strSrc.size() > 0)
        {
            vec.push_back(toInt(strSrc.substr(0,pos)));
        }

	}

	void getSplitData(string strSrc,vector<float>& vec,char c)
	{
		vec.clear();
		size_t pos = strSrc.find_first_of(c);
		while (pos != string::npos)
		{
			vec.push_back(toFloat(strSrc.substr(0,pos)));

			strSrc = strSrc.substr(pos + 1);
			pos = strSrc.find_first_of(c);
		}
        if (strSrc.size() > 0)
        {
            vec.push_back(toFloat(strSrc.substr(0,pos)));
        }
	}

    void getSplitData(string strSrc,int* pIntArray,int nLen,char c)
    {
        memset(pIntArray,0,nLen * sizeof(pIntArray[0]));

        int nIndex = 0;
        size_t pos = strSrc.find_first_of(c);
        while (pos != string::npos && nIndex < nLen)
        {
            pIntArray[nIndex++] = toInt(strSrc.substr(0,pos));

            strSrc = strSrc.substr(pos + 1);
            pos = strSrc.find_first_of(c);
        }
        if (strSrc.size() > 0 && nIndex < nLen)
        {
            pIntArray[nIndex] = toInt(strSrc.substr(0,pos));
        }
    }

	void parseCoordinateFromString(const char* pSrc,float& x,float& y)
	{
		x = 0;
		y = 0;

		if (pSrc)
		{
			sscanf(pSrc,"%f,%f",&x,&y);
		}
	}

	int toInt(const string &value)
	{
		return static_cast<int>(strtol(value.data(), NULL, 0));
	}

	int toInt(const char* pText)
	{
		return toInt(string(pText));
	}

	float toFloat(const string &value)
	{
		return static_cast<float>(atof(value.data()));
	}

	float toFloat(const char* pText)
	{
		return toFloat(string(pText));
	}

	long long toLongLong(const string &value)
	{
		return static_cast<long long>(atoll(value.data()));
	}

	long long toLongLong(const char* pText)
	{
		return toLongLong(string(pText));
	}

	void setData(const char* pKey, const std::string & value)
	{
        string strEncrypt = value;
#ifdef ENCRYPT_KEY
        //需要加密的话就在这里加密
        strEncrypt = GameEncrypt::Des3Encrypt::des_base64_encrypt(value.c_str(),ENCRYPT_KEY);
#endif

		UserDefault::getInstance()->setStringForKey(pKey,strEncrypt);
	}

	void setData(const char* pKey, const char* pValue)
	{
		string strValue = pValue;

		setData(pKey,strValue);
	}

    void setData(const char* pKey, int nValue)
    {
        if (! pKey)
        {
            return;
        }

        // format the value
        char tmp[50];
        memset(tmp, 0, 50);
        sprintf(tmp, "%d", nValue);

        setData(pKey, tmp);
    }

	std::string getData(const char* pKey)
	{
		string strRet = UserDefault::getInstance()->getStringForKey(pKey);

#ifdef ENCRYPT_KEY
        //在这个地方对字符串解密
        strRet = GameEncrypt::Des3Encrypt::base64_des_decrypt(strRet.c_str(),ENCRYPT_KEY);
#endif
		return strRet;
	}

	std::string getData(const char* pKey, const std::string & defaultValue)
	{
		string strRet = getData(pKey);

		if (strRet.size() == 0)
		{
			strRet = defaultValue;
		}

		return strRet;
	}

	std::string getData(const char* pKey, const char* pDefaultValue)
	{
		string strRet = getData(pKey);

		if (strRet.size() == 0)
		{
			strRet = pDefaultValue;
		}

		return strRet;
	}

    int getData(const char* pKey,int nDefaultValue)
    {
        int nRet = nDefaultValue;
        string strRet = getData(pKey);

        if (strRet.length() > 0)
        {
            sscanf(strRet.c_str(),"%d",&nRet);
        }

        return nRet;
    }

	bool isNum(const std::string & strValue)
	{
		bool bRet = true;

		for (int i = 0; i < strValue.length(); i++)
		{
			if (isdigit(strValue.at(i)) == 0)
			{
				return false;
			}
		}

		return bRet;
	}
}