#ifndef _SharkGameScene_H_
#define _SharkGameScene_H_

#include "cocos2d.h"
#include "MainView.h"

class SharkGameScene : public cocos2d::Scene
{
public:
	virtual bool init();
	//��ʼ��Ϸ
	void gameStart(int leftTime);
	//����
	void openPrize(int leftTime);
	//�ȴ���һ����Ϸ
	void waitNextGame(int leftTime);


	virtual void onEnter() override;

	virtual void onExit() override;

	virtual void onEnterTransitionDidFinish() override;

	CREATE_FUNC(SharkGameScene);
	//��������Ӧ
	virtual void OnHreatPak();
protected:
	/*-------------------��Ϸ��-------------------------*/
	void gameRunning(float t);

	void bridge(float t);


	//void PlayAnimalMusic(enumSpecialRs eRs, enumAnimalType eType, enumAnimalColor eColor);

	void waitUpdate(float t);

	//������
	void CheckNet(float t);
	void checkNetupdate(float t);
	//����������
	void updateSendHreat(float t);

protected:
	/*---------------------����-------------------------*/
	//��ͨ����
	void generalResult(int index);
	//�����㿪��
	void sharkResult(int index);
	//�����㿪��
	void silverResult(int index);

	//�����㿪��
	void colorShark(int index);
	bool AddGold(int g);

	//�ж�����һ������
	void judgeResult(int index, bool colorsharFlag);
	//ֹͣ����
	void stopResult(int index, bool colorsharFlag);
	//��ʾ�����ܽ����
	void showResult();
	//��ʾ��ȡ�ı���
	void showRate();
	//���ر�����ʾ�Ľ���
	void hideRate();
	//�ж϶���
	void judgeAnimal(int index);
	
	//�����õ�Ԫ�����ߴ�Ʊ�ĸ���
	int countNum(int index);

   //�жϿ���������ʾ��һ������
	void  showAnimal();

	//������������Ķ���
	void stopResAnimal();

	//������������л���Ѻ�ֽ���
	void changeView();

	//������Ϸ������
	void goMainScene(float t);

	//�����������һ������
	void battleResult();
protected:
	void comeFromBack(Ref* ref);               //�Ӻ�̨����ǰ��
	void comeToBack(Ref* ref);                 //��ǰ�˽����̨
	void startTimer(float time);                         //������ʱ��
	
protected:
	int						m_nowIndex;				//��ǰ��Ϸ����
	int                     m_nowState;             //��ǰ��Ϸ״̬
	bool					m_bIslive;				//���������Ƿ������ϼ��
	int                     timeDelay;              //��������ʱ���ӳ�

	time_t                  outTime;                //�����̨ʱ��ʱ���

	int                     resultIndex;            //������������

public:
	static  MainView*      m_mainView;
	static SharkGameScene*	This;
	Node*                   chat;



};

#endif // _SharkGameScene_H_