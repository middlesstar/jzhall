#ifndef JZ_CORE_H_H
#define JZ_CORE_H_H
#include <iostream>
#include "cocos2d.h"

class JZCore
{
public:
	static JZCore* getInstance();

public:
	/// 初始化平台相关的
	void initPlatform();
public:
	/// 得到设备的唯一标示
	std::string& getDeviceID();
	/// 得到设备的名称
	std::string& getDeviceName();
	/// 手机平台显示消息
	void Toast(std::string& message);
	/// 设置deviceID
	inline void setDeviceID(std::string dId){ mszDeviceID = dId; }
	/// 设置设备的名称
	inline void setDeviceName(std::string dName){ mszDeviceName = dName; }
private:
	static JZCore*     m_pCore;
	std::string mszDeviceID;
	std::string mszDeviceName;
};
#endif
