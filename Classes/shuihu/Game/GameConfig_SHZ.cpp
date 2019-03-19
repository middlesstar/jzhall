#include "shuihu/GameHeader_SHZ.h"
#include "GameConfig_SHZ.h"

SHZGameConfig gGameConfig_SHZ;

///////////////////////////////////////////////////////////////////////////
SHZGameConfig::SHZGameConfig()
{
	init(40, 25, 1, 1);
}

void SHZGameConfig::init(int cannon_type_max, int cannon_price_base, int ex_score_count, int ex_bullet_count)
{
	ex_bullet_count_	= ex_bullet_count;
	ex_score_count_		= ex_score_count;
	cannon_type_max_	= cannon_type_max;
	cannon_price_base_	= cannon_price_base;
}

int SHZGameConfig::get_cannon_price(int cannon) const
{
	return cannon_price_base_ * (cannon + 1);
}
///////////////////////////////////////////////////////////////////////////
