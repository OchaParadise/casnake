#ifndef INCLUDED_CHARACTER
#define INCLUDED_CHARACTER

class Character
{
public:
	void player_init();
	void player_coordinate_init();
	int player_input();
	int player_move();
	void hit_item();
	void tail_rechain(int old_playerX, int old_playerY);
};

#endif