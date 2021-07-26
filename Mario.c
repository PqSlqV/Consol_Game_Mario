#define _CRT_SECURE_NO_WARNINGS 1
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include<Windows.h>

#define mapheight 28
#define mapwidth 110
#define col 20

typedef struct SObject
{
	float x, y;
	float h, w;
	float speed;
	BOOL fly;
	char t;
	float gorspeed;
}Tobject;

char map[mapheight][mapwidth+1];

Tobject mario;
Tobject ob[col];
Tobject fin;
Tobject* enemy = NULL;
Tobject* box = NULL;
Tobject obbox[col/2];


int lvl = 1;
int colbox = 0;
int monbox = 0;
int colenemy = 0;
int renemy = 0;
int score = 0;

BOOL Coll(Tobject obj, Tobject obj2)
{
	return (((obj.x + obj.w) > obj2.x) && (obj.x < (obj2.w + obj2.x)) && ((obj.y + obj.h) > obj2.y) && (obj.y < (obj2.h + obj2.y)) && (obj2.h != 0));
}

void putobj(Tobject* obj)
{
	int xpos = (int)round((*obj).x);
	int ypos = (int)round((*obj).y);
	int height = (int)round((*obj).h) + ypos;
	int width = (int)round((*obj).w) + xpos;
		for (int j = ypos; j < height; j++)
			for (int i = xpos; i < width; i++)
				if ((i >= 0) && (i < mapwidth) && (j > 0) && (j < mapheight))
					map[j][i] = (*obj).t;
}

void setpos(Tobject* obj, float xpos, float ypos)
{
	(*obj).x = xpos;
	(*obj).y = ypos;
}

void initobj(Tobject* obj, float xpos, float ypos, float width, float height, char type)
{
	setpos(obj, xpos, ypos);
	(*obj).h = height;
	(*obj).w = width;
	(*obj).t = type;
}

void clearmap()
{
	for (int i = 0; i < mapwidth; i++)
		map[0][i] = ' ';
	map[0][mapwidth] = '\0';
	for (int i = 0; i < mapheight; i++)
		sprintf(map[i], map[0]);
}

int heightbox(int x)
{
	int k = 25;
	for (int i = 1; i < col; i++)
	{
		if ((ob[i].x + ob[i].w) > x)
		{
			k = ob[i].y;
			return k;
		}
	}
	return k;
}

Tobject* addbox()
{
	monbox++;
	box = realloc(box, sizeof(*box) * monbox);
	return box + monbox - 1;
}
void delbox(int i)
{
	monbox--;
	box[i] = box[monbox];
	box = realloc(box, sizeof(*box) * monbox);
}

Tobject* addenemy()
{
	colenemy++;
	enemy = realloc(enemy, sizeof(*enemy) * colenemy);
	return enemy + colenemy - 1;

}

void delenemy(int i)
{
	colenemy--;
	enemy[i] = enemy[colenemy];
	enemy = realloc(enemy, sizeof(*enemy) * colenemy);
}

void init()
{	
	monbox = 0;
	box = realloc(box, 0);
	colenemy = 0;
	enemy = realloc(enemy, 0);
	srand(lvl);
	int l = 0, k = 0, m = 0, i = 0;
	clearmap();
	initobj(&mario, 50, 10, 3, 3, '@');
	mario.speed = 0;
	for (i = 0; i < col; i++)
	{
		while ((k == l) || (k > m+7))	k = rand() % 15;
		initobj(&ob[i], 30 + i * 30, mapheight - k - 1, 30, k, '#');
		l = k;
		m = l;
	}
	initobj(&fin, 30 + col * 30, mapheight - 6, 20, 5, '+');

	colbox = rand() % (col / 2);
	for (i = 0; i < colbox; i++)
	{
		int q = 0, n = 0;
		do
		{
			k = (rand() % col) * 30 + 40;
			if (i != 0)
			{
				for (int j = 0; j < i; j++)
				{
					if (((obbox[j].x - 7) < k) && ((obbox[j].x + 7) > k))
					{
						n = 1;
						break;
					}
					else n = 0;
				}
			}
		} while (n != 0);
		l = rand() % 9;
		q = heightbox(k);
		initobj(&obbox[i], k, q - 7 - l, 11, 3, '-');
		initobj(addbox(), k + 3, q - 7 - l, 5, 3, '?');
	}
	renemy = rand() % (lvl+2);

	for (i = 0; i < renemy; i++)
	{
		k = (rand() % ((col-1) * 30))  + 35;
		initobj(addenemy(), k, 10 , 3, 3, 'o');
		enemy[i].speed = -1;
		enemy[i].gorspeed = 0.5;
	}
}
void vertmove(Tobject*);
void obgormove(Tobject* obj)
{
	(*obj).x +=((*obj).gorspeed);
	for (int i = 0; i < col; i++)
	{
		if (Coll(*obj, ob[i]))
		{
			(*obj).x -= (*obj).gorspeed;
			(*obj).gorspeed = -(*obj).gorspeed;
			return;
		}
	}
	Tobject temp = *obj;
	vertmove(&temp);
	if (temp.fly == TRUE)
	{
		(*obj).x -= (*obj).gorspeed;
		(*obj).gorspeed = -(*obj).gorspeed;
	}
}
void vertmove(Tobject* obj)
{
	(*obj).fly = TRUE;
	(*obj).speed += (float)0.05;
	setpos(obj, (*obj).x, (*obj).y + (*obj).speed);
	for (int i = 0; i < col; i++)
	{
		if ((Coll(*obj, ob[i]) == TRUE)||(Coll(mario, obbox[i]) == TRUE) && (i < colbox))
		{
			if (obj == &mario)
			{
				for (int i = 0; i < monbox; i++)
					if ((Coll(mario, box[i]) == TRUE) && (box[i].t != '$') && (mario.y  >= box[i].y))
					{
						initobj(&box[i], box[i].x + 1, box[i].y - 3, 3, 3, '$');
						box[i].speed = -1;
						box[i].gorspeed = 1;
					}
			}
			(*obj).y -= (*obj).speed;
			(*obj).speed = 0;
			(*obj).fly = FALSE;
		}
	}
	
}

void gormove(int x)
{
	int i = 0;
	mario.x -= x;
	for (i = 0; i < col; i++)
	{
		if ((Coll(mario, ob[i]) == TRUE) || ((Coll(mario, obbox[i]) == TRUE) && (i < colbox)))
		{
			mario.x += x;
			return;
		}
	}
	mario.x += x;
	for (i = 0; i < col; i++)
	{
			ob[i].x += x;
	}
	fin.x += x;
	for (i = 0; i < colbox; i++) obbox[i].x += x;
	for (i = 0; i < monbox; i++) box[i].x += x;
	for (i = 0; i < colenemy; i++) enemy[i].x += x;

}

void death()
{
	int i;
	if ((mario.y + mario.h) > (mapheight - 1))
	{
		score -= 300;
		system("color 4F");
		init();
		Sleep(1000);
		system("color 0F");
	}
	for (i = 0; i < monbox; i++)
	{
		if (Coll(mario, box[i]) == TRUE && box[i].t == '$')
		{
			delbox(i);
			score += 50;
		}
	}
	for (i = 0; i < colenemy; i++)
	{
		if ((Coll(mario, enemy[i]) == TRUE) && ((mario.y) <= enemy[i].y))
		{
			delenemy(i);
			score += 100;
			continue;
		}
		if ((Coll(mario, enemy[i]) == TRUE) && ((mario.y) > enemy[i].y))
		{
			score -= 300;
			system("color 4F");
			init();
			Sleep(1000);
			system("color 0F");
		}
	}
	
}

void show()
{
	printf("\t\t\tLevel = %d\t\tScore = %7d\t", lvl,score);
	for (int i = 0; i < mapheight; i++)
		printf("%s\n", map[i]);
}

void setcur(int x, int y)
{
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void main()
{
	void* handle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO structCursorInfo;
	GetConsoleCursorInfo(handle, &structCursorInfo);
	structCursorInfo.bVisible = FALSE;
	SetConsoleCursorInfo(handle, &structCursorInfo);
	init();
	int i;
	do
	{
		
		setcur(0, 0);
		if ((mario.fly==FALSE) && (GetKeyState(VK_SPACE) < 0))
		{
			mario.speed = -1;
		}
		clearmap();
		if ((GetKeyState('D') < 0) || (GetKeyState(VK_RIGHT) < 0))gormove(-1);
		if ((GetKeyState('A') < 0) || (GetKeyState(VK_LEFT) < 0))gormove(1);
		putobj(&mario);
		for (i = 0; i < col; i++)putobj(&ob[i]);
		for (i = 0; i < colbox; i++)putobj(&obbox[i]);
		for (i = 0; i < monbox; i++)
		{
			if (box[i].t == '$')
			{
				vertmove(&box[i]);
				obgormove(&box[i]);
			}
			putobj(&box[i]);
		}
		for (i = 0; i < colenemy; i++)
		{
			if (enemy[i].y >= mapheight)
			{
				delenemy(i);
				continue;
			}
			vertmove(&enemy[i]);
			obgormove(&enemy[i]);
			putobj(&enemy[i]);
		}
		putobj(&fin);
		vertmove(&mario);
		if (Coll(mario, fin) == TRUE)
		{
			lvl++;
			score += 100;
			system("color AF");
			Sleep(1000);
			init();
			system("color 0F");
		}
		death();
		show();
		Sleep(15);
	} while (GetKeyState(VK_ESCAPE) >= 0);
}