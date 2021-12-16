#include <stdio.h>
#include <iostream>
#include <time.h>
#include <conio.h>
#include <windows.h>
#include<ctype.h>

using namespace std;

const int  NONE = 0;      //空的座標標識   " "
const int  SNAKE = 1;     //蛇的座標標識   "O"
const int  FOOD = 2;      //食物的座標標識 "$"
const int  BLOCK = 3;     //牆的座標標識   "X"
const int _weigh = 25;    //預設寬度
const int _heigh = 18;    //預設高度
int foodExist = 0;        //食物是否存在
char pressKey = 'w';      //紀錄鍵盤(預設往上跑)

typedef struct parameter {
	int x[(_weigh - 1) * (_heigh - 1)];
	int y[(_weigh - 1) * (_heigh - 1)];
	int len;
};
struct parameter food;
struct parameter snake;

void gotoPoint(int x, int y){                                   //直接在任意位置輸出字元
	COORD coord;
	coord.X = x;
	coord.Y = y;
	HANDLE ConsoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);	    //獲取控制台緩衝區句柄
	SetConsoleCursorPosition(ConsoleHandle, coord);            	//設置光標位置
}

void reset() {                                                  //不要讓光標停留在遊戲畫面上
	gotoPoint(0, _heigh + 5);
}

void Welcome() {                                                //歡迎
	cout << "Welcome to Snake Game ! " << endl;
	cout << "Please Use WASD to control direction ! (不要用注音輸入法)" << endl;
	system("pause");                                            //cmd中打pause
}

void drawIcon(int type, int x, int y) {                         //依照給的參數畫出圖案
	char icon = '?';
	if (type == NONE)icon = ' ';                                //空格(讓蛇尾端消失)
	if (type == SNAKE)icon = 'O';                               //蛇的身體
	if (type == FOOD)icon = '$';                                //食物
	if (type == BLOCK)icon = 'X';                               //牆壁
	gotoPoint(x, y);
	cout << icon;
}

void setMap() {                                                  //畫出牆壁
	for (int h = 1; h <= _heigh + 1; h++) {
		if (h == 1 || h == _heigh + 1) {                         //起始是1&結尾+1是因為最上面要預留遊戲時間
			for (int w = 0; w <= _weigh; w++) {
				drawIcon(BLOCK, w, h);
			}
			cout << endl;
		}
		else {
			drawIcon(BLOCK, 0, h);
			drawIcon(BLOCK, _weigh, h);
		}
	}
	reset;
}

void createSnake() {                                              //初始蛇
	int startSnakeofX = _weigh / 2 + 1;
	int startSnakeofY = _heigh / 2 + 1;
	snake.len = 2;
	snake.x[0] = startSnakeofX;
	snake.y[0] = startSnakeofY;
	snake.x[1] = startSnakeofX;
	snake.y[1] = startSnakeofY + 1;
	for (int i = 0; i < snake.len; i++){
		drawIcon(SNAKE, snake.x[i], snake.y[i]);
	}
	reset();
}

void createFood() {                                              //造食物
	if (foodExist == 0) {
		int overlapping = 0;
		do {
			srand(time(NULL));
			food.x[0] = rand() % (_weigh - 1) + 1;
			food.y[0] = rand() % (_heigh -1) + 2;
			overlapping = 0;
			for (int i = 0; i < snake.len; i++) {                //檢查造出來的有沒有跟身體重疊
				if (snake.x[i] == food.x[0] && snake.y[i] == food.y[0]) {
					overlapping = 1;
					break;
				}
			}
		} while (overlapping == 1);
	}
	drawIcon(FOOD, food.x[0], food.y[0]);                        
	foodExist = 1;
	reset();
}

void snakeMove(int x, int y) {                                   //不管有沒有吃到，頭都+1
	if (foodExist) {                                             //食物還存在->沒吃到
		drawIcon(NONE, snake.x[snake.len - 1], snake.y[snake.len - 1]);//沒吃到->尾巴-1
	}
	else snake.len++;                                            //吃到->尾巴不減
	for (int i = snake.len - 1; i > 0; i--) {                    //全部往前1格
		snake.x[i] = snake.x[i - 1];
		snake.y[i] = snake.y[i - 1];
	}
	snake.x[0] = x;                                              //做出新的頭
	snake.y[0] = y;
	drawIcon(SNAKE, x, y);
	reset;
}

void detectKeyboard() {
	int x, y;
	char previousKey = pressKey;
	if (_kbhit()) {                                             //如果按下了鍵盤的某個鍵
		fflush(stdin);                                          //清空緩衝區的字元
		char tmpKey = _getch();
		pressKey = tolower(tmpKey);                             //大寫轉小寫
	}
	if (pressKey == 'w' && previousKey == 's')pressKey = 's';   //不能往回跑
	if (pressKey == 's' && previousKey == 'w')pressKey = 'w';
	if (pressKey == 'a' && previousKey == 'd')pressKey = 'd';
	if (pressKey == 'd' && previousKey == 'a')pressKey = 'a';
	switch (pressKey) {
	case 'w'://上
		x = snake.x[0];
		y = snake.y[0] - 1;
		break;
	case 'a'://左
		x = snake.x[0] - 1;
		y = snake.y[0];
		break;
	case 's'://下
		x = snake.x[0];
		y = snake.y[0] + 1;
		break;
	case 'd'://右
		x = snake.x[0] + 1;
		y = snake.y[0];
		break;
	}
	if (x == food.x[0] && y == food.y[0])foodExist = 0;       //吃到食物->食物不存在
	snakeMove(x, y);
}

void check() {                                               //看遊戲有沒有照規則走
	if (snake.x[0] == 0 || snake.x[0] == _weigh || snake.y[0] == 1 || snake.y[0] == _heigh+1 ){//撞牆
		gotoPoint(0, _heigh + 2);                            //讓光標移到下面
		printf("Game Over!\n");
		exit(0);                                             //結束程式
	}
	for (int i = 1; i < snake.len; i++){                      //自撞自
		if (snake.x[0] == snake.x[i] && snake.y[0] == snake.y[i]){
			gotoPoint(0, _heigh + 2);
			printf("Game Over!\n");
			exit(0);
		}
	}
}

int main()
{
	clock_t start, end;
	int diff;
	Welcome();
	system("cls");                  //cmd中打cls(清畫面)
	setMap();
	createSnake();
	start = clock();               //[程式執行] 到 [遊戲開始前] 的時間
	while (1) {
		end = clock();             //[程式執行] 到 [遊戲開始後] 的時間
		diff = (end - start) / 1000;//相減 = 遊戲執行的時間
		gotoPoint(0, 0);
		cout << "--Your length " << snake.len << " ，Game time " << diff << " s--" << endl;
		createFood();
		detectKeyboard();
		Sleep(200);                 // 用於控制遊戲速度
		check();
	}
}
