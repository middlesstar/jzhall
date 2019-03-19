#ifndef __DrawMain_LAYER_H__
#define __DrawMain_LAYER_H__

#include "cocos2d.h"
#include "shuihu/ViewHeader_SHZ.h"

class DrawMain
	: public cocos2d::Layer
{
public:
	DrawMain();
	~DrawMain();
public:
	bool init(int type, int exp,int index);
	static DrawMain* createLayer(int type,int exp,int index);
	//���δ���
public:
	void setTouchSwallow(cocos2d::Object* psender, bool bSwallow = true);
private:
	cocos2d::EventListenerTouchOneByOne* m_pEventListener;
public:
	//ȷ��ȡ����ť�¼�
	void  onBtnClick(Ref* ref, cocos2d::extension::Control::EventType evenType);
	//�رհ�ť�¼�
	void onBtnClose(cocos2d::Object* psender);
	//�����齱��Ƭ
	void createCards();
	//�����齱�ȼ�
	void createDegree(int type, int exp);
	//�齱�ȼ��ص�
	void menuItemCallback(cocos2d::Object* psender);
    //��ʼ�齱
	void startPlay(cocos2d::Object* psender);
	//����ȼ���ʾ
	void degreeUI(cocos2d::Object* psender);

	//�����콱������
	void createProgress(int type, int exp);
	//�����������ı�
	void changeProgress();
	//�齱���ƻ��ָı�
	void changeScoreLimit();
	//�����齱��
	void lotteryLayer();
	//��ʾ�齱�ȼ�
	void showDegree();
	
private:
	cocos2d::Size  s;
	cocos2d::Vector<cocos2d::MenuItemToggle* >    menuToggle;
	cocos2d::Vector<cocos2d::LabelTTF* >          scoreLabel;
private:
	//�齱�ȼ���ʾ 
	cocos2d::Sprite*           degreeIcon;
	//�����ȡ��ʾ
	cocos2d::LabelTTF*         expCue;
	//��ǰ����
	cocos2d::LabelAtlas*         curScore;
	//�齱���Ʒ���
	cocos2d::LabelAtlas*         limitScore;
	//��ʾ�������ٷֱ�
	cocos2d::LabelAtlas*         perLabel;
	//���������
	cocos2d::ProgressTimer*    d_progressBar;
	//��¼���뷿��ȼ�
	int houseIndex;
	//��ſ�Ƭ�ϵ�����
	int num[6][6];
	//��¼�齱�ȼ���
	int     degree;
	//��¼�ս���齱����ʱ�ĳ齱�ȼ�
	int     originDegree;
	//��¼�齱�ľ���ֵ
	int     lotteryExp;
	//��¼������һ�ȼ��ķ���
	int     disScore;
	//�齱��ʾ��
	LayerColor*  showLayer;
	//��������������
	Sprite*  progressBg;

	//��ʾ�ܽ��и��ߵȼ��ĳ齱
	 Sprite*                 tipText;
	 Menu*                   bt_start;
	 //���ܵ���齱
	 Sprite*                 notDraw;
	 //Label*         tipText;
};

#endif // __DrawMain_LAYER_H__
