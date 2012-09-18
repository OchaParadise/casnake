//#ifndef INCLUDED_DISPLAY
//#define INCLUDED_DISPLAY

//#include "DxLib.h"

class Display
{
public:
	void disp_character();
	void disp_map(); 
	int game_loop(int IsContinue);
	void tail_disp(int numX, int numY, int DrawPointX, int DrawPointY);
	int MessageEnding();
	int MessageGameOver();
	int MessageSelectMode();
	void Introduction();
	void Thanks();
};

//#endif