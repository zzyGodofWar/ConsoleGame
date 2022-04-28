#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>
#include <conio.h>

//Create By zzy 
int numtable[4][4];

void initializationArray();
void drawArray();
int getNumStrlen(int num);
int UlongToString(unsigned int val, char *buffer);
int Random(int x, int y);
void SpawnNum();
int isGameover();
void block_move(int direction);
void block_add(int direction);

int main()
{

	initializationArray();
	int gamestate = 1,key=0;
	SpawnNum();
	SpawnNum();
	/*
	numtable[0][0]=2;
	numtable[2][0] = 2;
	numtable[0][1] = 4;
	numtable[1][1] = 4;
	numtable[2][1] = 4;
	numtable[3][1] = 4;
	numtable[0][2] = 3;
	numtable[3][2] = 3;
	numtable[1][3] = 4;
	numtable[2][3] = 4;
	*/
	while (gamestate)
	{
		system("cls");
		drawArray();
		while (!key) {
			if (_kbhit()) {
				key = _getch();
			}
			switch (key)
			{
			default:key = 0;
				break;
			case 'w':
				block_move(1);
				block_add(1);
				block_move(1);
				break;
			case 's':
				block_move(3);
				block_add(3);
				block_move(3);
				break;
			case 'a':
				block_move(4);
				block_add(4);
				block_move(4);
				break;
			case 'd':
				block_move(2);
				block_add(2);
				block_move(2);
				break;
			}
		}
		system("cls");
		drawArray();
		key = 0;
		Sleep(100);


		gamestate=isGameover();
		if (gamestate == 0) {
			break;
		}
		/*
		if (Random(0, 12) > 9) {
			SpawnNum();
			SpawnNum();
		}
		else {
			SpawnNum();
		}
		*/
		SpawnNum();
		system("cls");
		drawArray();

		gamestate = isGameover();
		if (gamestate == 0) {
			break;
		}

	}
	printf("\n\tÓÎÏ·½áÊø\n");
	getchar();
	getchar();
	
    return 0;
}

void initializationArray() {
	for (int i = 0; i < 4; i++) {
		for (int n = 0; n < 4; n++) {
			numtable[i][n] = 0;
		}
	}
}

void drawArray() {
	int i, n;
	char tbuf[12];
	int len;
	printf("+------+------+------+------+\n");
	for (i = 0; i < 4; i++) {
		printf("|%6s|%6s|%6s|%6s|\n","","","","");
		for (n = 0; n < 4; n++) {
			if (numtable[n][i] != 0) {
				len = UlongToString(numtable[n][i], tbuf);
				printf("|");
				for (int kg = 0; kg < (6 - len) / 2; kg++)printf(" ");
				printf("%s", tbuf);
				for (int kg = 0; kg < 6 - len - (6 - len) / 2; kg++)printf(" ");
			}
			else {
				printf("|%6s","");
			}
			
		}
		printf("|\n");
		printf("|%6s|%6s|%6s|%6s|\n", "", "", "", "");
		if (i < 3) {
			printf("+------+------+------+------+\n");
		}
	}
	printf("+------+------+------+------+\n");
}

int getNumStrlen(int num) {
	int len = 1;
	int tmp = num;
	while ((tmp/=10)!=0)len++;
	return len;
}

int UlongToString(unsigned int val,char *buffer) {
	int len = 0;
	do {
		buffer[len++] = val % 10 + '0';
		val /= 10;
	} while (val);
	buffer[len]=0;

	for (int i = 0; i < len / 2;i++) {
		char tmp = buffer[i];
		buffer[i] = buffer[len -1 -i];
		buffer[len - 1 - i] = tmp;
	}
	return len;
}

int Random(int x, int y) {
	srand(time(0));
	return rand() % (y + 1 - x) + x;
}

void SpawnNum(){
	int row,col,subscript=-1;
	struct pos {
		int x;
		int y;
	}empty[16];
	for (row = 0; row < 4; row++) {
		for (col = 0; col < 4; col++) {
			if (numtable[row][col]==0) {
				subscript++;
				empty[subscript].x = row;
				empty[subscript].y = col;
			}
		}
	}
	if (subscript < 0) {
		return;
	}
	int rnd = Random(0, subscript);
	row = empty[rnd].x;
	col = empty[rnd].y;
	int _2or4 = Random(0,100);
	if( _2or4 > 55 && _2or4 < 66){
		numtable[row][col]=4;
	}else{
		numtable[row][col]=2;
	}
	
}

int isGameover(){
	int x,y,r;
	for(y=0;y<4;y++){
		for(x=0;x<4;x++){
			if(numtable[x][y]==0){
				return 1;
			}
			else {
				r = numtable[x][y];
				if (x-1 >= 0) {
					if (r == numtable[x - 1][y]) {
						return 1;
					}
				}
				if (x + 1 <= 3) {
					if (r == numtable[x + 1][y]) {
						return 1;
					}
				}
				if (y - 1 >= 0) {
					if (r == numtable[x][y-1]) {
						return 1;
					}
				}
				if (y + 1 <= 3) {
					if (r == numtable[x][y + 1]) {
						return 1;
					}
				}
			
			}
		
		}

	}

	return 0;
}

void block_move(int direction) {
	int row = 0, col = 0;
	switch (direction) 
	{
	default:
		break;
	case 1:
		for (row = 0;row < 4;row++) {
			for (col = 0;col <4;col++) {
				if (numtable[row][col] != 0 && col != 0) {
					for (int i = col;i >0;i--) {
						if (numtable[row][i-1] == 0) {
							numtable[row][i-1] = numtable[row][i];
							numtable[row][i] = 0;
						}

					}
				}

			}
		}
		break;
	case 2:
		for (col = 0;col < 4;col++) {
			for (row = 3;row >= 0;row--) {
				if (numtable[row][col]!=0 && row != 3) {
					for (int i = row;i < 3;i++) {
						if (numtable[i + 1][col]==0) {
							numtable[i + 1][col] = numtable[i][col];
							numtable[i][col] = 0;
						}
					
					}
				}

			}
		}
		break;
	case 3:
		for (row = 0;row < 4;row++) {
			for (col = 3; col >= 0;col--) {
				if (numtable[row][col] != 0 && col != 3) {
					for (int i = col;i <3;i++) {
						if (numtable[row][i + 1] == 0) {
							numtable[row][i + 1] = numtable[row][i];
							numtable[row][i] = 0;
						}

					}
				}

			}
		}
		break;
	case 4:
		for (col = 0;col < 4;col++) {
			for (row = 0;row <4;row++) {
				if (numtable[row][col] != 0 && row!=0) {
					for (int i = row;i >0;i--) {
						if (numtable[i - 1][col] == 0) {
							numtable[i - 1][col] = numtable[i][col];
							numtable[i][col] = 0;
						}

					}
				}

			}
		}
		break;
	}
	
}

void block_add(int direction) {
	int row = 0, col = 0;
	switch (direction)
	{
	default:
		break;
	case 1:
		for (row = 0;row < 4;row++) {
			for (col = 0;col <3;col++) {
				if (numtable[row][col] == numtable[row][col + 1] && numtable[row][col] != 0) {
					numtable[row][col] *= 2;
					numtable[row][col + 1] = 0;
				}

			}
		}
		break;
	case 2:
		for (col = 0;col < 4;col++) {
			for (row = 3;row > 0;row--) {
				if (numtable[row][col] == numtable[row-1][col]&& numtable[row][col]!=0) {
					numtable[row][col] *= 2;
					numtable[row - 1][col] = 0;
				}

			}
		}
		break;
	case 3:
		for (row = 0;row < 4;row++) {
			for (col = 3; col > 0;col--) {
				if (numtable[row][col] == numtable[row][col-1] && numtable[row][col] != 0) {
					numtable[row][col] *= 2;
					numtable[row][col-1] = 0;
				}
			}
		}
		break;
	case 4:
		for (col = 0;col < 4;col++) {
			for (row = 0;row <3;row++) {
				if (numtable[row][col] == numtable[row + 1][col] && numtable[row][col] != 0) {
					numtable[row][col] *= 2;
					numtable[row + 1][col] = 0;
				}

			}
		}
		break;
	}
}
