#ifndef SHZ__GAME_CONFIG_H__
#define SHZ__GAME_CONFIG_H__

///////////////////////////////////////////////////////////////////////////////////////
class SHZGameConfig
{
public:
	SHZGameConfig();
	
	void init(int cannon_type_max, int cannon_price_base, int ex_score_count, int ex_bullet_count);
 
	int get_bullet_radius() const					{ return BULLET_RADIUS_SHZ; }
	const SHZxSize& get_bird_size(int bird_type) const	{ return BIRD_SIZES_SHZ[bird_type]; }

	int get_max_rank() const {return 10;}
	int get_cannon_type_max() const {return cannon_type_max_;}
	int get_cannon_price_base() const {return cannon_price_base_;}
	int get_cannon_price(int connon_type) const;
	int get_ex_score_count() {return ex_score_count_;}
	int get_ex_bullet_count() {return ex_bullet_count_;}
private:
	int cannon_type_max_;
	int cannon_price_base_;
	int ex_score_count_;
	int ex_bullet_count_;
}; 

extern SHZGameConfig gGameConfig_SHZ;

///////////////////////////////////////////////////////////////////////////////////////

#endif