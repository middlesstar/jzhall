#ifndef SERVER_RULE_HEAD_FILE
#define SERVER_RULE_HEAD_FILE

//////////////////////////////////////////////////////////////////////////////////

//�������
#define SR_FORFEND_GAME_CHAT		0x00000001							//��ֹ����
#define SR_FORFEND_ROOM_CHAT		0x00000002							//��ֹ����
#define SR_FORFEND_WISPER_CHAT		0x00000004							//��ֹ˽��
#define SR_FORFEND_WISPER_ON_GAME	0x00000008							//��ֹ˽��

//�߼�����
#define SR_ALLOW_DYNAMIC_JOIN		0x00000010							//��̬����
#define SR_ALLOW_OFFLINE_TRUSTEE	0x00000020							//���ߴ���
#define SR_ALLOW_AVERT_CHEAT_MODE	0x00000040							//������Ϣ
#define SR_ALLOW_QUICK_MODE			0x00000080							//����ģʽ

//��Ϸ����
#define SR_RECORD_GAME_SCORE		0x00000100							//��¼����
#define SR_RECORD_GAME_TRACK		0x00000200							//��¼����
#define SR_DYNAMIC_CELL_SCORE		0x00000400							//��̬�׷�
#define SR_IMMEDIATE_WRITE_SCORE	0x00000800							//��ʱд��

//�������
#define SR_FORFEND_ROOM_ENTER		0x00001000							//��ֹ����
#define SR_FORFEND_GAME_ENTER		0x00002000							//��ֹ����
#define SR_FORFEND_GAME_LOOKON		0x00004000							//��ֹ�Թ�

//���й���
#define SR_FORFEND_TAKE_IN_ROOM		0x00010000							//��ֹȡ��
#define SR_FORFEND_TAKE_IN_GAME		0x00020000							//��ֹȡ��
#define SR_FORFEND_SAVE_IN_ROOM		0x00040000							//��ֹ��Ǯ
#define SR_FORFEND_SAVE_IN_GAME		0x00080000							//��ֹ���

//��������
#define SR_FORFEND_GAME_RULE		0x00100000							//��ֹ����
#define SR_FORFEND_LOCK_TABLE		0x00200000							//��ֹ����
#define SR_ALLOW_ANDROID_ATTEND		0x00400000							//��������
#define SR_ALLOW_ANDROID_SIMULATE	0x00800000							//����ռλ

//////////////////////////////////////////////////////////////////////////////////

//�������
class SHZCServerRule
{
	//�������
public:
	//��ֹ����
	static bool IsForfendGameChat(dword dwServerRule) { return (dwServerRule&SR_FORFEND_GAME_CHAT)!=0; }
	//��ֹ����
	static bool IsForfendRoomChat(dword dwServerRule) { return (dwServerRule&SR_FORFEND_ROOM_CHAT)!=0; }
	//��ֹ˽��
	static bool IsForfendWisperChat(dword dwServerRule) { return (dwServerRule&SR_FORFEND_WISPER_CHAT)!=0; }
	//��ֹ˽��
	static bool IsForfendWisperOnGame(dword dwServerRule) { return (dwServerRule&SR_FORFEND_WISPER_ON_GAME)!=0; }

	//ģʽ����
public:
	//��̬����
	static bool IsAllowDynamicJoin(dword dwServerRule) { return (dwServerRule&SR_ALLOW_DYNAMIC_JOIN)!=0; }
	//���ߴ���
	static bool IsAllowOffLineTrustee(dword dwServerRule) { return (dwServerRule&SR_ALLOW_OFFLINE_TRUSTEE)!=0; }
	//������Ϣ
	static bool IsAllowAvertCheatMode(dword dwServerRule) { return (dwServerRule&SR_ALLOW_AVERT_CHEAT_MODE)!=0; }
	//����ģʽ
	static bool IsAllowQuickMode(dword dwServerRule) { return (dwServerRule&SR_ALLOW_QUICK_MODE)!=0; }

	//��Ϸ����
public:
	//��¼����
	static bool IsRecordGameScore(dword dwServerRule) { return (dwServerRule&SR_RECORD_GAME_SCORE)!=0; }
	//��¼����
	static bool IsRecordGameTrack(dword dwServerRule) { return (dwServerRule&SR_RECORD_GAME_TRACK)!=0; }
	//��̬�׷�
	static bool IsDynamicCellScore(dword dwServerRule) { return (dwServerRule&SR_DYNAMIC_CELL_SCORE)!=0; }
	//��ʱд��
	static bool IsImmediateWriteScore(dword dwServerRule) { return (dwServerRule&SR_IMMEDIATE_WRITE_SCORE)!=0; }

	//�������
public:
	//��ֹ����
	static bool IsForfendRoomEnter(dword dwServerRule) { return (dwServerRule&SR_FORFEND_ROOM_ENTER)!=0; }
	//��ֹ����
	static bool IsForfendGameEnter(dword dwServerRule) { return (dwServerRule&SR_FORFEND_GAME_ENTER)!=0; }
	//��ֹ�Թ�
	static bool IsForfendGameLookon(dword dwServerRule) { return (dwServerRule&SR_FORFEND_GAME_LOOKON)!=0; }

	//���й���
public:
	//��ֹȡ��
	static bool IsForfendTakeInRoom(dword dwServerRule) { return (dwServerRule&SR_FORFEND_TAKE_IN_ROOM)!=0; }
	//��ֹȡ��
	static bool IsForfendTakeInGame(dword dwServerRule) { return (dwServerRule&SR_FORFEND_TAKE_IN_GAME)!=0; }
	//��ֹ��Ǯ
	static bool IsForfendSaveInRoom(dword dwServerRule) { return (dwServerRule&SR_FORFEND_SAVE_IN_ROOM)!=0; }
	//��ֹ��Ǯ
	static bool IsForfendSaveInGame(dword dwServerRule) { return (dwServerRule&SR_FORFEND_SAVE_IN_GAME)!=0; }

	//��������
public:
	//��ֹ����
	static bool IsForfendGameRule(dword dwServerRule) { return (dwServerRule&SR_FORFEND_GAME_RULE)!=0; }
	//��ֹ����
	static bool IsForfendLockTable(dword dwServerRule) { return (dwServerRule&SR_FORFEND_LOCK_TABLE)!=0; }
	//��������
	static bool IsAllowAndroidAttend(dword dwServerRule) { return (dwServerRule&SR_ALLOW_ANDROID_ATTEND)!=0; }
	//����ռλ
	static bool IsAllowAndroidSimulate(dword dwServerRule) { return (dwServerRule&SR_ALLOW_ANDROID_SIMULATE)!=0; }

	//�������
public:
	//��ֹ����
	static void SetForfendGameChat(dword & dwServerRule, bool bEnable) { (bEnable==true)?dwServerRule|=SR_FORFEND_GAME_CHAT:dwServerRule&=~SR_FORFEND_GAME_CHAT; }
	//��ֹ����
	static void SetForfendRoomChat(dword & dwServerRule, bool bEnable) { (bEnable==true)?dwServerRule|=SR_FORFEND_ROOM_CHAT:dwServerRule&=~SR_FORFEND_ROOM_CHAT; }
	//��ֹ˽��
	static void SetForfendWisperChat(dword & dwServerRule, bool bEnable) { (bEnable==true)?dwServerRule|=SR_FORFEND_WISPER_CHAT:dwServerRule&=~SR_FORFEND_WISPER_CHAT; }
	//��ֹ˽��
	static void SetForfendWisperOnGame(dword & dwServerRule, bool bEnable) { (bEnable==true)?dwServerRule|=SR_FORFEND_WISPER_ON_GAME:dwServerRule&=~SR_FORFEND_WISPER_ON_GAME; }

	//ģʽ����
public:
	//��̬����
	static void SetAllowDynamicJoin(dword & dwServerRule, bool bEnable) { (bEnable==true)?dwServerRule|=SR_ALLOW_DYNAMIC_JOIN:dwServerRule&=~SR_ALLOW_DYNAMIC_JOIN; }
	//���ߴ���
	static void SetAllowOffLineTrustee(dword & dwServerRule, bool bEnable) { (bEnable==true)?dwServerRule|=SR_ALLOW_OFFLINE_TRUSTEE:dwServerRule&=~SR_ALLOW_OFFLINE_TRUSTEE; }
	//������Ϣ
	static void SetAllowAvertCheatMode(dword & dwServerRule, bool bEnable) { (bEnable==true)?dwServerRule|=SR_ALLOW_AVERT_CHEAT_MODE:dwServerRule&=~SR_ALLOW_AVERT_CHEAT_MODE; }
	//����ģʽ
	static void SetAllowQuickMode(dword & dwServerRule, bool bEnable) { (bEnable==true)?dwServerRule|=SR_ALLOW_QUICK_MODE:dwServerRule&=~SR_ALLOW_QUICK_MODE; }

	//��Ϸ����
public:
	//��¼����
	static void SetRecordGameScore(dword & dwServerRule, bool bEnable) { (bEnable==true)?dwServerRule|=SR_RECORD_GAME_SCORE:dwServerRule&=~SR_RECORD_GAME_SCORE; }
	//��¼����
	static void SetRecordGameTrack(dword & dwServerRule, bool bEnable) { (bEnable==true)?dwServerRule|=SR_RECORD_GAME_TRACK:dwServerRule&=~SR_RECORD_GAME_TRACK; }
	//��̬�׷�
	static void SetDynamicCellScore(dword & dwServerRule, bool bEnable) { (bEnable==true)?dwServerRule|=SR_DYNAMIC_CELL_SCORE:dwServerRule&=~SR_DYNAMIC_CELL_SCORE; }
	//��ʱд��
	static void SetImmediateWriteScore(dword & dwServerRule, bool bEnable) { (bEnable==true)?dwServerRule|=SR_IMMEDIATE_WRITE_SCORE:dwServerRule&=~SR_IMMEDIATE_WRITE_SCORE; }

	//�������
public:
	//��ֹ����
	static void SetForfendRoomEnter(dword & dwServerRule, bool bEnable) { (bEnable==true)?dwServerRule|=SR_FORFEND_ROOM_ENTER:dwServerRule&=~SR_FORFEND_ROOM_ENTER; }
	//��ֹ����
	static void SetForfendGameEnter(dword & dwServerRule, bool bEnable) { (bEnable==true)?dwServerRule|=SR_FORFEND_GAME_ENTER:dwServerRule&=~SR_FORFEND_GAME_ENTER; }
	//��ֹ�Թ�
	static void SetForfendGameLookon(dword & dwServerRule, bool bEnable) { (bEnable==true)?dwServerRule|=SR_FORFEND_GAME_LOOKON:dwServerRule&=~SR_FORFEND_GAME_LOOKON; }

	//���й���
public:
	//��ֹȡ��
	static void SetForfendTakeInRoom(dword & dwServerRule, bool bEnable) { (bEnable==true)?dwServerRule|=SR_FORFEND_TAKE_IN_ROOM:dwServerRule&=~SR_FORFEND_TAKE_IN_ROOM; }
	//��ֹȡ��
	static void SetForfendTakeInGame(dword & dwServerRule, bool bEnable) { (bEnable==true)?dwServerRule|=SR_FORFEND_TAKE_IN_GAME:dwServerRule&=~SR_FORFEND_TAKE_IN_GAME; }
	//��ֹ��Ǯ
	static void SetForfendSaveInRoom(dword & dwServerRule, bool bEnable) { (bEnable==true)?dwServerRule|=SR_FORFEND_SAVE_IN_ROOM:dwServerRule&=~SR_FORFEND_SAVE_IN_ROOM; }
	//��ֹ��Ǯ
	static void SetForfendSaveInGame(dword & dwServerRule, bool bEnable) { (bEnable==true)?dwServerRule|=SR_FORFEND_SAVE_IN_GAME:dwServerRule&=~SR_FORFEND_SAVE_IN_GAME; }

	//��������
public:
	//��ֹ����
	static void SetForfendGameRule(dword & dwServerRule, bool bEnable) { (bEnable==true)?dwServerRule|=SR_FORFEND_GAME_RULE:dwServerRule&=~SR_FORFEND_GAME_RULE; }
	//��ֹ����
	static void SetForfendLockTable(dword & dwServerRule, bool bEnable) { (bEnable==true)?dwServerRule|=SR_FORFEND_LOCK_TABLE:dwServerRule&=~SR_FORFEND_LOCK_TABLE; }
	//��������
	static void SetAllowAndroidAttend(dword & dwServerRule, bool bEnable) { (bEnable==true)?dwServerRule|=SR_ALLOW_ANDROID_ATTEND:dwServerRule&=~SR_ALLOW_ANDROID_ATTEND; }
	//����ռλ
	static void SetAllowAndroidSimulate(dword & dwServerRule, bool bEnable) { (bEnable==true)?dwServerRule|=SR_ALLOW_ANDROID_SIMULATE:dwServerRule&=~SR_ALLOW_ANDROID_SIMULATE; }
};

//////////////////////////////////////////////////////////////////////////////////

#endif