//���ֽṹ�嶨��
//�ӵ��ṹ
#ifndef SHZ__GameStruct__H_
#define SHZ__GameStruct__H_

#include "shuihu/GameHeader_SHZ.h"

//����ͼƬ
struct SHZTypeImgStruct
{
	  //����ͼ
	  int m_typeid;
	  bool m_have;
	  //״̬ͼ
	  int m_index;
	  //��ǰͼƬ״̬,0��,1�ڰ�
	  int m_type;
	  //����
	  int m_Rows;
	  //����
	  int m_Cons;
	  //�Ƿ���Ȧ
	  bool m_havequan;
	  //ѭ������
	  int  m_RoundCount;
	  //
	  bool  m_CanAction;

};
//NPC�ṹ
struct NpcBossStruct_SHZ
{
	  //NPC״̬,����,ʧ��,���ߵ�
      int m_faceState;
	  //��ǰͼƬ����
	  int m_ImgIndex;
	  //

	  //
};
class SHZCGameSceneInterface 
{

public:

	//�л��ȱ�����
	virtual void OnChangeBibeiView()=0;

	//�л�������
	virtual void OnChangeGameView(int iCreditScore)=0;

	//�л���������
	virtual void OnChangeMlView()=0;

};
#endif