#include "libs/curl/coHttp.h"
//#include "pthread.h"
#include <thread>
#include "Utils_SHZ.h"
#include "MTNotification_SHZ.h"
#include "df/Timer.h"
#include "Utility_SHZ.h"
#include "libs/libs.h"
#include "tinyxml2/tinyxml2.h"

using namespace std;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#define XML_NAME				"shared_prefs/Cocos2dxPrefsFile.xml"
#endif


void* thHttpPost_SHZ(void* p)
{
//	SHttpRequest_SHZ* data = (SHttpRequest_SHZ*)p;
//	std::string rep;
//	int ret = coHttpPost(data->url.c_str(), data->post.c_str(), rep);
//	if (!data->notify.empty())
//		G_NOTIFICATION_SHZ(data->notify.c_str(), SHZMTData::create(ret, (unsigned int)data->userdata, 0, rep));
//	if (data->callback)
//		data->callback(ret, rep, data->userdata);
//
//	delete data;

	//pthread_detach(pthread_self());
	return 0;
}

void doHttpPost_SHZ(const SHttpRequest_SHZ& request)
{
	SHttpRequest_SHZ* data = new SHttpRequest_SHZ();
	data->url		= request.url;
	data->post		= request.post;
	data->notify		= request.notify;
	data->callback	= request.callback;
	data->userdata	= request.userdata;

	//pthread_t pid;
	//if (pthread_create(&pid, 0, thHttpPost, data) != 0)
	//{
	//	delete data;
	//}

	thread t(thHttpPost_SHZ, data);
	t.detach();
}


//////////////////////////////////////////////////////////////////////////
// 随机
//////////////////////////////////////////////////////////////////////////
int RandomArea_SHZ(int nLen, ... )
{
	using namespace std;

	if ( nLen <= 0 )
		return 0;

	va_list argptr;
	va_start(argptr, nLen);
	vector<int> Array;
	int nIndex = 0;
	int nACTotal = 0;

	for ( int i = 0; i < nLen; ++i )
	{
		int nTemp = va_arg( argptr, int );
		Array.push_back(nTemp);
		nACTotal += nTemp;
	}

	va_end(argptr);

	if ( nACTotal > 0 )
	{
		static int nRandAuxiliary = 0;
		nRandAuxiliary += rand()%10;
		int nRandNum = xlbe_utility::random_int(1, nACTotal);//((int)(rand() + CoTimer::getCurrentTime() + nRandAuxiliary)) % nACTotal;
		for (int i = 0, l = (int)Array.size(); i < l; i++)
		{
			nRandNum -= Array[i];
			if (nRandNum < 0)
			{
				nIndex = i;
				break;
			}
		}
	}
	else
	{
		nIndex = rand()%nLen;
	}

	return nIndex;
}

std::string strrpl_SHZ(const std::string& src, const std::string& sMatch, const std::string& sReplace)
{ 
	int pos = 0;
	std::string sRet;
	
	do 
	{
		int i = src.find(sMatch, pos);

		if (i == -1)
		{
			sRet.append(src.substr(pos));
			break;
		}

		sRet.append(src.substr(pos, i - pos));
		sRet.append(sReplace);
		pos = i+sMatch.length();
	} while (true);
	
	return sRet;
}

bool translateXML_SHZ(const char* oldKey, const char* newKey)
{
	do {
		tinyxml2::XMLElement* curNode = NULL;
		tinyxml2::XMLDocument* xmlDoc = new tinyxml2::XMLDocument;
		std::string xmlFilePath = CCUserDefault::sharedUserDefault()->getXMLFilePath();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
		int pos = xmlFilePath.rfind('/');
		xmlFilePath = xmlFilePath.substr(0, pos + 1) + XML_NAME;
#endif
		ssize_t nSize;
		const char* pXmlBuffer = (const char*)CCFileUtils::sharedFileUtils()->getFileData(xmlFilePath.c_str(), "rb", &nSize);
		if (NULL == pXmlBuffer) {
			CCLOG("can not read xml file");
			break;
		}

		xmlDoc->Parse(pXmlBuffer, nSize);
		delete[] pXmlBuffer;

		// 获取根节点
		curNode = xmlDoc->RootElement();
		if (NULL == curNode) {
			CCLOG("read root node error");
			break;
		}

		bool bException = false;
		curNode = curNode->FirstChildElement();
		while (NULL != curNode) {

			std::string decodeValue;
			try {
				// 解密
				coAesSetKey(oldKey);
				const char* pOldEcodeValue = curNode->FirstChild()->Value();
				decodeValue = decode(pOldEcodeValue);
				CCLOG("%s", decodeValue.c_str());
			} catch (...) {
				bException = true;
				CCLOG("Exception");
			}

			// 加密
			coAesSetKey(newKey);
			curNode->FirstChild()->SetValue(encode(decodeValue).c_str());
			curNode = curNode->NextSiblingElement();
		}

		xmlDoc->SaveFile(xmlFilePath.c_str());
		delete xmlDoc;

		CCLOG("translateXML: return true");
		return true;
	} while (0);

	CCLOG("translateXML: return false");
	return false;
}