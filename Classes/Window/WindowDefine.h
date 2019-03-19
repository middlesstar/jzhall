#ifndef  _WindowDefine_H_
#define  _WindowDefine_H_

//���ļ��������д��ڱ��
enum eWindowType
{
	EWT_Login = 0,				//��¼
	EWT_LoginAccount,			//�˺ŵ�¼
	EWT_Service,				//�ͷ�
	EWT_Hall,					//����

	EWT_Shop,					//�̳�
	EWT_MoneyTree,				//ҡǮ��
	EWT_FreeMoney,				//��ѽ��
	EWT_Rank,					//���а�
	EWT_Promote,				//�ƹ�׬��
	EWT_Exchange,				//�ҽ�
	EWT_VipGift,				//VIP���
	EWT_Bank,					//����
	EWT_Mail,					//�ʼ�
	EWT_Setting,				//����

	EWT_Personal,				//��������
	EWT_TicketSend,				//���ʹ�Ʊ
	EWT_Sign,					//ÿ����ս
	EWT_DialyPrize,				//ÿ�ճ齱
	EWT_WeiXin,					//΢�Ź��ں�
	EWT_Comment,				//��������
	EWT_Dole,					//�ȼý�
	EWT_Expedition,				//Զ��
	EWT_Activity,				//�

	EWT_Share,					//�������
	EWT_TreeUpgrade,			//ҡǮ������
	EWT_Chat,					//�������
	EWT_SendRedPacket,			//���췢�������
	EWT_PacketResult,			//���췢����������

	//todosl
	EWT_Update,                //���½���

	EWT_MAX
};

#endif // _WindowDefine_H_

