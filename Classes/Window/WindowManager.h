#ifndef  _WindowManager_H_
#define  _WindowManager_H_
#include <map>

class WindowBase;

class WindowFactory
{
public:
	virtual WindowBase* create() = 0;
};

#define DEFINE_WINDOW_FACTORY(winclass) \
class winclass##Factory : public WindowFactory \
{	\
public:	\
	virtual WindowBase* create() override	\
	{	\
		return winclass::create();	\
	}	\
};

#define REGISTER_WINDOW(winid,winclass) \
	DEFINE_WINDOW_FACTORY(winclass) \
	WindowManager::getInstance()->addWindowFactory(winid,new winclass##Factory);

class WindowManager
{
public: 
	static WindowManager* getInstance();
	~WindowManager();

	bool init();
	
	void addWindowFactory(int winid, WindowFactory* pWindowFactory);
	WindowFactory* getWindowFactory(int winid);
protected:
	WindowManager();

	static WindowManager* m_gpWindowManager;
	std::map<int, WindowFactory*> m_mapWindowFactory;
};

#endif // _WindowManager_H_

