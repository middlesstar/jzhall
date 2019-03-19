#ifndef  _ColorResult_H_
#define  _ColorResult_H_

#include "cocos2d.h"
#include "shark/Base/ViewBase.h"


class  ColorResult : public ViewBase
{
public:
	ColorResult();
	virtual ~ColorResult();
	CREATE_FUNC(ColorResult);

	virtual void setContent();

	//������˸
	void startPlay();
	//�ر���˸
	void stopPlay();

	
public:
	//�жϿ�������
	void judgeResult(int index);
	//�����ƹ���ת
	void rotateLight();

	//��ʾ�н�������
	void showType(Node* sp);

	//ֹͣ������Ȧ����ת
	void stopRotate();
	
	//������Ч��ʾ�ʽ���Ϯ
	void cardLight();

public:
	//��ǹЧ��
	void startShoot();
	//������ɫ��ǹЧ��
	void startColor();
	
protected:
	int    count;  //��¼�����Ĺ�Ч�ĸ���
	std::vector<Sprite*>   leftLight;
	
protected:	
	Node*        pMainNode;
	Sprite*      light;
	Sprite*      blinkLight;
	Sprite*      yuanbao;
	Sprite*      tickets;
	Node*      bg_caijin;
	Sprite*      caijin_box;
	Sprite*      caijinCome;

	bool         lightFlag;

protected:
	ImageView*			m_light;
	Text*				m_name[3];
	TextAtlas*			m_num[3];
	ImageView*			m_gold[3];
	Text*				m_cjlxText;

	//cocostudio::timeline::ActionTimeline* uiAction;


	bool				m_bIsOpened;

public:
	void ListShow();
	void ListHide();
protected:
	void CloseCallBack();
};

#endif // _ColorResult_H_

