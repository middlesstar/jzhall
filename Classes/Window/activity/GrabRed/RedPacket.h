#ifndef  _Redpacket_H_
#define  _Redpacket_H_

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "ActiveConfig.h"

USING_NS_CC;
using namespace ui;

class RedPacket :
	public Node
{
public:
	struct OnePacket : public Sprite
	{
		enum enumType{
			PACKET_GOLD_SMALL,
			PACKET_GOLD_MID,
			PACKET_GOLD_BIG,
			PACKET_HF_SMALL,
			PACKET_HF_BIG,
			BAO,
			BOMB_GET,
			BOMB_DIE
		};

		static OnePacket* createWithSpriteFrame(SpriteFrame *spriteFrame)
		{
			OnePacket *sprite = new (std::nothrow) OnePacket();
			if (sprite && spriteFrame && sprite->initWithSpriteFrame(spriteFrame))
			{
				sprite->autorelease();
				return sprite;
			}
			CC_SAFE_DELETE(sprite);
			return nullptr;
		}

		enumType				m_type;
		float					m_num;
		float					m_vX;
		float					m_time;
		float					m_vY;
	};

	struct PacketText : public Text
	{
		float time;

		static PacketText* create(const std::string &textContent, const std::string &fontName, int fontSize)
		{
			PacketText *text = new (std::nothrow) PacketText;
			if (text && text->init(textContent, fontName, fontSize))
			{
				text->autorelease();
				return text;
			}
			CC_SAFE_DELETE(text);
			return nullptr;
		}
	};


protected:
	bool						m_bIsActivity;
	Vector<OnePacket*>			m_packetList;
	Vector<PacketText*>			m_textList;
	CCActionInterval*			m_fadeout;
	CCActionInterval*			m_screenFadeout;
	CCLayerColor*				m_colorLayer;

	bool						m_bIsCreatHF;				//是否产生话费
public:
	RedPacket();
	~RedPacket();
	virtual void OnInit();

	
	void StartPlay();
	void StopPlay();

	void EatPacket(float t, OnePacket::enumType e, Vec2 v);

	Vector<OnePacket*>* GetList(){ return &m_packetList; }

	void BombGet(int &gold, float &hf);
	void BombDie();
	//主管说：得到话费超过1元 就不能再生产话费 增加此接口  在管理类（GrabRed中调用）
	void SetCreatHF(bool b){ m_bIsCreatHF = b; };
protected:
	void CreatPacket(float t);
	void update(float t);

	void UpdateColorLayer(float t);
	


private:

};

#endif // _Redpacket_H_