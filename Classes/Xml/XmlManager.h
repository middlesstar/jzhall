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
	tinyxml2::XMLDocument* getXmlRootElement(const string& strXmlFile);			//获取xml文件的根节点,调用此方法后自行释放返回的pDoc
};

#endif // __XmlManager_H__
