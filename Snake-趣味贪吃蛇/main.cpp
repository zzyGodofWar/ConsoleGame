

#include <stdio.h>//使用 printf 打印函数
#include <stdlib.h>//使用 rand 取随机数函数
#include <Windows.h>//使用 Sleep 延迟函数 以及 system 命令
#include <conio.h>//使用 getch 获取按键函数
#include <time.h>

//定义游戏的宽和高
#define MAP_WIDTH 38
#define MAP_HEIGHT 24

struct snakeblock {
	int x;
	int y;
};

/* 定义游戏二维数组（将地图=1 蛇头=2 蛇身=3 和 食物=4 保存在数组中）
定义游戏速度等全局变量 */
int map[MAP_WIDTH][MAP_HEIGHT];
struct snakeblock p_snake[(MAP_WIDTH-2)*(MAP_HEIGHT-2)];
int GAME_SPEED = 80;
int Snake_direction = 2;//蛇前进方向 上 1 右 2 下 3 左 4
int score = 1;

//设置道具出现概率
int MisteryProbability = 15;

/*声明自己的函数
初始化地图 画地图 初始化蛇头*/
void initializationMap();
void drawMap();
void initializationSnake();
void drawSnaketoMap();
void moveSnake();
int getGameState();
void randomFood();
void randomMystery();
int Random(int x, int y);
int isMapHavePickUp();
void printfscore();
bool IsNextMystery();

int main()
{
	srand(time(0));

	do {

	

	int gaming = 0,key=0,death = 0;
	initializationMap();
	initializationSnake();

	system("cls");
	printf("\t欢迎来到趣味贪吃蛇\n");
	printf("\t按下任意键开始游戏\n");
	while (!key) {
		if (_kbhit()) {
			key = _getch();
		}
	}

	gaming = 1;
	score = 0;
	while (gaming)
	{
		system("cls");
		drawMap();
		printfscore();
		if (!isMapHavePickUp()) {
			if (IsNextMystery()) {
				randomMystery();
			}
			else {
				randomFood();
			}
		}
		
		Sleep(GAME_SPEED);
		if (_kbhit()) {
			key = _getch();
			switch (key) {
			case 75:
			case 'a':	//left key A
				if (Snake_direction != 2) {
					Snake_direction = 4;
				}
				break;
			case 72:
			case 'w':	//up key W
				if (Snake_direction != 3) {
					Snake_direction = 1;
				}
				break;
			case 77:
			case 'd':	//right key D
				if (Snake_direction != 4) {
					Snake_direction = 2;
				}
				break;
			case 80:
			case 's':	//down key S
				if (Snake_direction != 1) {
					Snake_direction = 3;
				}
				break;
			}
		}
		moveSnake();
		death = getGameState();
		if(death!=0)gaming = 0;
		drawSnaketoMap();
	}
	system("cls");
	printf("\n\tGAME OVER\n\t游戏结束\n");
	switch (death)
	{
	case 1:
		printf("\n\t脑袋撞墙，头破蛇亡\n");
		break;
	case 2:
		printf("\n\t蛇肉真好吃，失血过多而亡\n");
		break;
	case 3:
		printf("\n\t你已通关！\n");
		break;
	}
	
	printf("\t得分：%d\n",score);

	printf("\n\t按回车键重新开始\n", score);
	getchar();

	} while (true);

    return 0;
}


int Random(int x, int y) {
	return rand() % (y + 1 - x) + x;
}

void initializationMap() {
	int x, y;
	for (x = 0; x < MAP_WIDTH; x++) {
		for (y = 0; y < MAP_HEIGHT; y++) {
			if ((x == 0||x == MAP_WIDTH-1)){
				map[x][y] = 1;
			}
			else if(y==0 || y == MAP_HEIGHT -1){
				map[x][y] = 1;
			}
			else {
				map[x][y] = NULL;
			}
		}
	}
}

void drawMap() {
	int x, y;
	for (y = 0; y < MAP_HEIGHT; y++) {
		for (x = 0; x < MAP_WIDTH; x++) {
			switch (map[x][y])
			{
				case 1:
					printf("□");
					break;
				case 2:
					printf("◎");
					break;
				case 3:
					printf("○");
					break;
				case 4:
					printf("●");
					break;
				case 5:
					printf("？");
					break;
				default:
					printf("  ");
			}
		}
		printf("\n");
	}
}

void initializationSnake() {
	p_snake[0].x=MAP_WIDTH / 2;
	p_snake[0].y = MAP_HEIGHT / 2;
	for (int i = 1; i < (MAP_WIDTH - 2)*(MAP_HEIGHT - 2); i++) {
		p_snake[i].x = -1;
		p_snake[i].y = -1;
	}
	for (int i = 1; i < 3; i++) {
		p_snake[i].x = p_snake[i-1].x - 1;
		p_snake[i].y = p_snake[0].y;

	}
}

void drawSnaketoMap() {
	int i = 1;
	int x, y;
	for (y = 0; y < MAP_HEIGHT; y++) {
		for (x = 0; x < MAP_WIDTH; x++) {
			if (map[x][y] == 2 || map[x][y] == 3) {
				map[x][y] = NULL;
			}
		}
	}
	map[p_snake[0].x][p_snake[0].y]=2;
	while (p_snake[i].x!=-1) {
		map[p_snake[i].x][p_snake[i].y]=3;
		i++;
	}
}

void moveSnake() {
	int hx = 1,hy = 1;
	int add=-1,i;

	for (i = (MAP_WIDTH - 2)*(MAP_HEIGHT - 2)-1; i > 0; i--) {
		if (p_snake[i].x != -1) {
			if (add == -1) {
				add = i;
				hx = p_snake[i].x;
				hy = p_snake[i].y;
			}
			p_snake[i].x = p_snake[i - 1].x;
			p_snake[i].y = p_snake[i - 1].y;
		}
	}

	if (add == -1) {
		return;
	}

	switch (Snake_direction) {
	case 1:
		p_snake[0].y--; break;
	case 2:
		p_snake[0].x++; break;
	case 3:
		p_snake[0].y++; break;
	case 4:
		p_snake[0].x--; break;
	}

	if (map[p_snake[0].x][p_snake[0].y] == 4) {
		score = score + 1;
		for (i = add; i < (MAP_WIDTH - 2)*(MAP_HEIGHT - 2); i++) {
			if (p_snake[i].x == -1) {
				p_snake[i].x = hx;
				p_snake[i].y = hy;
				break;
			}
		}
	}
	else if (map[p_snake[0].x][p_snake[0].y] == 5) {
		int v = Random(1, 100);
		if (v % 10 >= 5) {
			randomFood();
			randomFood();
			randomFood();
		}
		else if (v % 10 >= 2) {
			score += 10;
		}
		else {
			for (i = add; i >= add/2; i--) {
				p_snake[i].x = p_snake[i].y = -1;
			}
		}
	}
}

int getGameState() {
	if (map[p_snake[0].x][p_snake[0].y] == 1) {
		return 1;
	}
	for (int i = 1; i < (MAP_WIDTH - 2)*(MAP_HEIGHT - 2); i++) {
		if (p_snake[0].x == p_snake[i].x && p_snake[0].y==p_snake[i].y) {
			return 2;
		}
	}
	if (p_snake[(MAP_WIDTH - 2)*(MAP_HEIGHT - 2) - 1].x != -1) {
		return 3;
	}
	return 0;
}

void randomFood() {
	if (p_snake[(MAP_WIDTH - 2)*(MAP_HEIGHT - 2) - 1].x != -1) {
		return;
	}
	int fx, fy; 
	do {
		fx= Random(1, MAP_WIDTH - 2);
		fy = Random(1, MAP_HEIGHT - 2);
	} while (map[fx][fy] != NULL);
	map[fx][fy]=4;
}

void randomMystery() {
	if (p_snake[(MAP_WIDTH - 2)*(MAP_HEIGHT - 2) - 1].x != -1) {
		return;
	}
	int fx, fy;
	do {
		fx = Random(1, MAP_WIDTH - 2);
		fy = Random(1, MAP_HEIGHT - 2);
	} while (map[fx][fy] != NULL);
	map[fx][fy] = 5;
}

bool IsNextMystery() {
	int key = Random(1,100);
	if (key <= MisteryProbability) {
		return true;
	}
	return false;
}

int isMapHavePickUp() {
	int x, y;
	for (y = 1; y < MAP_HEIGHT-1; y++) {
		for (x = 1; x < MAP_WIDTH-1; x++) {
			if (map[x][y] == 4 || map[x][y] == 5) {
				return 1;
			}
		}
	}
	return 0;
}

void printfscore() {
	printf("当前分数:%d", score);
}
