#ifndef SHZ_GameView_H__
#define SHZ_GameView_H__

#include "cocos2d.h"
#include "shuihu/GameHeader_SHZ.h"
#include "cocos-ext.h"
#include "GameStruct_SHZ.h"
#include "df/LocalStore.h"
#include "GoldDownLayer_SHZ.h"

class DrawMain;
//������
class SHZCGameView0 : public cocos2d::CCNode
{
public:

	//����ͼƬ
	cocos2d::CCNode*				m_TypeImgeTexture;

	//���
	cocos2d::CCNode*				m_BoxNode;

	//����ͼƬ
	cocos2d::CCSprite*				m_BgImgeTexture;
	//����߿�
	cocos2d::CCSprite*				m_BorderImgeTexture;


	// ʱ�������
	cocos2d::CCProgressTimer*		m_progressBar;

	//����ͼ�н��߿�
	cocos2d::CCSprite*				m_TypeBorderImgeTexture;

	//��������
	cocos2d::CCSpriteBatchNode*		m_GunDongImgeTexture;

	//��
	cocos2d::CCSpriteBatchNode*		m_LineTexture;

	//��
	cocos2d::CCSpriteBatchNode*     m_LightTexture;


	cocos2d::CCSprite*				m_winscoreViewTexture;				//Ӯ�ֱ�־
	cocos2d::CCSprite*				m_bibeiViewTexture;					//�ȱ�
	cocos2d::CCSprite*				m_defenViewTexture;					//�÷�

	cocos2d::CCSprite*				m_tishiTexture;						//�Զ���ʾ

	cocos2d::Sprite*                    chatLabel;                      //��������򱳾�
	cocos2d::Sprite*                    showBoard;                      //������ʾ��
	cocos2d::Sprite*                    spreadBoard;                    //������ʾ��չ��

	//�ӿ�
public:

	SHZCGameSceneInterface*			m_GameSceneInterface;

	//����
public:
	/*-------�޸�bysl--------*/
	
	//�洢�齱��������
	int                            m_ExpScore;
	/*--------------------------*/

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

	//����
public:

	bool					m_bTwinkle;						//������˸

	bool					m_bPlayTypeAnim;				//��������

	//����ͼ�ṹ
	SHZTypeImgStruct           m_typeimgstruct[15];

	//������
	int                     m_typeid[15];

	bool                    m_LineType[45];

	int						m_iCurLine;						//��ǰѹ��

	int						m_iAddCount;					//��ע����

	//�Ƿ��Զ�������Ϸ
	bool                    m_IsZidong;

	//�Ƿ������Ϸ
	bool                    m_InitScreen;

	//�Ƿ��ڹ���״̬
	bool                    m_ScrollState;

	bool					m_IsYanshi;

	bool                    m_havequan[15];

	//�Ƿ�����·�
	bool                    m_CanHaveMoney;

	int                     m_ThreeMaLiCount;

	int						m_iGameViewState;			//���� 0������ 1�ȴ�С 2С����

	bool					m_bQuanpan;					//ȫ�̽�


	bool                   m_bGetScore;                 //�Ƿ��Ѿ��÷�

	bool                   m_bAllStop;
	//TODLSL
	int                     houseIndex;                     //��������

	//λ��
public:

	cocos2d::CCPoint		m_ptline[9];				//��λ��

	cocos2d::CCPoint		m_ptLight[9];				//��λ��


	//��ʱ����ʶ
public:

	int						m_iTypeTimeTag;					//����ʱ���ʶ

	int						m_iTypeScrollTag;				//������ʱ��ʶ

	int						m_iDwonScoreTag;				//Ӯ�ֶ�ʱ��ʶ

	//��ť
public:
	cocos2d::extension::ControlButton*					m_btZidong;				//�Զ�
	cocos2d::extension::ControlButton*					m_btGetScore;			//�÷�
	cocos2d::extension::ControlButton*					m_btAddScore;			//��ע
	cocos2d::extension::ControlButton*					m_btbibei;				//�ȱ�
	cocos2d::extension::ControlButton*					m_btStart;				//��ʼ
	cocos2d::extension::ControlButton*					m_btStop;				//ȫͣ
	cocos2d::extension::ControlButton*					m_btshoudong;			//�ֶ�

	cocos2d::extension::ControlButton*					m_btSend;			    //��������
	cocos2d::extension::ControlButton*					m_btUp;			        //��������
	cocos2d::extension::ControlButton*					m_btDown;			    //��������


	//����
public:


	cocos2d::LabelAtlas					*m_lbScore2;						//���������ʾ
	cocos2d::LabelAtlas					*m_lbScore5;						//���������ʾ

	cocos2d::CCLabelBMFont				*m_lbwinScore;						//Ӯ��

public:
	/*---------------�޸�bysl--------------*/
	//�齱����������
	cocos2d::Sprite                     *bg;
	//��ʾ���Գ齱
	cocos2d::Sprite                     *tiShi;

	cocos2d::Sprite                     *boxLight;
	cocos2d::extension::ControlButton*					m_btPool;			    //�齱
	cocos2d::LabelAtlas                   *m_lbaward;                         //���ػ���label
	cocos2d::LabelTTF                  *m_lbshow;                         //���ػ��ְٷֱ���ʾ

	//�콱������
	cocos2d::CCProgressTimer*       s_progressBar;


	//�齱��
	DrawMain* lotteryLayer;

	//�жϱ����Ƿ�������
	bool flag;
	/*------------------------------------*/

	


public:
	SHZCGameView0(void);
	~SHZCGameView0(void);

	virtual bool init();
	void IsDisableView();
	static SHZCGameView0* create();

	//������Ķ���
	void dragonRun();

	//������Ч
	void playEff();

	//������ť
	void  CreatedGameBt();

	/*-----------�޸�bysl-----------*/
	//�жϳ齱�ȼ�
	int getDegreeType();
	//��¼��һ�ȡ�ľ���
	void setLotteryExp(int myExp);

	//��¼��Ҹ��µľ���
	void updateLotteryExp();

	//�ж���Ҿ���ֵ�Ƿ���Գ齱
	void arriveLottery();

	//����������
	void CreateChatRoom();
	//����������
	void createProgress();

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
	void SetGameSceneInterface(SHZCGameSceneInterface* scene){ m_GameSceneInterface = scene; };

	//���벻��ʱ�ĳ�ֵ��ʾ
	void rechargeNotice(Node* node);

	//�̳к���
private:
	void onBtnClick(cocos2d::CCObject* obj, cocos2d::extension::Control::EventType e);


	// �Ƴ�������Ķ����ص�
	void removeCallback(cocos2d::CCNode *obj);
	//���ܺ���
public:

	void ChecKLine();  //������ߣ��Ƿ�÷�

	void TypeFlash();

	//�����Լ��Ϸ�
	void SetCreditScoreScore(int myscore);

	//���¹������ȡ�ķ����Լ���������
	void ScrollInformation(int winscore, int index, int imgtypeid, bool bquanpan);

	//������ע
	void SetXiaZhu(int allcellscore);


	//����ѹ��
	void SetYaLine(int cline);

	//���ù���
	void SetScrollState(bool bScroll);
	

	//����15������ͼ
	void SetTypeImg(int iPos);

	//����15������ͼ��
	void SetTypeImgBack(int iPos);

	//����15������ͼӮ
	void SetTypeImgWin(int iPos);


	//���Ͷ�������
	void ShowTypeAnim1(cocos2d::CCNode* node, void* data);

	//���Ͷ�������
	void ShowTypeAnim2(cocos2d::CCNode* node, void* data);

	//���ö�������
	void SetTypeAnim(int typeIndex);

	//���·�
	void AddOrRemoveScore(bool addorremove);

	//�÷�
	void GetWinScore(int gettype);

	//��ȡӮ��
	void GetWinScore(int winscore, int gametype);

	//�ȱ�
	void GetBiBeiData();

	//��ע
	void ChangeCellScore();

	//�л���������
	void ChangeSceneAnim();

	//������������
	void ChangeScenAnimFinish(cocos2d::CCNode* node, void* data);

	//��ʾ����
	void ShowView(int iCreditScore);

	//��ʾ��
	void ShowLine();

	//��ʾ��
	void ShowLight();

	//��ʾӮ����
	void SetWinLine();

	//������Ϸ�Ի���
	void DiableGamePopup();



	//��ʱ��
public:

	//��ʱ����Ϣ
	void OnTimer(int nIDEvent);

	//��˸
	void OnTwinkleTimer(float dt);

	//��ʾȫͣ��ť
	void OnShowStopBt(float dt);

	//��ʱ��
	void On1000Timer(float dt);
	void On1001Timer(float dt);
	void On1002Timer(float dt);

	//�ȱ��÷�
	void OnbibeiordefenTimer(float dt);

	//���͹���
	void OnTypeScrollStopTimer(float dt);

	//��ȡӮ��
	void OnDwonScoreTimer(float dt);

	//��ϸ������Ķ�����˸
	void OnTypeImgStructTimer(float dt);

	//����
public:

	//��������(ѭ�����Ŷ���)
	void ForeverAction(cocos2d::CCNode* pNode, const char *strName, int iCount, cocos2d::CCPoint pt);

	//��������(ѭ�����Ŷ���)
	void ForeverAction(cocos2d::CCSprite* pSprite, const char *strName, int iCount, int iFirst, int ilayer);

	//��������(���Ŷ���һ��)
	void SequenceAction(const char *strImage, cocos2d::SEL_CallFuncND selector, int iCount, cocos2d::CCPoint ptImage, int iTag);
	///////////////////////////////////////////////////////////////////////
	// UIUpgradeSink�ص�
public:
	virtual void setCreditScore(int iCreditScore);

public:
	SHZGoldDownLayer* m_goldDownLayer;
private:
	void ShowGoldDownEffect();
};


#endif
