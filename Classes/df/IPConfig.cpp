#include "IPConfig.h"
#include "cocos2d.h"

#define MAX_DEFAULT_URL 1

#define RELEASE "release"
#define RELEASE_IP "go.buyubisai.com"

#define RELEASE_TEST "release_test"
#define RELEASE_IP_TEST "42.96.146.152"

#define FANG_WEI "fangwei"
#define FANG_WEI_IP "192.168.0.16"

#define CHEN_WEI "chenwei"
#define CHEN_WEI_IP "120.26.6.159"

#define JUN_FEI "junfei"
#define JUN_FEI_IP "192.168.0.251"

const int gPort = 8300;
IPConfig* IPConfig::m_pConfig = 0;

IPConfig* IPConfig::getInstance()
{
	if (m_pConfig == 0) m_pConfig = new IPConfig;
	return m_pConfig;
}

IPConfig::IPConfig()
{
	m_gUrl[RELEASE] = RELEASE_IP;
	m_gUrl[RELEASE_TEST] = RELEASE_IP_TEST;
	m_gUrl[FANG_WEI] = FANG_WEI_IP;
	m_gUrl[CHEN_WEI] = CHEN_WEI_IP;
    m_gUrl[JUN_FEI] = JUN_FEI_IP;

    m_LogonServerNode.iPort = gPort;
    int network_index = cocos2d::UserDefault::getInstance()->getIntegerForKey(KEY_NETWORK_SELECTION, 0);
    std::string key = RELEASE;
    switch (network_index) {
        case 0:
            key = RELEASE;
            break;
        case 1:
            key = RELEASE_TEST;
            break;
        case 2:
            key = FANG_WEI;
            break;
        case 3:
            key = CHEN_WEI;
            break;
        case 4:
            key = JUN_FEI;
            break;
        default:
            break;
    }
    strncpy(m_LogonServerNode.szAddr, m_gUrl[CHEN_WEI].c_str(), LEN_SERVERADDR);
}

IPConfig::~IPConfig()
{
}

const ip_node_t &IPConfig::getLogonServerInfo() const
{
	return m_LogonServerNode;
}

void IPConfig::setLogonServerAddr(const std::string &newAddr)
{
    strncpy(m_LogonServerNode.szAddr, newAddr.c_str(), LEN_SERVERADDR);
}

void IPConfig::setLogonServerPort(int port)
{
	this->m_LogonServerNode.iPort = port;
}

void IPConfig::setRechargeAddr(const std::string &newAddr)
{
	this->m_LogonServerNode.rechargeAddr = newAddr;
}

void IPConfig::setValidateAddr(const std::string &newAddr)
{
	this->m_LogonServerNode.validateAddr = newAddr;
}

void IPConfig::setPromotionAdd(const std::string &newAddr)
{
    this->m_LogonServerNode.promotionAdd = newAddr;
}

#if INNER_TEST
const std::string &IPConfig::getLogonServerAddr(const std::string &key) const
{
	return m_gUrl.at(key);
}
#endif
