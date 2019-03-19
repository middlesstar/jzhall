#ifndef SHZ_GameClientView_H__
#define SHZ_GameClientView_H__

#include "cocos2d.h"
#include "shuihu/GameHeader_SHZ.h"
#include "TouchLayer_SHZ.h"
#include "df/Timer.h"
#include "cocos-ext.h"
#include "GameStruct_SHZ.h"
#include "GameView0_SHZ.h"
#include "GameViewBibei_SHZ.h"
#include "GameViewMali_SHZ.h"


//���붨��
#define JETTON_COUNT_SHZ				7									//������Ŀ
#define JETTON_RADII_SHZ				55									//����뾶

//��Ϣ����
#define IDM_PLACE_JETTON_SHZ			WM_USER+200							//��ס��Ϣ
#define IDM_APPLY_BANKER_SHZ			WM_USER+201							//������Ϣ
#define IDM_CONTINUE_CARD_SHZ			WM_USER+202							//��������
#define IDM_AUTO_OPEN_CARD_SHZ			WM_USER+203							//��ť��ʶ
#define IDM_OPEN_CARD_SHZ				WM_USER+204		

//��������
#define INDEX_PLAYER_SHZ				0									//�м�����
#define INDEX_BANKER_SHZ				1									//ׯ������

#define MAX_APPLY_DISPLAY_SHZ			6									//���������ʾ����

//��ʷ��¼
#define MAX_SCORE_HISTORY_SHZ			50									//��ʷ����
#define MAX_FALG_COUNT_SHZ				32									//��ʶ����

//��������
//typedef CWHArray<tagJettonInfo,tagJettonInfo&> CJettonInfoArray;
class SHZGameTest : public cocos2d::CCLayer
{
public:

	CREATE_FUNC(SHZGameTest);

	virtual bool init();

	//������Androidƽ̨�µķ��ؼ�����
	//virtual void keyBackClicked();
	virtual void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event);

	inline void setViewRef(SHZCGameView0* pGameView0Ref, SHZCGameViewMali* pGameViewMaliRef) {
		m_pGameView0Ref = pGameView0Ref;
		m_pGameViewMaliRef = pGameViewMaliRef;
	}

	void onCloseCallback(cocos2d::CCNode *pNode);

	//��ֵ��ť����ʱ������
	void comeToHall();
private:
	SHZCGameView0* m_pGameView0Ref;
	SHZCGameViewMali* m_pGameViewMaliRef;
};


class SHZCGameClientView : public cocos2d::CCScene
	, public SHZCGameSceneInterface
	, public SHZITouchSink
{

public:
	//�Ƿ��Զ�������Ϸ
	bool                           m_IsZidong;

	//��Ϸģʽ,Ϊ0ʱ��15�ߣ�1ʱ�Ǳȱ���2ʱ�ǹ�����
	int                            m_GameState;

	//�ϵĵ�Ԫ����
	int                     m_CellScore;

	int						m_iDuihuan;						//�һ�����

	//����ϵķ�
	int                     m_CreditScore;

	bool					m_bPlayTypeAnim;				//��������


	int						m_iGetScoreType;				//�÷�����

	int						m_iCount;						//��������



private:

	dword				    m_Time;							//ȫ��ʱ��

	//��ԴTEXTURE
private:

	cocos2d::CCSprite*				m_winscoreViewTexture;				//Ӯ�ֱ�־
	cocos2d::CCSprite*				m_bibeiViewTexture;					//�ȱ�
	cocos2d::CCSprite*				m_defenViewTexture;					//�÷�

	cocos2d::CCSprite*				m_GameNotcieTexture;				//��Ϸ����

	bool						    m_IsMos;

	int								m_iImageCount;

	//����
public:

	SHZCGameView0*						m_NodeGameView0;	//������
	SHZCGameViewBibei*					m_NodeGameView1;	//�ȴ�С
	SHZCGameViewMali*					m_NodeGameView2;	//С����

	SHZGameTest*						m_GameTest;

	CCProgressTimer *				mProgress;

	SHZTailorView*						m_TailorView;

	//����
public:

	//��Ϣ
	cocos2d::LabelAtlas				*m_LbUserScore;						//�û�����

	cocos2d::LabelAtlas				*m_LbLv;							//�ȼ�

	cocos2d::CCLabelBMFont			*m_lbwinScore;						//Ӯ��

	//��ť
public:

	//ControlButton*				m_btshare;								//����
	ControlButton*				m_btGameBack;							//����
	ControlButton*				m_btshezhi;								//����
	ControlButton*				m_btHelp;								//����
	ControlButton*				m_btJeton;								//�������
	ControlButton*                m_btRecharge;                           //��ֵ��ť
public:

	//TouchLayer*						touch_layer_;						//���ӿ�

	//��������
public:
	//���캯��
	SHZCGameClientView();
	//��������
	virtual ~SHZCGameClientView();

	//��������
	CREATE_FUNC(SHZCGameClientView);

	//�̳к���
private:
	void onBtnClick(cocos2d::CCObject* obj, cocos2d::extension::Control::EventType e);
public:

	void onReward(cocos2d::CCObject* obj);

	//��ֵ֮��ˢ��ˮ䰴�����
	void refreshCoin(Ref* ref);

	//������ʾ�˳��ĶԻ���
	void closeOutGame(Node* node);
	
protected:

	/** ��ʼ�� */
	bool init();
	/** �ͷ� */
	void OnDeinit();
	/** �ָ� */
	void OnRestore();
	/** ���� */
	void update(float dt);
	/** ��Ⱦ */
	void OnDrawView();

	//������ť
	void  CreatedGameBt();

	//�����������
	virtual void onEnterTransitionDidFinish();

	virtual void onEnter();  //��������

	//��ʾ���ϵĺ��
	void ShowTopView(bool bshow);

	//���غ���
public:
	//���ý���
	virtual void ResetGameView();
	//�����ؼ�
	virtual void RectifyControl();

	//�л��ȱ�����
	virtual void OnChangeBibeiView();

	//�л�������
	virtual void OnChangeGameView(int iCreditScore);

	//�л���������
	virtual void OnChangeMlView();

	

public:
	
	//���³齱��Ľ������Ԫ�����;���ֵ
	void  updateLotteryResult(Ref* obj);

	//��¼��һ�ȡ�ľ���
	void setLotteryExp(int myExp);
	//�����Լ��Ϸ�
	void SetCreditScoreScore(int myscore);
	//���¹������ȡ�ķ����Լ���������
	void ScrollInformation(int winscore, int index, int imgtypeid, bool bquanpan);
	//�����������ע
	void SetXiaZhu(int allcellscore);
	//��ȡ����
	void GetWinScore(int winscore, int gametype);
	//�ȱ����
	void SetBiBei(int winscore, int S1, int S2);
	//С������Ϣ
	void SetMaLiInformation(int winscore, int index, int imgtypeid, int gamecount, int tyepindex, bool nowbegin);
	//������������
	void CleanAllMemory();
	//����������ʾ
	void SetFontString(char WString);
	//�ȱ�����
	void CopyBiBeiData(int ibibeidata, int ipos);

	//ʱ��ִ��
	bool TypeImgVoid(int timeID, int typeIndex, int abindex);

	//�齱��Ϸɽ��
	void jinbiFly(Ref* ref);
	//�齱��Ϻ��Ԫ��
	void yuanbaoFly(Ref* ref);

   //һ��һ�ּӽ�ҵ�Ч��
	void addCoinScore(float t);
	//һ��һ�ּ�Ԫ����Ч��
	void addBaoScore(float t);


	//�ؼ���Ϣ
	void ControlsMessage();

	//���ư�ť
	void DrawBt();

	//���ܺ���
public:


	//����15������ͼ
	void SetTypeImg(int iPos);

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

	//�ȱ�
	void GetBiBeiData();

	//��ע
	void ChangeCellScore();

	//��Ϸ����
	void SetGameNotice(const char* strMsg);

	void RollText(cocos2d::CCNode* node, void* data);

	void DisableGamePopUp();  //��ʾ�޷���Ϸ

	//����ѹ��
	void SetYaLine(int line);


	//��ʱ��
public:

	//�ȱ��÷�
	void OnbibeiordefenTimer(float dt);

	//��ȡӮ��
	void OnDwonScoreTimer(float dt);

	//������Ч
	void playEff();

	

private:

	//��������(���Ŷ���)
	void ForeverAction(cocos2d::CCSprite* pSprite, int32 iWidth, int32 iHeight, int ilayer = 8);

	//��������(ѭ�����Ŷ���)
	void ForeverAction(cocos2d::CCSprite* pSprite, char *strName, int iCount, int iFirst = 0, int ilayer = 8);

	//��������(���Ŷ���һ��)
	void SequenceAction(const char *strImage, cocos2d::SEL_CallFuncND selector, int iCount, cocos2d::CCPoint ptImage, int iTag);

private:
	int vipColorIndex(const char* strMsg);

private:
	void recordScore(long addscore);
	/*----------�齱֮��ˢ�½��--------------*/
	int  originScore;
	int  disScore;
	int  getScore;
};

#endif