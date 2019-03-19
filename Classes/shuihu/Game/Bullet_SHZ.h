#ifndef SHZ_Bullet_H_
#define SHZ_Bullet_H_

class SHZBullet
{
public:
	uint32	id_;
	float	rotation_;
	uint16	chair_id_;
	uint16	cannon_type_;
	uint32	move_action_id_;
	float	speed_;
	void*   node_;
	uint16  bullet_type_;
	uint16  net_type_;
};

///////////////////////////////////////////////////////////////////////////////////////////
typedef std::list<SHZBullet*> BulletManager_SHZ;

#endif // SHZ_Bullet_H_