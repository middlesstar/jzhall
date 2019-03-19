#ifndef IP_CONFIG_H_H
#define IP_CONFIG_H_H
#include <vector>
#include <string>
#include "Define.h"
#include <unordered_map>
//#include "consts/GameConsts.h"

#define KEY_NETWORK_SELECTION "key_network_selection"

////保存的IP节点
typedef struct ip_node_t
{
	int     iPort;
	char    szAddr[LEN_SERVERADDR];
	std::string  rechargeAddr;
	std::string  validateAddr;
	std::string  promotionAdd;
}ip_node_t;

class IPConfig
{
public:
	static IPConfig* getInstance();
    
private:
	 IPConfig();
	~IPConfig();

public:
	const ip_node_t &getLogonServerInfo() const;
	void setLogonServerAddr(const std::string &newAddr);
	void setLogonServerPort(int port);
	void setRechargeAddr(const std::string &newAddr);
	void setValidateAddr(const std::string &newAddr);
	void setPromotionAdd(const std::string &newAddr);
	void purge();
#if INNER_TEST
	const std::string &getLogonServerAddr(const std::string &key) const;
#endif
private:
	static IPConfig*    m_pConfig;
    ip_node_t           m_LogonServerNode;
	std::unordered_map<std::string, std::string> m_gUrl;
};
#endif
