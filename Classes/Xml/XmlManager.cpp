#include "XmlManager.h"
using namespace tinyxml2;

static XmlManager* pGlobelXmlManager = NULL;

XmlManager::XmlManager()
{

}

XmlManager::~XmlManager()
{

}

XmlManager* XmlManager::getInstance()
{
	if (!pGlobelXmlManager)
	{
		pGlobelXmlManager = new XmlManager;
	}

	return pGlobelXmlManager;
}

tinyxml2::XMLDocument*	XmlManager::getXmlRootElement(const string& strXmlFile)
{
    if (strXmlFile.empty())
    {
        return NULL;
    }

	tinyxml2::XMLDocument* pDoc = new tinyxml2::XMLDocument;

    Data filedata = FileUtils::getInstance()->getDataFromFile(strXmlFile);
	tinyxml2::XMLError error = pDoc->Parse((const char*)filedata.getBytes(),filedata.getSize());

	if (error != tinyxml2::XML_NO_ERROR)
	{
		cocos2d::log("****************XmlManager::getXmlRootElement [%s] ERROR****************",strXmlFile.c_str());

		CC_SAFE_DELETE(pDoc);
		return NULL;
	}

	return pDoc;
}