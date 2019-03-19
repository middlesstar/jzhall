#ifndef  _CycleUnit_H_
#define  _CycleUnit_H_

//#include "cocos2d.h"
#include "ui/CocosGUI.h"

//USING_NS_CC;
using namespace cocos2d::ui;

struct sUnitInfo
{
	sUnitInfo() {};
	sUnitInfo(const std::string imgPath, const std::string effectPath, int nSceneId)
		: strImgPath(imgPath)
		, strEffectPath(effectPath)
		, m_nSceneId(nSceneId)
	{

	};

	std::string strImgPath;
	std::string strEffectPath;
	int m_nSceneId;
};

class  CycleUnit : public ImageView
{
public:
    CycleUnit();
    virtual ~CycleUnit();
	static CycleUnit* create(const sUnitInfo& info);

	bool init(const sUnitInfo& info);
	void load(const sUnitInfo& info);

	void playEffect(bool bPlaySoundEffect = false);
	void stopEffect();

protected:
	Node* m_pEffectNode;
};

#endif // _CycleUnit_H_

