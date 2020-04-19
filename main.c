//author:   Hengda
//date:		2020年4月19日
//编译环境：Visual Studio 2019
#include <stdio.h>//引入所需头文件
#include <string.h>//字符串
#include <malloc.h>
#include <stdlib.h>
#include <conio.h>
#include <math.h>

#define SPAN_WIDTH 48 //蛇可运动区域宽度
#define SPAN_HEIGHT 32 //蛇可运动区域高度
#define GAME_WIDTH SPAN_WIDTH+2+1 //画面宽度（+2是两边围墙 +1是行尾换行符）
#define GAME_HEIGHT SPAN_HEIGHT+2 //画面高度（+2是上下围墙）
#define SPEED_LEVEL_DIFF_SPEED 100 //速度等级之间的的速度差

typedef struct body_node{ int x; int y; struct body_node *next;} BODY_NODE;
char spanArr[GAME_HEIGHT][GAME_WIDTH] = { ' '};// 存储整个画面
BODY_NODE* snakeP = NULL,apple = { 0,0,NULL };//蛇头指针，苹果
char appleImage = 'O',snakeImage = '@',emptyImage=' ', direction = 'r';//苹果图标，蛇图标，空区域图标，默认运动方向（l r u d）
unsigned int score = 0;

char info[9][60] = {
	"GAME OVER !!!",
	"得分：%d ",
	"《贪吃蛇游戏》",
	"C语言控制台字符界面版",
	"作者：Hengda",
	"日期：2020/04/19",
	"版权所有",
	"1.按Enter键继续游戏",
	"2.按其他键退出"
};

void setSpanCharByXY(int x, int y, char c) { spanArr[y + 1][x + 1] = c; }//+1原因是有边框

char getSpanCharByXY(int x, int y) { return spanArr[y + 1][x + 1]; }//+1原因是有边框

void init_sanke(){//生成蛇
	snakeP = (BODY_NODE*)malloc(sizeof(BODY_NODE));//初始蛇头位置
	snakeP->x = SPAN_WIDTH / 2;
	snakeP->y = SPAN_HEIGHT / 2;
	snakeP->next = NULL;
	setSpanCharByXY(snakeP->x, snakeP->y, snakeImage);
}

void free_snake() {//释放蛇所占内存
	BODY_NODE* P=snakeP,*Q = NULL;
	while (P->next != NULL) {
		Q = P;
		P = P->next;
		free(Q);
		Q = NULL;
	}
	free(P);
	P = NULL;
}

void init_apple() {//生成随机且不与蛇重叠的Apple
	apple.x = -1;
	apple.y = -1;
	while (apple.x < 0 || apple.y < 0 || getSpanCharByXY(apple.x, apple.y) == '#') {
		apple.x = rand()% SPAN_WIDTH;
		apple.y = rand()% SPAN_HEIGHT;
	}
	setSpanCharByXY(apple.x, apple.y, appleImage);
}

void eateAppleOrAddHead(int appleX, int appleY) {//吃苹果
	BODY_NODE* node = (BODY_NODE*)malloc(sizeof(BODY_NODE));
	node->x = appleX;
	node->y = appleY;
	node->next = snakeP;
	snakeP = node;
	setSpanCharByXY(appleX, appleY, snakeImage);//碰到苹果，则把苹果变成自己的身体
}

void popTail() {//去尾
	BODY_NODE * P=snakeP;
	if (P == NULL || P->next == NULL) return;//蛇不存在或者蛇只有一节，不做删尾操作
	while(P->next->next != NULL) {P = P->next;};
	setSpanCharByXY(P->next->x, P->next->y, emptyImage);
	free(P->next);
	P->next = NULL;
}

int snake_Step(int nextX, int nextY) {//蛇移动一步
	//碰到自己的身体，或者越界则gameover //nextX >= SPAN_WIDTH || nextY >= SPAN_HEIGHT || nextX < 0 || nextY < 0 ||
	if (getSpanCharByXY(nextX, nextY) == snakeImage) return 0;
	else if (getSpanCharByXY(nextX, nextY) == appleImage) {
		eateAppleOrAddHead(nextX, nextY);
		score++;
		init_apple();//生成新苹果
	}else {
		eateAppleOrAddHead(nextX, nextY);
		popTail();
	}
	return 1;
}

int snake_Move_By_Drection(char direction) {//蛇朝指定方向移动
	if (direction == 'l') return snake_Step(snakeP->x - 1, snakeP->y);
	else if (direction == 'u') return snake_Step(snakeP->x, snakeP->y - 1);
	else if (direction == 'r') return snake_Step(snakeP->x + 1, snakeP->y);
	else if(direction == 'd') return snake_Step(snakeP->x, snakeP->y + 1);
	return 0;
}

char get_direction() {//获取方向 UP(向上方向键)为: 0X4800 DOWN(向下方向键)为 : 0X5000 LEFT(向左方向键)为 : 0X4b00 RIGHT(向右方向键)为 : 0X4d00
	if ( kbhit() && getch()) {
		switch (getch()) {
			case 0x4b: if(direction != 'r') direction = 'l'; break;
			case 0x48: if (direction != 'd') direction = 'u'; break;
			case 0x4d: if (direction != 'l') direction = 'r'; break;
			case 0x50: if (direction != 'u')direction = 'd'; break;
		}
	}
	return direction;
}

void display(){//更新画面
	system("cls");//mac 用这个 system("clear");
	spanArr[GAME_HEIGHT -1][GAME_WIDTH -1] = '\0';
	printf("%s", spanArr);//打印画面
}

void int_span(){// 清盘
	for (int j = 0; j < GAME_HEIGHT; j++)
		for (int i = 0; i < GAME_WIDTH; i++)
			if (i == 0 || (j == 0 && i != GAME_WIDTH - 1) || i == GAME_WIDTH - 2 || j == GAME_HEIGHT - 1) { spanArr[j][i] = snakeImage; }
				else if (i == GAME_WIDTH - 1) { spanArr[j][i] = '\n'; }
					else { spanArr[j][i] = emptyImage; }
}

void display_Info() {//打印信息
	int gameWidth = GAME_WIDTH;
	int gameHeight = GAME_HEIGHT;
	int infoNum = 0;
	char str[60] = "";
	infoNum = (sizeof(info)/ 60)/sizeof(char);
	for (int i = 0; i < infoNum;i++) {
		if (i == 1) sprintf(str,info[i],score);
		else strcpy(str, info[i]);
		memcpy(spanArr[gameHeight / 2 - infoNum + i*2] + gameWidth / 2 - strlen(str) / 2, str, strlen(str));
	}
}

int main(int argc, char* argv[]) {//主函数
	char info[10] = "";
	int gameWidth = GAME_WIDTH;
	int gameHeight = GAME_HEIGHT;
	while (1) {
		score = 0;
		int_span();
		init_sanke();
		init_apple();
		while (1) {
			display();//刷新棋盘
			if (!snake_Move_By_Drection(get_direction())) {//读取运动方向 并控制移动一步
				free_snake();
				display_Info();
				score = 0;
				display();//刷新有些画面
				if (getchar() == '\n') break;//等待按键
				else return 0;
			}
		}
	}
	return 0;
}