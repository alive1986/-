//author:   Hengda
//date:		2020��4��19��
//���뻷����Visual Studio 2019
#include <stdio.h>//��������ͷ�ļ�
#include <string.h>//�ַ���
#include <malloc.h>
#include <stdlib.h>
#include <conio.h>
#include <math.h>

#define SPAN_WIDTH 48 //�߿��˶�������
#define SPAN_HEIGHT 32 //�߿��˶�����߶�
#define GAME_WIDTH SPAN_WIDTH+2+1 //�����ȣ�+2������Χǽ +1����β���з���
#define GAME_HEIGHT SPAN_HEIGHT+2 //����߶ȣ�+2������Χǽ��
#define SPEED_LEVEL_DIFF_SPEED 100 //�ٶȵȼ�֮��ĵ��ٶȲ�

typedef struct body_node{ int x; int y; struct body_node *next;} BODY_NODE;
char spanArr[GAME_HEIGHT][GAME_WIDTH] = { ' '};// �洢��������
BODY_NODE* snakeP = NULL,apple = { 0,0,NULL };//��ͷָ�룬ƻ��
char appleImage = 'O',snakeImage = '@',emptyImage=' ', direction = 'r';//ƻ��ͼ�꣬��ͼ�꣬������ͼ�꣬Ĭ���˶�����l r u d��
unsigned int score = 0;

char info[9][60] = {
	"GAME OVER !!!",
	"�÷֣�%d ",
	"��̰������Ϸ��",
	"C���Կ���̨�ַ������",
	"���ߣ�Hengda",
	"���ڣ�2020/04/19",
	"��Ȩ����",
	"1.��Enter��������Ϸ",
	"2.���������˳�"
};

void setSpanCharByXY(int x, int y, char c) { spanArr[y + 1][x + 1] = c; }//+1ԭ�����б߿�

char getSpanCharByXY(int x, int y) { return spanArr[y + 1][x + 1]; }//+1ԭ�����б߿�

void init_sanke(){//������
	snakeP = (BODY_NODE*)malloc(sizeof(BODY_NODE));//��ʼ��ͷλ��
	snakeP->x = SPAN_WIDTH / 2;
	snakeP->y = SPAN_HEIGHT / 2;
	snakeP->next = NULL;
	setSpanCharByXY(snakeP->x, snakeP->y, snakeImage);
}

void free_snake() {//�ͷ�����ռ�ڴ�
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

void init_apple() {//��������Ҳ������ص���Apple
	apple.x = -1;
	apple.y = -1;
	while (apple.x < 0 || apple.y < 0 || getSpanCharByXY(apple.x, apple.y) == '#') {
		apple.x = rand()% SPAN_WIDTH;
		apple.y = rand()% SPAN_HEIGHT;
	}
	setSpanCharByXY(apple.x, apple.y, appleImage);
}

void eateAppleOrAddHead(int appleX, int appleY) {//��ƻ��
	BODY_NODE* node = (BODY_NODE*)malloc(sizeof(BODY_NODE));
	node->x = appleX;
	node->y = appleY;
	node->next = snakeP;
	snakeP = node;
	setSpanCharByXY(appleX, appleY, snakeImage);//����ƻ�������ƻ������Լ�������
}

void popTail() {//ȥβ
	BODY_NODE * P=snakeP;
	if (P == NULL || P->next == NULL) return;//�߲����ڻ�����ֻ��һ�ڣ�����ɾβ����
	while(P->next->next != NULL) {P = P->next;};
	setSpanCharByXY(P->next->x, P->next->y, emptyImage);
	free(P->next);
	P->next = NULL;
}

int snake_Step(int nextX, int nextY) {//���ƶ�һ��
	//�����Լ������壬����Խ����gameover //nextX >= SPAN_WIDTH || nextY >= SPAN_HEIGHT || nextX < 0 || nextY < 0 ||
	if (getSpanCharByXY(nextX, nextY) == snakeImage) return 0;
	else if (getSpanCharByXY(nextX, nextY) == appleImage) {
		eateAppleOrAddHead(nextX, nextY);
		score++;
		init_apple();//������ƻ��
	}else {
		eateAppleOrAddHead(nextX, nextY);
		popTail();
	}
	return 1;
}

int snake_Move_By_Drection(char direction) {//�߳�ָ�������ƶ�
	if (direction == 'l') return snake_Step(snakeP->x - 1, snakeP->y);
	else if (direction == 'u') return snake_Step(snakeP->x, snakeP->y - 1);
	else if (direction == 'r') return snake_Step(snakeP->x + 1, snakeP->y);
	else if(direction == 'd') return snake_Step(snakeP->x, snakeP->y + 1);
	return 0;
}

char get_direction() {//��ȡ���� UP(���Ϸ����)Ϊ: 0X4800 DOWN(���·����)Ϊ : 0X5000 LEFT(�������)Ϊ : 0X4b00 RIGHT(���ҷ����)Ϊ : 0X4d00
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

void display(){//���»���
	system("cls");//mac ����� system("clear");
	spanArr[GAME_HEIGHT -1][GAME_WIDTH -1] = '\0';
	printf("%s", spanArr);//��ӡ����
}

void int_span(){// ����
	for (int j = 0; j < GAME_HEIGHT; j++)
		for (int i = 0; i < GAME_WIDTH; i++)
			if (i == 0 || (j == 0 && i != GAME_WIDTH - 1) || i == GAME_WIDTH - 2 || j == GAME_HEIGHT - 1) { spanArr[j][i] = snakeImage; }
				else if (i == GAME_WIDTH - 1) { spanArr[j][i] = '\n'; }
					else { spanArr[j][i] = emptyImage; }
}

void display_Info() {//��ӡ��Ϣ
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

int main(int argc, char* argv[]) {//������
	char info[10] = "";
	int gameWidth = GAME_WIDTH;
	int gameHeight = GAME_HEIGHT;
	while (1) {
		score = 0;
		int_span();
		init_sanke();
		init_apple();
		while (1) {
			display();//ˢ������
			if (!snake_Move_By_Drection(get_direction())) {//��ȡ�˶����� �������ƶ�һ��
				free_snake();
				display_Info();
				score = 0;
				display();//ˢ����Щ����
				if (getchar() == '\n') break;//�ȴ�����
				else return 0;
			}
		}
	}
	return 0;
}