#include <Windows.h>
#include <iostream>
#include <time.h>
#include <conio.h>
using namespace std;

#define		BLOCK_NONE	0
#define		BLOCK_SET	1
#define		BLOCK_DROP	2
#define		BLOCK_EDGE	3

const char BLOCK_CHAR[] = { ' ','#','#','@' };

ULONG GAME_WIDTH = 10;
ULONG GAME_HEIGHT = 18;

ULONG DISPLAY_WIDTH = GAME_WIDTH + 2, DISPLAY_HEIGHT = GAME_HEIGHT + 2;

typedef struct BLOCK_DATA {
	int blockWidth;
	int blockHeight;
	char blockShapData[4][4];
}BLOCK_DATA;

typedef struct SOURCE_BLOCK_DATA {
	int blockDirect;
	char blockShapData[16];
	BLOCK_DATA *blockFullData;
}SOURCE_BLOCK_DATA;

SOURCE_BLOCK_DATA DROPBLOCK_CHARDATA[] = {
	{2,
	{ 
		0,0,0,0 ,
		0,0,0,0 ,
		0,0,0,0 ,
		1,1,1,1  
	}
	},

	{4,
	{
		0,0,0,0 ,
		0,0,0,0 ,
		0,1,0,0 ,
		1,1,1,0
	}
	},

	{4,
	{
		0,0,0,0 ,
		0,0,0,0 ,
		1,0,0,0 ,
		1,1,1,0
	}
	},

	{ 4,
	{
		0,0,0,0 ,
		0,0,0,0 ,
		0,0,1,0 ,
		1,1,1,0
	}
	},

	{1,
	{
		0,0,0,0 ,
		0,0,0,0 ,
		1,1,0,0 ,
		1,1,0,0
	}
	},

	{2,
	{
		0,0,0,0 ,
		0,0,0,0 ,
		1,1,0,0 ,
		0,1,1,0
	}
	},

	{ 2,
	{
		0,0,0,0 ,
		0,0,0,0 ,
		0,1,1,0 ,
		1,1,0,0
	}
	}
};


int GAME_SCORE = 0;

int newdrop = 0;
int dropblock_way, dropblock_type, dropblock_x, dropblock_y;

char **display_data = NULL;
char **game_data = NULL;
void CreateGameData();
void SetDataToDisplay();
int GameLoopCheck();
void SetBlockToData();
void DisplayGameData();

int GetRandNum(int low, int high);

void CreateDropBlock();
int CheckDropCrash();
int CheckLeftCrash();
int CheckRightCrash();



void KeyInput() {
	int keycode;
	int begintime = 0;
	int fastDown = 0;
	int nextWay;
	int wayValid;
	while (true) {
		if (!_kbhit()) {
			break;
		}
		keycode = _getch();
		switch (keycode)
		{
		case 'a':
			if (!CheckLeftCrash()) {
				dropblock_x--;
			}
			break;
		case 'd':
			if (!CheckRightCrash()) {
				dropblock_x++;
			}
			break;
		case 's':
			if (dropblock_y >= 0) {
				fastDown = 1;
				begintime = 200;
			}
			break;
		case 'w':
			nextWay = dropblock_way+1;
			if (nextWay >=DROPBLOCK_CHARDATA[dropblock_type].blockDirect){
				nextWay = 0;
			}
			wayValid = 1;


			for (int blockline = 3; dropblock_y - 3 + blockline < GAME_HEIGHT && blockline >= 4 - DROPBLOCK_CHARDATA[dropblock_type].blockFullData[nextWay].blockHeight; blockline--) {
				for (int blockwidth = DROPBLOCK_CHARDATA[dropblock_type].blockFullData[nextWay].blockWidth - 1; blockwidth >= 0; blockwidth--) {
					
					if (dropblock_x + blockwidth >= GAME_WIDTH) {
						wayValid = 0;
						break;
					}
					if (DROPBLOCK_CHARDATA[dropblock_type].blockFullData[nextWay].blockShapData[blockline][blockwidth] && game_data[dropblock_y - 3 + blockline][dropblock_x + blockwidth]) {
						wayValid = 0;
						break;
					}
				}
				if (!wayValid) {
					break;
				}
			}
			if (wayValid) {
				begintime += 100;
				dropblock_way = nextWay;
			}
			break;
		default:
			break;
		}
		Sleep(20);
		begintime++;
		if (begintime>200) {
			break;
		}

	}
	if (fastDown) {
		while (!CheckDropCrash()) {
			dropblock_y++;
		}
	}
}

void testmove() {
	if (CheckDropCrash()) {
		SetBlockToData();
		CreateDropBlock();
	}
	else {
		dropblock_y++;
	}
}

void InitBlockData() {
	int maxWidth = 0;
	int maxHeight = 0;
	int tmpWidth = 0;
	for (int fi = 0; fi < sizeof(DROPBLOCK_CHARDATA) / sizeof(SOURCE_BLOCK_DATA); fi++) {
		DROPBLOCK_CHARDATA[fi].blockFullData = new BLOCK_DATA[DROPBLOCK_CHARDATA[fi].blockDirect]();
		for (int fn = 0; fn < DROPBLOCK_CHARDATA[fi].blockDirect; fn++) {
			maxWidth = 0;
			maxHeight = -1;
			tmpWidth = 0;
			switch (fn) {
			case 0:
				for (int lfi = 0; lfi < 4; lfi++) {
					tmpWidth = 0;
					for (int lfn = 0; lfn < 4; lfn++) {
						DROPBLOCK_CHARDATA[fi].blockFullData[fn].blockShapData[lfi][lfn] = DROPBLOCK_CHARDATA[fi].blockShapData[lfi * 4 + lfn];
						if (DROPBLOCK_CHARDATA[fi].blockFullData[fn].blockShapData[lfi][lfn]) {
							tmpWidth = lfn + 1;
							if (maxHeight < 0) {
								maxHeight = 4 - lfi;
							}
						}
					}
					if (tmpWidth > maxWidth) {
						maxWidth = tmpWidth;
					}
				}
				DROPBLOCK_CHARDATA[fi].blockFullData[fn].blockWidth = maxWidth;
				DROPBLOCK_CHARDATA[fi].blockFullData[fn].blockHeight = maxHeight;

				break;
			case 1:
				for (int lfi = 3; lfi >= 4 - DROPBLOCK_CHARDATA[fi].blockFullData[0].blockWidth; lfi--) {
					tmpWidth = 0;
					for (int lfn = 0; lfn < DROPBLOCK_CHARDATA[fi].blockFullData[0].blockHeight; lfn++) {

						DROPBLOCK_CHARDATA[fi].blockFullData[fn].blockShapData[lfi][lfn] =
							DROPBLOCK_CHARDATA[fi].blockShapData[(3 - lfn) * 4 + DROPBLOCK_CHARDATA[fi].blockFullData[0].blockWidth - 4 + lfi];
						if (DROPBLOCK_CHARDATA[fi].blockFullData[fn].blockShapData[lfi][lfn]) {
							tmpWidth = lfn + 1;
							maxHeight = 4 - lfi;
						}
					}
					if (tmpWidth > maxWidth) {
						maxWidth = tmpWidth;
					}
				}

				DROPBLOCK_CHARDATA[fi].blockFullData[fn].blockWidth = maxWidth;
				DROPBLOCK_CHARDATA[fi].blockFullData[fn].blockHeight = maxHeight;
				break;
			case 2:
				for (int lfi = 3; lfi >= 4 - DROPBLOCK_CHARDATA[fi].blockFullData[0].blockHeight; lfi--) {
					tmpWidth = 0;
					for (int lfn = 0; lfn < DROPBLOCK_CHARDATA[fi].blockFullData[0].blockWidth; lfn++) {

						DROPBLOCK_CHARDATA[fi].blockFullData[fn].blockShapData[lfi][lfn] =
							DROPBLOCK_CHARDATA[fi].blockShapData[(4 - DROPBLOCK_CHARDATA[fi].blockFullData[0].blockHeight + 3 - lfi) * 4 + DROPBLOCK_CHARDATA[fi].blockFullData[0].blockWidth - 1 - lfn];
						if (DROPBLOCK_CHARDATA[fi].blockFullData[fn].blockShapData[lfi][lfn]) {
							tmpWidth = lfn + 1;
							maxHeight = 4 - lfi;
						}
					}
					if (tmpWidth > maxWidth) {
						maxWidth = tmpWidth;
					}
				}

				DROPBLOCK_CHARDATA[fi].blockFullData[fn].blockWidth = maxWidth;
				DROPBLOCK_CHARDATA[fi].blockFullData[fn].blockHeight = maxHeight;

				break;
			case 3:
				for (int lfi = 3; lfi >= 4 - DROPBLOCK_CHARDATA[fi].blockFullData[0].blockWidth; lfi--) {
					tmpWidth = 0;
					for (int lfn = 0; lfn < DROPBLOCK_CHARDATA[fi].blockFullData[0].blockHeight; lfn++) {

						DROPBLOCK_CHARDATA[fi].blockFullData[fn].blockShapData[lfi][lfn] =
							DROPBLOCK_CHARDATA[fi].blockShapData[(4 - DROPBLOCK_CHARDATA[fi].blockFullData[0].blockHeight + lfn) * 4 + 3 - lfi];
						if (DROPBLOCK_CHARDATA[fi].blockFullData[fn].blockShapData[lfi][lfn]) {
							tmpWidth = lfn + 1;
							maxHeight = 4 - lfi;
						}
					}
					if (tmpWidth > maxWidth) {
						maxWidth = tmpWidth;
					}
				}

				DROPBLOCK_CHARDATA[fi].blockFullData[fn].blockWidth = maxWidth;
				DROPBLOCK_CHARDATA[fi].blockFullData[fn].blockHeight = maxHeight;
				break;
			}
		}
	}
}

int main() {

	int timeSamp = 0;
	srand(time(0));

	InitBlockData();

	CreateGameData();
	CreateDropBlock();
	while (true) {
		system("cls");
		printf("·ÖÊý£º%d\n", GAME_SCORE);
		if (timeSamp % 3 == 0) {
			KeyInput();
		}
		if (timeSamp>9) {
			timeSamp = 0;
			testmove();
		}
		GameLoopCheck();
		SetDataToDisplay();
		DisplayGameData();
		Sleep(20);
		timeSamp++;
	}
}

int GameLoopCheck() {
	int setfull = 0;
	char **old_data;
	char **newgame_data = new char*[GAME_HEIGHT];
	for (int fi = GAME_HEIGHT - 1,fn = GAME_HEIGHT - 1; fi >= 0; fi--) {
		newgame_data[fi] = new char[GAME_WIDTH]();
		setfull = 0;
		for (int fm = 0; fm < GAME_WIDTH;fm++) {
			newgame_data[fn][fm] = game_data[fi][fm];
			if (newgame_data[fn][fm]) {
				setfull++;
			}
		}
		if (setfull < GAME_WIDTH) {
			fn--;
		}
		else {
			GAME_SCORE++;
		}
	}
	old_data = game_data;
	game_data = newgame_data;
	delete[]old_data;
	return 1;
}

int CheckDropCrash() {
	int tmpY = dropblock_y + 1;
	if (tmpY >= GAME_HEIGHT) {
		return 1;
	}

	for (int blockline = 3; tmpY >=0 && blockline >= 4 - DROPBLOCK_CHARDATA[dropblock_type].blockFullData[dropblock_way].blockHeight; tmpY-- && blockline--) {
		for (int blockwidth = DROPBLOCK_CHARDATA[dropblock_type].blockFullData[dropblock_way].blockWidth - 1; blockwidth >= 0; blockwidth--) {
			if (DROPBLOCK_CHARDATA[dropblock_type].blockFullData[dropblock_way].blockShapData[blockline][blockwidth] && game_data[tmpY][dropblock_x + blockwidth]) {
				return 1;
			}
		}
	}

	return 0;
}

int CheckLeftCrash() {
	int tmpX = dropblock_x - 1;
	if (tmpX < 0) {
		return 1;
	}
	for (int blockline = 0; blockline <= 3 && dropblock_y - blockline >= 0; blockline++) {
		if (game_data[dropblock_y - blockline][tmpX] && DROPBLOCK_CHARDATA[dropblock_type].blockFullData[dropblock_way].blockShapData[3 - blockline][0]) {
			return 1;
		}
	}
	return 0;
}

int CheckRightCrash() {
	int tmpX = dropblock_x + DROPBLOCK_CHARDATA[dropblock_type].blockFullData[dropblock_way].blockWidth;
	if (tmpX >= GAME_WIDTH) {
		return 1;
	}
	for (int blockline = 0; blockline <= 3 && dropblock_y - blockline >= 0; blockline++) {
		if (game_data[dropblock_y - blockline][tmpX] && 
			DROPBLOCK_CHARDATA[dropblock_type].blockFullData[dropblock_way].blockShapData[3 - blockline][DROPBLOCK_CHARDATA[dropblock_type].blockFullData[dropblock_way].blockWidth-1]) {
			return 1;
		}
	}
	return 0;
}

void SetBlockToData() {
	for (int blockline = 3, tmpY = dropblock_y; tmpY >= 0 && blockline >= 0; tmpY--, blockline--) {
		for (int i = 0; i < DROPBLOCK_CHARDATA[dropblock_type].blockFullData[dropblock_way].blockWidth; i++) {
			if (DROPBLOCK_CHARDATA[dropblock_type].blockFullData[dropblock_way].blockShapData[blockline][i]) {
				game_data[tmpY][dropblock_x+i] = BLOCK_SET;
			}
		}
	}
}

void CreateDropBlock() {
	dropblock_type = GetRandNum(0, sizeof(DROPBLOCK_CHARDATA) / sizeof(SOURCE_BLOCK_DATA) - 1);
	dropblock_way = GetRandNum(0, DROPBLOCK_CHARDATA[dropblock_type].blockDirect - 1);
	dropblock_x = (GAME_WIDTH - 4) / 2;
	dropblock_y = -1;
	
}

void CreateGameData() {
	if (display_data) {
		delete[]display_data;
	}
	if (game_data) {
		delete[]game_data;
	}
	display_data = new char*[DISPLAY_HEIGHT]();
	game_data = new char*[GAME_HEIGHT]();
	for (int fi = 0; fi < DISPLAY_HEIGHT; fi++) {
		display_data[fi] = new char[DISPLAY_WIDTH]();
		if (fi < GAME_HEIGHT) {
			game_data[fi] = new char[GAME_WIDTH]();
		}
	}
}

void SetDataToDisplay() {
	for (int fi = 0; fi <DISPLAY_HEIGHT; fi++) {
		for (int fn = 0; fn < DISPLAY_WIDTH; fn++) {
			if (fn == 0 || fn == DISPLAY_WIDTH - 1 || fi == 0 || fi == DISPLAY_HEIGHT - 1) {
				display_data[fi][fn] = BLOCK_EDGE;
			}
			if (fi<GAME_HEIGHT && fn < GAME_WIDTH) {
				display_data[fi+1][fn+1] = game_data[fi][fn];
			}
		}
	}

}

void DisplayGameData() {
	char displaycode;
	int dropShowY= 0,dropShowX = 0;
	for (int fi = 0; fi <DISPLAY_HEIGHT ; fi++) {
		for (int fn = 0; fn <DISPLAY_WIDTH; fn++) {
			dropShowY = 0;
			dropShowX = 0;
			displaycode = BLOCK_CHAR[display_data[fi][fn]];
			if (!display_data[fi][fn]) {
				if (fi && dropblock_x + 1 <= fn && fn <= dropblock_x + DROPBLOCK_CHARDATA[dropblock_type].blockFullData[dropblock_way].blockWidth && dropblock_y - 2 <= fi && fi <= dropblock_y + 1) {
					dropShowY = fi - dropblock_y + 2;
					dropShowX = fn - dropblock_x - 1;
					displaycode = BLOCK_CHAR[DROPBLOCK_CHARDATA[dropblock_type].blockFullData[dropblock_way].blockShapData[dropShowY][dropShowX]];
				}
			}
			
			printf("%c%c", displaycode, displaycode);
		}
		printf("\n");
	}
}

int GetRandNum(int low, int high) {
	return rand()%(high-low+1) + low;
}

