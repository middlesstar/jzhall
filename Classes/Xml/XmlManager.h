#ifndef __XmlManager_H__
#define __XmlManager_H__
#include "cocos2d.h"
#include "tinyxml2/tinyxml2.h"
#include <string>

USING_NS_CC;
using namespace std;

class XmlManager
{
public:
	XmlManager();
	~XmlManager();

	static XmlManager*	   getInstance();
	tinyxml2::XMLDocument* getXmlRootElement(const string& strXmlFile);			//��ȡxml�ļ��ĸ��ڵ�,���ô˷����������ͷŷ��ص�pDoc
};

#endif // __XmlManager_H__
