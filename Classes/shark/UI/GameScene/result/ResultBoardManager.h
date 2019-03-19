#ifndef  _ResultBoardManager_H_
#define  _ResultBoardManager_H_

#include "cocos2d.h"
#include "shark/Base/ViewBase.h"
#include "shark/UI/GameScene/result/OneOfResult.h"

class ResultBoardManager : public ViewBase
{
public:
	ResultBoardManager();
	virtual ~ResultBoardManager();
	CREATE_FUNC(ResultBoardManager);

	virtual void setContent();

	//������˸
	void startPlay();
	//�ر���˸
	void stopPlay();

protected:

	//������ʾ��ԭ
	void setRestore();
	//���ó�����Ӯ����ʾ
	void setBigWin();
	//����Ӯ����ʾ
	void setWin();
	//���㱾����Ϸ�ĵ÷�
	void summaryNum();

	//������Ϸ�÷�
	void summaryScore();

	//��ʾ��Ч
	void playEff(float t);


	//����ͨ����
	void generalSummary(int index);
	//�н���
	void sharkSummary(int index);

	//�вʽ���
	void colorSummary(int index);

	//�ʽ������ʵ��
	void colorShark(int index);

	//����Ӯȡ������Ԫ��������Ʊ��
	void setWinScore(int score, int yuanbao, int ticket);

	//��ձ��ֵ÷ּ�¼
	void clearWinScore();

public:
	//���ñ���
	void SetRate(int index);
	//������ʾ������Ϸ�еİ�ť
	void showWinBtn(int index,bool flag,int score);

	//���Ž����Ȫ�����ֹ���
	void goldandnumroll(float t);
	//���ֹ���Ч��
	void numroll(float t);
	//����GetNum��������string
	void setGetnum(SCORE l);
	//���Ž�ҷ��ж���
	void playGoldFly(Vec2 startPos, Vec2 endPos);
protected:
	//���ڵ�
	Node*              pMainNode;
	Node*              downNode;
	Node*              commandShow;
	Node*              winNode;
	Node*              stakeButton;
	//Node*              lostNode;
	TextAtlas*         scoreLabel;
	TextAtlas*         ticketLabel;
	TextAtlas*         yuanBaoLabel;

	Sprite*            winLogo;
	Sprite*            bigWinLogo;

	//�洢���еĹ�Ȧ
	cocos2d::Vector<Sprite* > ringCircle;
	//�洢���е���ʾlabel
	cocos2d::Vector<TextAtlas*>textScore;

	//�����������
	int            ticketNum;
	int            yuanbaoNum;
	int            scoreNum;

	//Stake_Btn_Max��ѹ����尴ť  
	OneOfResult		m_stakeArray[12];


};

#endif // _SharkResultManger_H_

