#ifndef SHZ_Bird_H_
#define SHZ_Bird_H_

///////////////////////////////////////////////////////////////////////////////////////////
class SHZBird
{
public:
	SHZBird() 
	{
		reset();
	}

	void reset()
	{
		rotation_	= 0;
		type_		= 0;
		item_		= 0;
		live_		= 1;
		path_id_	= 0;
		path_type_	= 0;
		path_delay_ = 0;
		elapsed_	= 0;
		speed_		= (float)BIRD_MOVE_NORMAL_SHZ;
		node_		= 0;
		shadow_		= 0;
		effect_		= 0;
	}

public:
	uint32	id_;
	float	rotation_;
	SHZxSize	size_;
	SHZxPoint	position_; 
	uint8	type_;
	uint8	item_ ;
	uint8	live_;
	uint16	path_id_;
	uint8	path_type_;
	float	path_delay_;
	SHZxPoint	path_offset_;
	float	elapsed_;
	float	speed_;
	uint32	move_action_id_;

	void* node_;
	void* shadow_;
	void* effect_;
};

///////////////////////////////////////////////////////////////////////////////////////////
typedef std::map<uint32_t, SHZBird*>	BirdManager_SHZ;
typedef BirdManager_SHZ::iterator		BirdManagerIter_SHZ;

#endif // SHZ_Bird_H_