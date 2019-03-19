#include "LanguageConfig.h"
#include "Xml/XmlManager.h"
using namespace tinyxml2;

static LanguageConfig* pGlobelLanguageConfig = NULL;

LanguageConfig::LanguageConfig()
{
}

LanguageConfig::~LanguageConfig()
{

}

LanguageConfig* LanguageConfig::getInstance()
{
	if (!pGlobelLanguageConfig)
	{
		pGlobelLanguageConfig = new LanguageConfig;
	}

	return pGlobelLanguageConfig;
}

bool LanguageConfig::init(const char *filename)
{
	tinyxml2::XMLDocument* pDoc = XmlManager::getInstance()->getXmlRootElement(filename);
	if (!pDoc)
	{
		return false;
	}

	XMLElement *pinfo = pDoc->RootElement(); 
	if (!pinfo)
	{
		cocos2d::log("****************LanguageConfig::init RootElement is NULL!s****************");

		CC_SAFE_DELETE(pDoc);
		return false;
	}

	XMLElement *pchildinfo =  pinfo->FirstChildElement("lang");
	m_mapLanguage.clear();

	string str;
	while (pchildinfo)
	{
		str = pchildinfo->Attribute("name");
		m_mapLanguage[static_cast<int>(strtol(str.data(), NULL, 0))] = pchildinfo->Attribute("value");
		pchildinfo = pchildinfo->NextSiblingElement();
	}

	CC_SAFE_DELETE(pDoc);
	return true;
}

const string& LanguageConfig::getString(int id)
{
	map<int,string>::iterator it = m_mapLanguage.find(id);
	if (it != m_mapLanguage.end())
	{
		return it->second;
	}

	return "";
}