/////////////////////////////////////////
// 2016.12.19
// Written by ��ȣ�� <agensoft@naver.com>
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <conio.h>
#include <time.h>
#include <windows.h>

#define MAX_X 20
#define MAX_Y 20
#define LEFT 75
#define RIGHT 77
#define UP 72
#define DOWN 80
#define SPACEBAR 32
#define ESC 27

struct Node
{
	BOOL mine;
	BOOL strip;
	BOOL flag;
	BOOL wall;
	int mineCnt;
};

struct APoint
{
	int x;
	int y;
};

typedef struct value
{
	int score;
	int time;
}values;


void mainMenu();
void levelSelectMenu();

void initMap(struct Node(*gameMap)[MAX_Y], int gameLevel, int* retMineCnt);
void randMine(struct Node(*gameMap)[MAX_Y], int gameLevel, int* retMineCnt);
values gameStart(struct Node(*gameMap)[MAX_Y], int gameLevel, int* retMineCnt, int *life);
void displayMap(struct Node(*gameMap)[MAX_Y], int gameLevel, int* retMineCnt, int* life);
void mineRecursive(struct Node(*gameMap)[MAX_Y], int tmpX, int tmpY);
int resultFunc(struct Node(*gameMap)[MAX_Y], int gameLevel, int* retMineCnt, int tmpRetMineCnt);
int levelSelect(struct Node(*gameMap)[MAX_Y]);
void initLife(int gameLevel, int *life);

int randx(int x, int y);
void gotoxy(int x, int y);
void textcolor(int color_number);

void main()
{
	values v;
	struct Node gameMap[MAX_X][MAX_Y];
	int gameLevel = 10;
	int select;
	int retMineCnt;
	int life;
	int level1 = 999, level2 = 999, level3 = 999;
	int score = 0;
	int score1 = 0, score2 = 0, score3 = 0;//�ʱ� ���� ��

	while (1)
	{
		system("cls");
		mainMenu(); //���� �޴�
		fflush(stdin); scanf("%d", &select);
		system("cls");

		switch (select)
		{
		case 1:
			score = 0; 
			initMap(gameMap, gameLevel, &retMineCnt); //�� �ʱ�ȭ
			initLife(gameLevel, &life); //���� �ʱ�ȭ
			v = gameStart(gameMap, gameLevel, &retMineCnt , &life , &score); //�ɸ� �ð�

			if (gameLevel == 10)
				if (level1 > v.time)
				{
					level1 = v.time;
					score1 = v.score;
				}//�ְ� ��� ����
				if (level1 == v.time)
					if (score1 < v.score)
						score1 = v.score;
			else if (gameLevel == 5)
				if (level2 > v.time)
				{
					level2 = v.time;
					score2 = v.score;
				}//�ְ� ��� ����
				if (level2 == v.time)
					if (score2 < v.score)
						score2 = v.score;
			else if (gameLevel == 0)
				if (level3 > v.time)
				{
					level3 = v.time;
					score3 = v.score;
				}//�ְ� ��� ����
				if (level3 == v.time) // �ð��� ���� ���
					if (score3 < v.score)
						score3 = v.score;
			break;
		case 2:
			gameLevel = levelSelect(gameMap); //���̵� ����
			break;
		case 3: //��� Ȯ��
			printf("�Ǣ�      �ְ� ���      �Ǣ�\n");
			printf(" 1.��  �� : %d �� ���� : %d\n", level1 , score1);
			printf(" 2.��  �� : %d �� ���� : %d\n", level2 , score2);
			printf(" 3.��  �� : %d �� ���� : %d\n", level3 , score3);
			//printf(" 4.�ֻ�� : %d \n", level4);
			system("pause");
			break;
		case 0: //���� ����
			puts("������ �����մϴ�");
			return;
		default:
			break;
		}
	}
}

void mainMenu() //���θ޴�
{
	textcolor(14);
	puts("������������������������������������������������������������������������");
	puts("��    �Ǣ� ���� ã�� ���� �Ǣ�      ��");
	puts("��                                  ��");
	puts("��          1. ���� ����            ��");
	puts("��                                  ��");
	puts("��          2. ���̵� ����          ��");
	puts("��                                  ��");
	puts("��          3. �ְ� ����            ��");
	puts("��                                  ��");
	puts("��          0. ��     ��            ��");
	puts("��                                  ��");
	puts("������������������������������������������������������������������������");
	printf(" Select > ");
}

void levelSelectMenu() //���̵� ����
{
	textcolor(11);
	puts("������������������������������������������������������������");
	puts("��    �Ǣ� ���̵� ���� �Ǣ�   ��");
	puts("��                            ��");
	puts("��        1. ��     ��        ��");
	puts("��                            ��");
	puts("��        2. ��     ��        ��");
	puts("��                            ��");
	puts("��        3. ��     ��        ��");
	puts("��                            ��");
	//puts("��        4. ��  ��  ��       ��");
	//puts("��                            ��");
	puts("������������������������������������������������������������");
	printf(" Select > ");
}

void initMap(struct Node(*gameMap)[MAX_Y], int gameLevel, int* retMineCnt) //���� ���� 0���� �ʱ�ȭ
{
	int i, j;
	int cx, cy;

	for (i = 0; i < MAX_X - gameLevel; i++)
	{
		for (j = 0; j < MAX_Y - gameLevel; j++)
		{
			gameMap[i][j].wall = 0;
			gameMap[i][j].flag = 0;
			gameMap[i][j].mine = 0;
			gameMap[i][j].strip = 0;
			gameMap[i][j].mineCnt = 0;
		}
	}

	for (i = 0; i < MAX_X - gameLevel; i++)
	{
		for (j = 0; j < MAX_Y - gameLevel; j++)
		{
			if (i == 0 || i == MAX_X - gameLevel - 1 || j == 0 || j == MAX_Y - gameLevel - 1)
				gameMap[i][j].wall = 1;
			else
				gameMap[i][j].strip = 0;
		}
	}
	randMine(gameMap, gameLevel, retMineCnt); //���� �ɱ�

	for (i = 0; i < MAX_X - gameLevel; i++)
	{
		for (j = 0; j < MAX_Y - gameLevel; j++)
		{
			for (cx = i - 1; cx < i + 2; cx++)
			{
				for (cy = j - 1; cy < j + 2; cy++)
				{
					if (gameMap[cx][cy].mine == 1)
					{
						gameMap[i][j].mineCnt++; 
					}
				}
			}
		}
	}
}

void initLife(int gameLevel, int *life) //���� �ʱ�ȭ
{
	int i;

	if (gameLevel == 10)		i = 5;
	else if (gameLevel == 5)	i = 4;
	else if (gameLevel == 0)	i = 3;
	else {}

	*life = i;
}

void randMine(struct Node(*gameMap)[MAX_Y], int gameLevel, int* retMineCnt) //���� �ɱ�
{
	int i, j;
	int mineCntSet; // ���� ������

	if (gameLevel == 10)		mineCntSet = 10;
	else if (gameLevel == 5)	mineCntSet = 30;
	else if (gameLevel == 0)	mineCntSet = 80;
	else {}
	*retMineCnt = mineCntSet;

	do
	{
		i = randx(1, MAX_X - gameLevel - 2);
		j = randx(1, MAX_Y - gameLevel - 2);

		if (gameMap[i][j].mine == 0)
		{
			gameMap[i][j].mine = 1;
			mineCntSet--;
		}
	} while (mineCntSet);
}


values gameStart(struct Node(*gameMap)[MAX_Y], int gameLevel, int* retMineCnt , int *life , int *score) //���� ����
{
	values p;
	struct APoint player;
	char ch;
	int i, j;
	int tmpRetMinCnt;
	time_t startTime;
	time_t endTime;
	double elapsedTime = 0;

	time(&startTime);
	tmpRetMinCnt = *retMineCnt;
	player.x = 2;
	player.y = 1;
	displayMap(gameMap, gameLevel, retMineCnt,life,score); //�� ǥ��
	gotoxy(player.x, player.y);

	while (1)
	{
		if (*life == 0) //����� 0�̸�
		{
			for (i = 0; i < MAX_X - gameLevel; i++)
			{
				for (j = 0; j < MAX_Y - gameLevel; j++)
				{
					gameMap[i][j].strip = 1;
				}
			}
			displayMap(gameMap, gameLevel, retMineCnt, life , score);
			gotoxy(((MAX_X - gameLevel) * 2) + 3, 4);
			printf("Game Over... T.T");
			gotoxy(((MAX_X - gameLevel) * 2) + 3, 5);
			system("pause");
			p.time = 999;
			p.score = 0;
			return p;
		}

		if (_kbhit())
		{
			ch = _getch();
			switch (ch)
			{
			case LEFT:
				if (player.x - 2 > 0) gotoxy(player.x -= 2, player.y);
				break;
			case RIGHT:
				if (player.x < (MAX_Y - gameLevel - 2) * 2) gotoxy(player.x += 2, player.y);
				break;
			case UP:
				if (player.y - 1 > 0) gotoxy(player.x, --player.y);
				break;
			case DOWN:
				if (player.y + 1 < MAX_X - gameLevel - 1)	gotoxy(player.x, ++player.y);
				break;
			default:
				break;
			}

			if (ch == 'f' || ch == 'F')	// flag
			{
				if (gameMap[player.y][player.x / 2].flag == 0) { gameMap[player.y][player.x / 2].flag = 1;	--* retMineCnt; }
				else { gameMap[player.y][player.x / 2].flag = 0;	++* retMineCnt; }

				if (resultFunc(gameMap, gameLevel, retMineCnt, tmpRetMinCnt,life,score) == 1) //Ŭ���� ������
				{
					time(&endTime);
					elapsedTime = difftime(endTime, startTime);
					gotoxy(((MAX_X - gameLevel) * 2) + 3, 5);
					printf("�� ��� �ð� : %3d �� ", (int)elapsedTime);
					gotoxy(((MAX_X - gameLevel) * 2) + 3, 6);
					system("pause");
					p.time = elapsedTime;
					p.score = *score;
					return p;
				}
			}
			else if (ch == SPACEBAR) // Ÿ�� ����
			{
				if (gameMap[player.y][player.x / 2].mine != 1)
				{
					mineRecursive(gameMap, player.y, player.x / 2);
					*score += 10;
					if (resultFunc(gameMap, gameLevel, retMineCnt, tmpRetMinCnt,life) == 1)
					{
						time(&endTime);
						elapsedTime = difftime(endTime, startTime);
						gotoxy(((MAX_X - gameLevel) * 2) + 3, 5);
						printf("�� ��� �ð� : %3d �� ", (int)elapsedTime);
						gotoxy(((MAX_X - gameLevel) * 2) + 3, 6);
						system("pause");
						p.time = elapsedTime;
						p.score = *score;
						return p;
					}
				}
				else	// ������ ���
				{

					/*for (i = 0; i < MAX_X - gameLevel; i++)
					{
						for (j = 0; j < MAX_Y - gameLevel; j++)
						{
							gameMap[i][j].strip = 1;
						}
					}*/
				*score -= 50;
				gameMap[player.y][player.x / 2].strip = 1;
				displayMap(gameMap, gameLevel, retMineCnt,life,score);
				printf("Bomb Explode...!  \n");
				gotoxy(((MAX_X - gameLevel) * 2) + 3, 5);
				//system("pause");
				//return 999;
				*life -= 1;
				--* retMineCnt;
				--tmpRetMinCnt;
				
				}
			}
			else if (ch == ESC)
			{
				p.time = 999;
				p.score = 0;
				return p;
			}
			displayMap(gameMap, gameLevel, retMineCnt, life,score);
			gotoxy(player.x, player.y);
		}
		Sleep(10);
	}
}

void displayMap(struct Node(*gameMap)[MAX_Y], int gameLevel, int* retMineCnt , int *life , int *score) //�� ǥ��
{
	int i, j;

	gotoxy(0, 0);
	for (i = 0; i < MAX_X - gameLevel; i++)
	{
		for (j = 0; j < MAX_Y - gameLevel; j++)
		{
			if (gameMap[i][j].wall == 1) { textcolor(4);	printf("��"); }
			else
			{
				if (gameMap[i][j].strip == 0)
				{
					if (gameMap[i][j].flag == 0) { textcolor(3);	printf("��"); }
					else { textcolor(4);	printf("��"); }
				}
				else if (gameMap[i][j].strip == 1)
				{
					if (gameMap[i][j].mine == 1) { textcolor(12);	printf("��"); }
					else
					{
						if (gameMap[i][j].mineCnt == 0) { textcolor(6);	printf("��"); }
						else if (gameMap[i][j].mineCnt == 1) { textcolor(1);	printf("��"); }
						else if (gameMap[i][j].mineCnt == 2) { textcolor(2);	printf("��"); }
						else if (gameMap[i][j].mineCnt == 3) { textcolor(3);	printf("��"); }
						else if (gameMap[i][j].mineCnt == 4) { textcolor(5);	printf("��"); }
						else if (gameMap[i][j].mineCnt == 5) { textcolor(6);	printf("��"); }
						else if (gameMap[i][j].mineCnt == 6) { textcolor(7);	printf("��"); }
						else if (gameMap[i][j].mineCnt == 7) { textcolor(8);	printf("��"); }
						else if (gameMap[i][j].mineCnt == 8) { textcolor(9);	printf("��"); }
					}
				}
			}
		}
		printf("\n");
	}
	gotoxy(((MAX_X - gameLevel) * 2) + 3, 1);	textcolor(14);	printf("�� ���� : %2d", *score);
	gotoxy(((MAX_X - gameLevel) * 2) + 3, 2);	textcolor(14);	printf("�� ���� ��� : %2d", *life);
	gotoxy(((MAX_X - gameLevel) * 2) + 3, 3);	textcolor(14);	printf("�� ��� ���� : %2d", *retMineCnt);
	gotoxy(((MAX_X - gameLevel) * 2) + 3, 4);
}


void mineRecursive(struct Node(*gameMap)[MAX_Y], int tmpX, int tmpY) //��ó ���� ���� �ľ�
{
	int i, j;

	if (gameMap[tmpX][tmpY].wall == 1) return;
	if (gameMap[tmpX][tmpY].strip == 1) return;
	if (gameMap[tmpX][tmpY].mineCnt)
	{
		gameMap[tmpX][tmpY].strip = 1;
		return;
	}

	gameMap[tmpX][tmpY].strip = 1;
	for (i = tmpX - 1; i < tmpX + 2; i++)
	{
		for (j = tmpY - 1; j < tmpY + 2; j++)
		{
			mineRecursive(gameMap, i, j);
		}
	}
}

int resultFunc(struct Node(*gameMap)[MAX_Y], int gameLevel, int* retMineCnt, int tmpRetMineCnt,int *life, int *score) //���
{
	int i, j;
	int stripCnt = 0;
	int tmpCnt;
	tmpCnt = tmpRetMineCnt;

	for (i = 0; i < MAX_X - gameLevel; i++)
	{
		for (j = 0; j < MAX_Y - gameLevel; j++)
		{
			if (gameMap[i][j].wall != 1 && gameMap[i][j].strip == 0)	stripCnt++;
		}
	}
	if (*retMineCnt == 0 && stripCnt == tmpCnt)
	{
		displayMap(gameMap, gameLevel, retMineCnt,life,score);

		gotoxy(((MAX_X - gameLevel) * 2) + 3, 4);
		printf("Good...!!                       ");
		gotoxy(((MAX_X - gameLevel) * 2) + 3, 5);
		return 1;
	}
	return 0;
}

int levelSelect(struct Node(*gameMap)[MAX_Y]) //���� ����
{
	int select;
	while (1)
	{
		system("cls");
		levelSelectMenu();
		fflush(stdin);	scanf("%d", &select);

		if (select == 1)			return 10;
		else if (select == 2)	return 5;
		else if (select == 3)	return 0;
		else					continue;
	}
}

int randx(int x, int y)
{
	static int flag = 0;
	int retVal;
	if (flag == 0)
	{
		srand(time(NULL));
		rand(); rand(); rand(); rand();
		srand(rand());
		flag = 1;
	}
	retVal = rand() % (y - x + 1) + x;
	return retVal;
}

void gotoxy(int x, int y)
{
	COORD cd;
	cd.X = x;
	cd.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cd);
}

void textcolor(int color_number)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color_number);
}
