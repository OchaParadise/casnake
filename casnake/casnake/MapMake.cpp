#include "MapMake.h"
#include "Variable.h"
#include "Calc.h"
#include <vector>

using namespace std;

void MapMake::map_init()
{
	int i, j;

	for(j = 0; j < MAP_H; j++){
		for(i = 0; i < MAP_W; i++){
			map[i][j] = BLANK;
		}
	}
}

void MapMake::rect_split(_rect *rect_parent, int precutDirect)
{
	Calc calc;
	_rect split_rect[2];

	split_rect[0] = *rect_parent;
	split_rect[1] = *rect_parent;

	//if parent ‚Ìrect‚ªƒRƒŒˆÈã•ªŠ„‚Å‚«‚È‚¢(Å¬‹æ‰æ~2‚æ‚è¬‚³‚¢‚Æ‚«)‚Íreturn
	if(rect_parent->hx - rect_parent->lx <= MINIMUM_RECT_SIZE*2 + 5 ||
		rect_parent->hy - rect_parent->ly <= MINIMUM_RECT_SIZE*2 + 5)
	{
		return;
	}

	if(rand()%2 == 0){
		// ‰¡‚É(y²‚Å)‹æØ‚é
		int split_coord_y;
		int loopcnt = 0;

		if(rect_parent->hy - rect_parent->ly < MINIMUM_RECT_SIZE*2 + 2)
		{
			split_coord_y = rect_parent->ly + (rect_parent->hy - rect_parent->ly)/2;
		}
		else{
			do{
				if(loopcnt > 5000){
					return;
				}
				split_coord_y = calc.Random(rect_parent->ly + MINIMUM_RECT_SIZE +1, rect_parent->hy - MINIMUM_RECT_SIZE);
				loopcnt++;
			}while(split_coord_y - rect_parent->ly <= MINIMUM_RECT_SIZE * 2 || rect_parent->hy - split_coord_y <= MINIMUM_RECT_SIZE * 2);
		}

		split_rect[0].hy = split_coord_y;
		split_rect[1].ly = split_coord_y;

		// add‚Érect[0]‚ğ“Š‚°‚Älist‚É’Ç‰Á
		rect_add(split_rect[0].lx, split_rect[0].ly, split_rect[0].hx, split_rect[0].hy, cutY, precutDirect);
		rect_split(&split_rect[1], cutY);
	}else{
		// c‚É(x²‚Å)‹æØ‚é
		int split_coord_x;
		int loopcnt = 0;
		
		if(rect_parent->hx - rect_parent->lx < MINIMUM_RECT_SIZE*2 + 2){
			split_coord_x = rect_parent->lx + (rect_parent->hx - rect_parent->lx)/2;
		}else{
			do{
				if(loopcnt > 5000){
					return;
				}
				split_coord_x = calc.Random(rect_parent->lx + MINIMUM_RECT_SIZE, rect_parent->hx - MINIMUM_RECT_SIZE);
				loopcnt++;
			}while(split_coord_x - rect_parent->ly <= MINIMUM_RECT_SIZE * 2 || rect_parent->hy - split_coord_x <= MINIMUM_RECT_SIZE * 2);
		}

		split_rect[0].hx = split_coord_x;
		split_rect[1].lx = split_coord_x;

		// add‚Érect[0]‚ğ“Š‚°‚Älist‚É’Ç‰Á
		rect_add(split_rect[0].lx, split_rect[0].ly, split_rect[0].hx, split_rect[0].hy, cutX, precutDirect);
		rect_split(&split_rect[1], cutX);
	}
}

_rect* MapMake::rect_add(int lx, int ly, int hx, int hy, int cutDirection, int precutDirection)
{
	_rect *rect = new _rect;

	rect->lx = lx;
	rect->ly = ly;
	rect->hx = hx;
	rect->hy = hy;
	rect->cutDirection = cutDirection;
	rect->precutDirection = precutDirection;

	rect_list.push_back(*rect);

	delete rect;

	return rect;
}

void MapMake::room_make()
{	
	int x, y, w, h;
	_path path;
	_path path_front;

	vector<_rect>::iterator it = rect_list.begin();

	for(it = rect_list.begin(); it != rect_list.end(); it++){
		w = (rand()%(it->hx - it->lx - (MARGIN_BETWEEN_RECT_ROOM * 2) + 1 - MINIMUM_ROOM_SIZE)) + MINIMUM_ROOM_SIZE;
		h = (rand()%(it->hy - it->ly - (MARGIN_BETWEEN_RECT_ROOM * 2) + 1 - MINIMUM_ROOM_SIZE)) + MINIMUM_ROOM_SIZE;
		x = (rand()%(it->hx - MARGIN_BETWEEN_RECT_ROOM - w + 1 - (it->lx + MARGIN_BETWEEN_RECT_ROOM))) + it->lx + MARGIN_BETWEEN_RECT_ROOM;
		y = (rand()%(it->hy - MARGIN_BETWEEN_RECT_ROOM - h + 1 - (it->ly + MARGIN_BETWEEN_RECT_ROOM))) + it->ly + MARGIN_BETWEEN_RECT_ROOM;

		if(it->cutDirection == cutX){
			// c‚ÅØ‚Á‚½
			path.pathX = x + w + 1;
			path.pathW = it->hx + 1;
			path.pathY = y + h/2;
			path.pathH = path.pathY+1;
			path.isPathVertical = false;
		}else{
			// ‰¡‚ÅØ‚Á‚½
			path.pathX = x + w/2;
			path.pathW = path.pathX+1;
			path.pathY = y + h + 1;
			path.pathH = it->hy + 1;
			path.isPathVertical = true;			
		}

		if(it->precutDirection == cutX){
			// ˆê‚Â‘O‚Æc‚Å‚«‚ç‚ê‚½
			path_front.pathX = it->lx;
			path_front.pathW = x;
			path_front.pathY = y + h/2;
			path_front.pathH = path_front.pathY+1;
			path_front.isPathVertical = false;
		}else{
			// ˆê‚Â‘O‚Æ‰¡‚ÅØ‚ç‚ê‚½
			path_front.pathX = x + w/2;
			path_front.pathW = path_front.pathX+1;
			path_front.pathY = it->ly;
			path_front.pathH = y;
			path_front.isPathVertical = true;
		}

		it->room = room_add(x, y, x + w, y + h, path, path_front);
		delete it->room;
	}
}

_room* MapMake::room_add(int lx, int ly, int hx, int hy, _path path, _path path_front)
{
	_room *room = new _room;

	room->lx = lx;
	room->ly = ly;
	room->hx = hx;
	room->hy = hy;
	room->path.pathX = path.pathX;
	room->path.pathY = path.pathY;
	room->path.pathW = path.pathW;
	room->path.pathH = path.pathH;
	room->path.isPathVertical = path.isPathVertical;
	room->path_front.pathX = path_front.pathX;
	room->path_front.pathY = path_front.pathY;
	room->path_front.pathW = path_front.pathW;
	room->path_front.pathH = path_front.pathH;
	room->path_front.isPathVertical = path_front.isPathVertical;

	room_list.push_back(*room);

	//delete room;

	return room;
}

void MapMake::list_to_map()
{
	int i, j;

	vector<_rect>::iterator it_rect = rect_list.begin();
	while(it_rect != rect_list.end() ){
		for(i = it_rect->lx, j = it_rect->ly; i <= it_rect->hx; i++){
			map[i][j]= WALL;
		}
		for(i = it_rect->lx, j = it_rect->hy; i <= it_rect->hx; i++){
			map[i][j]= WALL;
		}
		for(i = it_rect->lx, j = it_rect->ly; j <= it_rect->hy; j++){
			map[i][j]= WALL;
		}
		for(i = it_rect->hx, j = it_rect->ly; j <= it_rect->hy; j++){
			map[i][j]= WALL;
		}
		it_rect++;
	}

	// •”‰®¶¬
	vector<_room>::iterator it_room = room_list.begin();
	while(it_room != room_list.end()){
		for(i = it_room->lx; i <= it_room->hx; i++){
			for(j = it_room->ly; j <= it_room->hy; j++){
				map[i][j] = ROOM;
			}
		}
		it_room++;
	}

	// ’Ê˜H¶¬
	it_room = room_list.begin();
	while(it_room != room_list.end()-1){  // ˆê”ÔÅŒã‚Í”ò‚Î‚·
		for(i = it_room->path.pathX; i < it_room->path.pathW; i++){
			for(j = it_room->path.pathY; j < it_room->path.pathH; j++){
				map[i][j] = PATH;
				// ’Ê˜H‚Ì•‚ğ3ƒ}ƒX‚É‚·‚é
				if(it_room->path.isPathVertical == true){
					map[i][j+1] = APPEND_PATH;
					map[i-1][j] = APPEND_PATH;
					map[i-1][j+1] = APPEND_PATH;
					map[i+1][j] = APPEND_PATH;
					map[i+1][j+1] = APPEND_PATH;
				}else{
					map[i+1][j] = APPEND_PATH;
					map[i][j-1] = APPEND_PATH;
					map[i+1][j-1] = APPEND_PATH;
					map[i][j+1] = APPEND_PATH;
					map[i+1][j+1] = APPEND_PATH;
				}
			}
		}
		it_room++;
	}

	// ’Ê˜H¶¬(ˆê‚Â‘O‚Æ‚Ì‹æØ‚è)
	it_room = room_list.begin();
	it_room++;                            // ˆê”ÔÅ‰‚Í”ò‚Î‚·
	while(it_room != room_list.end()){
		for(i = it_room->path_front.pathX; i < it_room->path_front.pathW; i++){
			for(j = it_room->path_front.pathY; j < it_room->path_front.pathH; j++){
				map[i][j] = PATH_FRONT;
				// ’Ê˜H‚Ì•‚ğ3ƒ}ƒX‚É‚·‚é
				if(it_room->path_front.isPathVertical == true){
					map[i][j+1] = APPEND_PATH;
					map[i-1][j] = APPEND_PATH;
					map[i-1][j-1] = APPEND_PATH;
					map[i+1][j] = APPEND_PATH;
					map[i+1][j-1] = APPEND_PATH;
				}else{
					map[i+1][j] = APPEND_PATH;
					map[i][j-1] = APPEND_PATH;
					map[i-1][j-1] = APPEND_PATH;
					map[i][j+1] = APPEND_PATH;
					map[i-1][j+1] = APPEND_PATH;
				}
			}
		}
		it_room++;
	}

	// ’Ê˜H¶¬(’Ê˜H“¯m‚É‹²‚Ü‚ê‚½‹«ŠE•”•ª‚ğ’Ê˜H‚É‚·‚é)
	bool path_flag = false;
	bool wall_flag = false;
	int container_count = 0;

	it_room = room_list.begin();
	while(it_room != room_list.end()-1){
		// ’Ê˜H‚É‚·‚é‹æ‰æ‚ª‰¡ <-> ’Ê˜Hc
		if(it_room->path.isPathVertical == true){
			for(i = 0; i < MAP_W; i++){
				if(map[i][it_room->path.pathH -1] == WALL && map[i+1][it_room->path.pathH -1] == WALL && wall_flag == false){ 
					wall_flag = true;
				}
				if(map[i][room_list[container_count].path.pathH -1] == PATH || map[i][room_list[container_count+1].path_front.pathY] == PATH_FRONT && wall_flag == true){
					// path‚Æpath_front‚ªŒq‚ª‚Á‚Ä‚¢‚½
					if(map[i][room_list[container_count].path.pathH -2] == PATH && map[i][room_list[container_count+1].path_front.pathY +1] == PATH_FRONT){
						break;
					}
					if(path_flag == false){
						path_flag = true;
					}else{
						path_flag = false;
						break;
					}
				}
				if(path_flag == true && wall_flag == true){
					map[i][it_room->path.pathH -1] = PATH_OF_WALL;
					map[i][it_room->path.pathH -2] = APPEND_PATH;
					map[i][it_room->path.pathH] = APPEND_PATH;
				}
			}
			path_flag = false;
			wall_flag = false;
		}
		// ’Ê˜H‚É‚·‚é‹æ‰æ‚ªc <-> ’Ê˜H‰¡
		if(it_room->path.isPathVertical == false){
			for(i = 1; i < MAP_H; i++){
				if(map[it_room->path.pathW -1][i] == WALL && map[it_room->path.pathW -1][i+1] == WALL && wall_flag == false){
					wall_flag = true;
				}
				if(map[room_list[container_count].path.pathW -1][i] == PATH || map[room_list[container_count +1].path_front.pathX][i] == PATH_FRONT && wall_flag == true){
					// path‚Æpath_front‚ªŒq‚ª‚Á‚Ä‚¢‚½
					if(map[room_list[container_count].path.pathW -2][i] == PATH && map[room_list[container_count +1].path_front.pathX +1][i] == PATH_FRONT){
						break;
					}
					if(path_flag == false){
						path_flag = true;
					}else{
						path_flag = false;
						break;
					}
				}
				if(path_flag == true && wall_flag == true){
					map[it_room->path.pathW -1][i] = PATH_OF_WALL;
					map[it_room->path.pathW -2][i] = APPEND_PATH;
					map[it_room->path.pathW][i] = APPEND_PATH;
				}
			}
			path_flag = false;
			wall_flag = false;
		}
		it_room++;
		container_count++;
	}
}