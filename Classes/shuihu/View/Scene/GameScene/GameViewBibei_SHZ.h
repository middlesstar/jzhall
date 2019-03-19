#ifndef SHZ_GameViewBiBei_H__
#define SHZ_GameViewBiBei_H__

#include "cocos2d.h"
#include "shuihu/GameHeader_SHZ.h"
#include "cocos-ext.h"
#include "GameStruct_SHZ.h"
#include "shuihu/ViewHeader_SHZ.h"
#include "TouchLayer_SHZ.h"
class SHZCGameViewBibei : public cocos2d::CCNode, public SHZITouchSink
{

	//��Դ
public:

	//����ͼƬ
	cocos2d::CCSprite*				m_BgImgeTexture;

	cocos2d::CCNode*				m_NodeBoss;				//boss

	//cocos2d::CCNode*				m_NodeLeft;				//���

	//cocos2d::CCNode*				m_NodeRight;			//�ұ�

	cocos2d::CCSprite*				m_TaiSaiViewTexture;	//��С����

	cocos2d::CCSprite*				m_sai1Texture;			//����1
	cocos2d::CCSprite*				m_sai2Texture;			//����2

	cocos2d::CCSprite*				m_yuanbao1Texture;		//Ԫ��1
	cocos2d::CCSprite*				m_yuanbao2Texture;		//Ԫ��2

	//��¼
	cocos2d::CCSpriteBatchNode*		m_RecordTexture;		//��¼


	cocos2d::CCSprite*				m_winscoreViewTexture;				//Ӯ�ֱ�־
	cocos2d::CCSprite*				m_bibeiViewTexture;					//�ȱ�
	cocos2d::CCSprite*				m_defenViewTexture;					//�÷�
	//����
public:

	bool							m_bStartAnim;		//��ʼ����

	bool							m_bwinLostAnim;		//��Ӯ����

	//���ӵ���,����һ
	int								 m_S1;
	//
	int								 m_S2;

	//�ȱ�����
	int								 m_bibeiScore;
	//�ȱ���÷���
	int								 m_bibeiWinScore;
	//
	int								 m_bibeiWinTempScore;

	//����ϵķ�
	int								 m_CreditScore;

	//�Ƿ�����·�
	bool							 m_CanHaveMoney;

	int								 m_ixiazhuPos;		//��עλ��

	int								 m_TempDwonscore;

	int								 m_TempCCscore;
	//
	int								 m_TempTTscore;

	int								 m_iDoubleCount;	//�ӱ�����

	bool							m_bxiazhu;


	cocos2d::CCRect					m_rcdaxiao[3];			//ѹ��С����

	//�ӿ�
public:

	SHZCGameSceneInterface*			m_GameSceneInterface;

	//��ť
public:

	cocos2d::extension::ControlButton*					m_btBig;				//���
	cocos2d::extension::ControlButton*					m_btGetScore;			//�÷�
	cocos2d::extension::ControlButton*					m_btSmall;				//��ע
	//cocos2d::extension::ControlButton*					m_btbibei;				//�ȱ�
	cocos2d::extension::ControlButton*					m_bttie;				//���

	cocos2d::extension::ControlButton*					m_btaddbei;				//�ӱ�
	cocos2d::extension::ControlButton*					m_btcurbei;				//����

	//����
public:

	//cocos2d::CCLabelBMFont					*m_lbScore1;						//���������ʾ
	//cocos2d::CCLabelBMFont					*m_lbScore2;						//���������ʾ
	cocos2d::CCLabelTTF						*m_lbScore3;						//���������ʾ

	cocos2d::CCLabelBMFont					*m_lbwinScore;						//Ӯ��

public:

	SHZTouchLayer*						touch_layer_;						//���ӿ�

public:
	SHZCGameViewBibei(void);
	~SHZCGameViewBibei(void);

	bool init();

	static SHZCGameViewBibei* create();

	//������ť
	void  CreatedGameBt();

	//������Ч
	void playEff();

	//���ýӿ�
	void  SetGameSceneInterface(SHZCGameSceneInterface* face){ m_GameSceneInterface = face; }

	//��������
public:

	//��ʼ����
	void ShowStartAnim(cocos2d::CCNode* node, void* data);


	//ׯ�ҿ�������
	void ShowLotteryAnim(cocos2d::CCNode* node, void* data);

	//ׯ�ҺͶ�ͽ�����󶯻�
	void ShowWinLostAnim(cocos2d::CCNode* node, void* data);


	//�̳к���
private:
	void onBtnClick(cocos2d::CCObject* obj, cocos2d::extension::Control::EventType e);


	//���ܺ���
public:
	//����
	void touchEvent();
	//�÷�
	void GetWinScore(int gettype);

	//��ȡӮ��
	void GetWinScore(int winscore, int gametype);

	//�ȱ�
	virtual bool BiBei(int bibeitype, int bibeimax);

	//�ȱ����
	void SetBiBei(int winscore, int S1, int S2);

	//��ʼ����
	void StartAnim(int iCreditScore, int ibibeiscore);

	//�ȱ�����
	void CopyBiBeiData(int ibibeidata, int ipos);

	//�ӱ�
	void SetAddbei(int iaddScore);

	//�ȱ�Ӯ��
	void ShowBibeiwin(bool bshow);

	/*-------------todosl---------------*/
	//�������
	void createCoin();
	//�����Ч
	void createCoinFly(Node* sp, int m);
	//����winscore
	void getWinScore();

	//�Ƴ����
	void removeCoinFly(Node* obj);

	//��ҷŴ�Ч��
	void coinChange(Node* obj);

	/*-------------------------------*/

	//����
public:

	//��������(ѭ�����Ŷ���)
	void ForeverAction(CCNode* node, const char *strName, int iCount, cocos2d::CCPoint ptImage, int iFirst = 0);

	//��������(���Ŷ���һ��)
	void SequenceAction(const char *strImage, cocos2d::SEL_CallFuncND selector, int iCount, cocos2d::CCPoint ptImage, int itag);


	//��ʱ��
public:

	//�ȱ��÷�
	void OnbibeiordefenTimer(float dt);

	//��ȡӮ��
	void OnDwonScoreTimer(float dt);

	//��ʾ����
	void OnShowSaizi(float dt);

private:
	bool mbGetScore;
	cocos2d::EventListenerTouchOneByOne* listener;//���ӿ�
};


#endif
