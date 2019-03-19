#ifndef  _ActiveConfig_H_
#define  _ActiveConfig_H_


#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include "LanguageConfig.h"
#include "Sound/SoundManager.h"
using namespace CocosDenshion;
USING_NS_CC;

extern int _RAND0;
extern int _RAND1;
extern int _RAND2;

extern Vec2 VISIBLE_SIZE;
//std::string WStrToUTF8(const std::wstring& str);


//������Ϸ
#ifndef _RUN_FISH_ACTIVETY(_Type ,CallBack ,This)
#define _RUN_FISH_ACTIVETY(_Type ,CallBack ,This) \
	auto x = _Type::create(); \
	Director::getInstance()->getRunningScene()->addChild(x); \
	x->SetCallback(This, (SEL_CallFun)(&CallBack)); \
	x->StartPlay();
#endif


#ifndef _PLAY_MUSCI_EFF(X)
//#define _PLAY_MUSCI_EFF(X)	SimpleAudioEngine::getInstance()->playEffect(X);
#define _PLAY_MUSCI_EFF(X)		SoundManager::shared()->playEffectNormal(X)
#endif 

#ifndef _PLAY_MUSCI_BACK(X)
//#define _PLAY_MUSCI_BACK(X)	SimpleAudioEngine::getInstance()->playBackgroundMusic(X,true);
#define _PLAY_MUSCI_BACK(X)	SoundManager::shared()->playMusicNormal(X,true)
#endif 

//����Ҫ�����ű���(��.���.ˮ��)
#define ALL_SCALE				0.7

#ifndef _TRANSPARENT
#define _TRANSPARENT			255
#endif // !_TRANSPARENT


#ifndef _ZERO
#define _ZERO					0
#endif // !_ZERO

#ifndef _PI
#define _PI						3.1415926
#endif // !PI

#ifndef RES_PLIST_PNG
#define RES_PLIST_PNG
#define	PLIST_PATH				"images/hall/activity/red_packet.plist"
#define	_BK						"rp_blue.png"
#define _TOP					"rp_top.png"
#define _WAVE0					"rp_wave0.png"
#define _WAVE1					"rp_wave1.png"
#define	_FISH0					"rp_fish0.png"
#define	_FISH1					"rp_fish1.png"
#define	_RBOMB0					"rp_bomb_0.png"
#define _RBOMB1					"rp_bomb_1.png"
#define RED_PAKAGE0				"rp_red_0.png"		
#define RED_PAKAGE1				"rp_red_1.png"	
#define RED_PAKAGE2				"rp_red_2.png"	
#define	_TICKET0				"rp_ticket_0.png"
#define RP_DROPS				"rp_drops.png"
#define _BAO					"rp_box.png"

#endif // !COCOS_PLIST_PNG

//���Ϸ�����
#define TEXT_CONTENT			GETSTRING(1300)
#define	TEXT_FONT				GETSTRING(1301)
#define TEXT_SIZE				24


//Info��Ϣ��ǩ
#define	INFO_TIME				GETSTRING(1302)
#define	INFO_GOLD				GETSTRING(1303)
#define INFO_HF					GETSTRING(1304)
#define INFO_SIZE				22
#define INFO_LABEL_COLOR		Color3B(0,255,0)
#define INFO_TEXT_COLOR			Color3B(255,255,0)
#define INFO_TIME_MAX			60								//60��
#define INFO_YOFFSET			33								//Y��λ������


//�� ����
#define FISH_MIN_HEIGHT			100								//���ζ���Y����
#define	FISH_OFFSET_X			77								//��ײǽƫ�� ͼƬ��ȵ�һ��
#define	FISH_JUMP_SLIDER		5								//����Ƕ�ƽ��ϵ��
#define FISH_MOVE_X_V			200								//������ƶ��ٶ�
#define FISH_DIVE_V				500								//�ϸ����ٶ�
#define FISH_DIVE_LOSE			0.3								//����ˮ��������ʧ
#define FISH_G					1000							//�������ٶ�
#define FISH_JUMP_V				1100							//�������ٶ�
#define FISH_JUMP_FORCE1		0.7								//��������1
#define FISH_JUMP_FORCE2		0.6								//��������2
#define FISH_JUMP_LOSE			0.15							//2����������ʧ

//ˮ������



//�������
#define PACKET_TIME				0.4								//����ܼ���  0.8s�������
#define	PACKET_V_MIN			200								//����ٶ�small
#define	PACKET_V_MID			500								//����ٶ�mid
#define PACKET_V_MAX			800								//����ٶ�big
#define	PACKET_TEXT_SIZE		30								//����������ִ�С
#define PACKET_TEXT_COLOR0		Color3B(255,0,0)			
#define PACKET_TEXT_COLOR1		Color3B(255,0,0)		
#define PACKET_LAYER_COLOR		Color4B(255,0,0,255)	
#define PACKET_TEXT_V_Y			100								//�����ϸ��ٶ�
#define PACKET_TEXT_MAX_TIME	1.f								//�����ϸ�ʱ��	
#define PACKET_SCREEN_CLEAR		.5f								//ը������ʱ��	
#define PACKET_TRANSPARENT		128								//ը������͸����
#define PACKET_UP_DOWN_V		50								//������¸����ٶ�	
#define PACKET_UP_DOWN_TIME		.5f								//������¸���ʱ��
#define	PACKET_ROTATE_ANGLE		60								//��������ת�Ƕ�
#define	PACKET_ROTATE_TIME		.5f								//�����תʱ��
#define PACKET_HF_MAX			15								//���ѵõ��������ٲ���
#define PACKET_HF_FLOAT			10.f							//���Ѿ�ȷһλС�� ��2λС��Ϊ100  �Դ����ƣ�



//����path
#define MUSIC_BK				"music/ios/activity/pk.mp3"					//��������
#define MUSIC_FISH_JUMP			"music/ios/activity/click.mp3"					//������
#define	MUSCI_EAT_PACKET		"music/ios/activity/get_gold.mp3"				//�ٵ����
#define MUSCI_EAT_BOMB_GET		"music/ios/activity/mfish_gold.mp3"			//��ը��
#define MUSCI_EAT_BOMB_DIE		"music/ios/activity/mfish_31.mp3"				//��ը��

#endif // _ActiveConfig_H_


