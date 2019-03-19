#ifndef __MacroDefineCommon_H__
#define __MacroDefineCommon_H__

#define JZ_PRINTF cocos2d::log

//¶¨Òåµ¥Àýºê
#ifndef SINGLE_INSTANCE_FUNC
#define SINGLE_INSTANCE_FUNC(_CLASS_) \
static _CLASS_* getInstance() \
{ \
	static _CLASS_ g##_CLASS_##Object; \
	return &g##_CLASS_##Object; \
}
#endif //SINGLE_INSTANCE_FUNC

#define HALL_NOTIFICATION(name, obj)					NotificationCenter::sharedNotificationCenter()->postNotification(name, obj)
#define HALL_NOTIFICATION_REG(name, callfuncO, obj)		NotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(callfuncO), name, obj)
#define HALL_NOTIFICATION_UNREG(name)					NotificationCenter::sharedNotificationCenter()->removeObserver(this, name)

#endif
