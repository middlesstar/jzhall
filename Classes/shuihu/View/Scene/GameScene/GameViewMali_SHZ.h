#ifndef SHZ_GameViewMail_H__
#define SHZ_GameViewMail_H__

#include "cocos2d.h"
#include "shuihu/GameHeader_SHZ.h"
#include "cocos-ext.h"
#include "GameStruct_SHZ.h"
#include "GoldDownLayer_SHZ.h"

class SHZCGameViewMali :
	public cocos2d::CCNode
{

	//��Դ
public:

	//��������
	cocos2d::CCSpriteBatchNode*		m_GunDongImgeTexture;

	//����ͼƬ
	cocos2d::CCSprite*				m_BgImgeTexture;

	cocos2d::CCSprite*				m_TypeImageTexture;		//

	cocos2d::CCNode*				m_TypeNode;

	cocos2d::CCSprite*				m_winscoreViewTexture;				//Ӯ�ֱ�־
	cocos2d::CCSprite*				m_bibeiViewTexture;					//�ȱ�
	cocos2d::CCSprite*				m_defenViewTexture;					//�÷�

	bool							m_bNext;				//

	bool							m_bRoll;				//����

	//�ӿ�
public:

	SHZCGameSceneInterface*			m_GameSceneInterface;

	//����
public:

	int                     m_ThreeMaLiCount;
	//
	int                     m_ServerPai;
	//
	int                     m_ThreeInt[4];
	//
	SHZTypeImgStruct           m_typeimgstructthree[4];

	int						m_typeid[4];			//����ͼƬ


	int						m_izhuanQuanID;			//תȦ��ʶ

	int                     m_CurrThreePai;

	int                     m_CurrQuanCount;
	//
	bool                    m_ThreeShan;

	int                     m_ThisScore;

	//ÿ�ο����ƺ�Ļ�÷���
	int                     ThisScore;

	//��Ԫ����
	int                     m_CellScore;

	//����ϵķ�
	int                     m_CreditScore;

	int                     m_TempDwonscore;

	int                     m_TempCCscore;
	//
	int                     m_TempTTscore;

	int                     m_MaLiSquenceCount;

	bool					m_bzidong;

	cocos2d::CCPoint		m_ptTypeImage[24];

	//��ť
public:

	cocos2d::extension::ControlButton*					m_btGetScore;			//�÷�
	cocos2d::extension::ControlButton*					m_btbibei;				//�ȱ�
	//cocos2d::extension::ControlButton*					m_btshare;				//����

	//����
public:

	cocos2d::CCLabelTTF						*m_lbScore1;						//���������ʾ
	cocos2d::CCLabelTTF						*m_lbScore2;						//���������ʾ
	cocos2d::CCLabelTTF						*m_lbScore3;						//���������ʾ
	cocos2d::CCLabelTTF						*m_lbScore4;						//���������ʾ

	cocos2d::CCLabelBMFont					*m_lbwinScore;						//Ӯ��

public:
	SHZCGameViewMali(void);
	~SHZCGameViewMali(void);


	bool init();

	static SHZCGameViewMali* create();

	//������ť
	void  CreatedGameBt();

	void SetGameSceneInterface(SHZCGameSceneInterface* scene){ m_GameSceneInterface = scene; };

	//�̳к���
private:
	void onBtnClick(cocos2d::CCObject* obj, cocos2d::extension::Control::EventType e);

public:



	//���ܺ���
public:

	//�÷�
	void GetWinScore(int gettype);

	//��ȡӮ��
	void GetWinScore(int winscore, int gametype);

	//С������Ϣ
	void SetMaLiInformation(int winscore, int index, int imgtypeid, int gamecount, int tyepindex, bool nowbegin);

	//��ʼС����
	void StartMali(int icreditscore, int ibibeiScore, int icellscore, bool bzidong);

	//����������Ϣ
	void MaLiSendMessage();

	//���ù���
	void SetScrollState(bool bScroll);

	//����4������ͼ
	void SetTypeImg(int iPos);

	//���Ͷ�������
	void ShowTypeAnim(cocos2d::CCNode* node, void* data);

	//��������
	void TypescalingAnim(cocos2d::CCNode* node, void* data);

	//���õ÷ֶ�����ʾ
	void SetGetScore(int iScore, int idefen);

	//�÷ֶ������
	void GetScoreFinsh();

	//����
public:

	//��������(���Ŷ���һ��)
	void SequenceAction(const char *strImage, cocos2d::SEL_CallFuncND selector, int iCount, cocos2d::CCPoint ptImage);

	//��ʱ��
public:

	void OnZhuanQuanTimer(float dt);

	//��ȡӮ��
	void OnDwonScoreTimer(float dt);

	//�ȱ��÷�
	void OnbibeiordefenTimer(float dt);


public:
	SHZGoldDownLayer* m_goldDownLayer;

private:
	void ShowGoldDownEffect();
	void TouchStateCheck(float dt);
};

#endif

