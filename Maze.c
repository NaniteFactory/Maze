#define _CRT_SECURE_NO_WARNINGS
#define FALSE 0
#define TRUE !FALSE
#define DARR(name, y, x, width) (*(name + y*width + x)) // ��ũ�� �Լ� // name[y][x]
// ���:
// DARR(map,����,����) => map[����][����]
// ex) map[3][4] => map �迭�� ��ǥ (4,3)�� ���
// �ο�:
// ������ �迭 ��Ҹ� ���� �����ͷ� ǥ���� ���̴�.
// �̰��� �ʿ��� ������ �Լ����� ������ �迭�� �̷θ� ���ڷ� ���� �� �� �迭�� ��Ȯ�� ���� ũ�⸦ �̸� �� �� �����Ƿ�
// ���� ������ ��� ���� ������(int *map)�� �Ű������� ����ϰ� �ֱ� �����̴�.

#include "stdio.h" //�⺻ �����
#include "stdlib.h" //malloc, exit

///////////////////////////////// ��ó�� ////////////////////////////////////////

enum{ EMPTY = 0, EXIT = 1, WALL = 2, ENTRANCE = 3, CAMEIN = 4, WENTBACK = 5, BRANCH = 6 }; // SPACE TYPE // 2 �̻��� ���� ������ �� ���� // ������ ���� 4 �̻����� ǥ�õȴ�
enum{ UNKNOWN = 0, UP = 1 , DOWN = 2, RIGHT = 3, LEFT = 4 }; // �̷� ������ ���

typedef struct point
{
	unsigned int x;
	unsigned int y;
}point;

typedef struct point element;

typedef struct stacknode
{
	element data; // point = element = stacknode.data
	struct stacknode *link;
} stacknode;

typedef struct
{
	stacknode *top;
} stacktype;

typedef struct maze
{
	struct point size;
	int *map;
}maze;

///////////////////////////////// ����ü ////////////////////////////////////////

void init_stacktype(stacktype *s)
{
	s->top = NULL;
}

void push(stacktype *s, element data)
{
	stacknode *temp = (stacknode *)malloc(sizeof(stacknode));
	if (!temp)
	{
		printf("\n�޸� ����\n\n");
		system("pause");
		exit(1);
	} //�Ҵ� �Ұ�
	if (s->top != NULL)
	{
		temp->data = data;
		temp->link = s->top;
	}
	else
	{
		temp->data = data;
		temp->link = NULL;
	} // �� ������ ���
	s->top = temp;	// ���� ����, �ʱ�ȭ, top ����
}

element pop(stacktype *s)
{
	if (s->top == NULL) 
	{
		printf("\n����: ��� �ִ� ���ÿ� POP �õ�\n\n");
		system("pause");
		exit(1);
	}
	else
	{
		stacknode *temp = s->top; //������ ��� ����
		element data = temp->data; //�������� ���� ����
		s->top = s->top->link;
		free(temp);
		return data; //��ũ ����, ����, ��ȯ
	}
}

int stackfree(stacktype *s) //���� ����
{
	stacknode *temp = s->top; // ������ ���
	if (s->top == NULL) // �̹� �� ������ ���
	{
		return FALSE;
	}
	while (s->top != NULL) // �� �� ������ �� ������ �ݺ�
	{
		s->top = s->top->link; // top �� ĭ ����
		free(temp); // ���� �޸� ����
		temp = s->top; // ���� ���� ��� ����
	}
	return TRUE;
}// ������� TRUE, �̹� �� �����̸� FALSE ��ȯ

///////////////////////////////// ���� �Լ� ////////////////////////////////////////

void display(maze *maze)
{
	unsigned int i;
	unsigned int j;
	for (i = 0; i < maze->size.y; ++i)
	{
		for (j = 0; j < maze->size.x; ++j)
		{
			switch (DARR(maze->map, i, j, maze->size.x)) // map[i][j]
			{
			case EMPTY:
				printf("��"); // ������ �� �ִ� ����
				break;
			case EXIT:
				printf("��"); //�ⱸ
				break;
			case ENTRANCE:
				printf("��"); //�Ա�
				break;
			case WALL:
				printf("��"); // ������ �� ���� ��
				break;
			case CAMEIN:
				printf("��"); // �ùٸ� �� ���� ������ ��
				break;
			case WENTBACK:
				printf("��"); //������ �� �ִ� ���ٸ� ��
				break;
			case BRANCH:
				printf("��"); //������
				break;
			} // switch
		} // for j
		printf("\n"); // �� �� ����� ��ģ �� ����
	} // for i
}

void show(stacktype *s) // ������ ���� peek�ؼ� ������ ���븦 ��ǥ�� ǥ��
{
	int i=0;
	stacknode *temp = s->top;
	for (; temp; temp = temp->link) // while(s->top != NULL) {printf(); s->top = s->top->link;}
	{
		i++;
		printf("(%d, %d) ", temp->data.x, temp->data.y);
		if (i == 5)
		{
			printf("\n");
			i = 0;
		}
	}
}

point find(maze *maze, int lookingfor)
{
	unsigned int i;
	unsigned int j;
	point here;
	for (i = 0; i < maze->size.y; ++i)
	{
		for (j = 0; j < maze->size.x; ++j)
		{
			if (DARR(maze->map, i, j, maze->size.x) == lookingfor) // map[i][j] == lookingfor // ã�� ���̸�
			{
				here.x = j;
				here.y = i;
				return here; // �� ������ ��ȯ
			}
		} // for j
	} // for i
	printf("\n�߸��� �̷�: �䱸�Ǵ� ������ ã�� �� �����ϴ�.\n");
	system("pause");
	exit(1); 
}

void go(stacktype *s, maze *maze) // �� ĭ�� ���� �� ã��
{
	point spot = find(maze, ENTRANCE); // ���� ��ǥ�� ��Ƶ� ���� ���� // ��� ���� Ž��
	point entrance = spot; // ��ȯ�� �� ���� (��� ����)
	int count;
	init_stacktype(s); // ���� �ʱ�ȭ
	system("cls"); // ȭ�� û��
	while (DARR(maze->map, spot.y, spot.x, maze->size.x) != EXIT) // map[spot.y][spot.x] != �ⱸ�� ã�� ������ �ݺ�
	{
		switch (DARR(maze->map, spot.y, spot.x, maze->size.x)) // ������ ���� ǥ�� ������ �׷���
		{
		case EMPTY:
			DARR(maze->map, spot.y, spot.x, maze->size.x) = CAMEIN;
			break;
		case CAMEIN:
			if
				(
				DARR(maze->map, (spot.y + 1), spot.x, maze->size.x) > 1
				&& DARR(maze->map, (spot.y - 1), spot.x, maze->size.x) > 1
				&& DARR(maze->map, spot.y, (spot.x - 1), maze->size.x) > 1
				&& DARR(maze->map, spot.y, (spot.x + 1), maze->size.x) > 1
				)//�� ĭ�� ��� ��𿡵� �� ������ ���ٸ� ���ƿԴٴ� ǥ�ø� �����. �� �����濡���� ǥ������ �ʴ´�.
			{
				DARR(maze->map, spot.y, spot.x, maze->size.x) = WENTBACK;
			}
			else // ���� �Դ� �������� �ٸ� ���⿡ �� ������ �ִ� ��� �������̶�� ǥ�ø� �����.
			{
				DARR(maze->map, spot.y, spot.x, maze->size.x) = BRANCH;
			}
			break;
		}//switch
		display(maze); // �׸� ���
		show(s); // ���� ���
		if (DARR(maze->map, (spot.y - 1), spot.x, maze->size.x) < 2) // ������ �ʿ� ���̳� ���� ������ ���ٸ� �� ������ ���� �� ��ġ�� ���(push)�Ѵ�.
		{
			spot.y--;
			push(s, spot);
		}//��
		else if (DARR(maze->map, spot.y, (spot.x + 1), maze->size.x) < 2)
		{
			spot.x++;
			push(s, spot);
		}//��
		else if (DARR(maze->map, (spot.y + 1), spot.x, maze->size.x) < 2) 
		{
			spot.y++;
			push(s, spot);
		}//��
		else if (DARR(maze->map, spot.y, (spot.x - 1), maze->size.x) < 2)
		{
			spot.x--;
			push(s, spot);
		}//��
		else
		{
			spot = pop(s);
		}//���ٸ� ���� ��� �� ����� ����� (pop)
		system("pause"); // �� ���ʸ� ������ �Է��� ��ٸ���
		system("cls"); // ���� ���� ����
	} // while
	display(maze);
	printf("\n\n<<<<< ������ ���� !! >>>>>\n\n\n");
	stackfree(s);
}

void reset(maze *maze) // �̷θ� ������ ǥ�� ����
{
	unsigned int i;
	unsigned int j;
	for (i = 0; i < maze->size.y; i++)
	{
		for (j = 0; j < maze->size.x; j++)
		{
			if (DARR(maze->map, i, j, maze->size.x) > 3)
			{
				DARR(maze->map, i, j, maze->size.x) = EMPTY;
			}
		}
	}
}

///////////////////////////////// �̷� Ž�� �Լ� ////////////////////////////////////////

/*
���� �̷� ���� �Լ�
������ ����:
���(2����) �̷θ� ����� ���θ��� �׸� ����� �������� ǥ���Ѵ�.
������ ���� ���(�������� or �����¿�) 4���� �ݺ����� �ѷ��ο� �ִ�.
������ �波�� ���� Ʈ���� ���� ���� �ⱸ���� ���� ���� �Ա��� �� ��ġ�� ���� ���ش�.
���� ��� ���� ���� ���� ���� �����ʿ� �ִٰ� �ϸ� ���� ���� ������ ������ ���� ���� ������ �Ǵ� ���� �����ϴ� ���̴�.
*/

int is_closed(maze *maze, unsigned int x, unsigned int y) // �� ���� ����� ���� �ִ�
{
	if
		(
		DARR(maze->map, (y - 1), x, maze->size.x) == WALL
		&& DARR(maze->map, (y + 1), x, maze->size.x) == WALL
		&& DARR(maze->map, y, (x - 1), maze->size.x) == WALL
		&& DARR(maze->map, y, (x + 1), maze->size.x) == WALL
		)
	{
		return TRUE;
	}
	return FALSE;
}

int *cell(unsigned int height, unsigned int width) // ũ�⿡ ���� ���� �����ϰ� ���� �����. �̷� ������ �ռ� ���� �ʱ�ȭ �۾�.
{
	unsigned int i;
	unsigned int j;
	int *map = (int *)malloc(sizeof(int)*height*width);
	for (i = 0; i < height; i++)
	{
		for (j = 0; j < width; j++)
		{
			if (i % 2 == 0 || j % 2 == 0) // ¦���� ���̰ų� ���̸� ���� ������ ���� �����. ���� ������ # ������� �����.
			{
				DARR(map, i, j, width) = WALL;
			}
			else
			{
				DARR(map, i, j, width) = EMPTY;
			}
		}//for
	}//for
	return map;
}

void gen(stacktype *s, maze *maze, point spot)
{ // �̷� ������ ������ �̷� Ž���� ����. �ٸ� �����濡�� ��� �������� ���������� �������� �����Ѵ�. Ž�� �Լ������� �� �� �ִ� ���� �������(�ð����) ������ Ȯ���ߴ�.
	unsigned int visit = 1; // �湮 Ƚ�� (���� Ʒ�� Ƚ��)
	point neighbour[4];
	int kind[4] = { 0, 0, 0, 0 };
	int index = 0;
	int random; // �̻��� �̿��� �濡 ���� ����
	point entrance = spot;
	init_stacktype(s); // ������ ����Ѵ�
	while (visit < ((maze->size.x - 1) / 2)*((maze->size.y - 1) / 2)) //���� �� = ((����-1 )/2) * ((����-1)/2) // ��� ���� �湮�� ������ �ݺ�
	{
		index = 0;
		if ((signed)spot.y - 2 > 0 && is_closed(maze, spot.x, (spot.y - 2)))
		{
			neighbour[index].x = spot.x;
			neighbour[index].y = spot.y - 2;
			kind[index++] = UP;
		} //��
		if (spot.y + 2 < maze->size.y && is_closed(maze, spot.x, (spot.y + 2)))
		{
			neighbour[index].x = spot.x;
			neighbour[index].y = spot.y + 2;
			kind[index++] = DOWN;
		} //��
		if ((signed)spot.x - 2 > 0 && is_closed(maze, (spot.x - 2), spot.y))
		{
			neighbour[index].x = spot.x - 2;
			neighbour[index].y = spot.y;
			kind[index++] = LEFT;
		} //��
		if (spot.x + 2 < maze->size.x && is_closed(maze, (spot.x + 2), spot.y))
		{
			neighbour[index].x = spot.x + 2;
			neighbour[index].y = spot.y;
			kind[index++] = RIGHT;
		} //��
		// ���� �濡�� �̿��� �� �� �մ��� ���� ���� ã�� �������� �߰��Ѵ�
		if (index == 0) // �մ��� ���� �̿��� ���� ���ٸ� �ǵ��ư���
		{
			spot = pop(s);
		}
		else
		{
			random = rand() % index; // �ĺ��� ����� �� �� �� �ϳ��� �������� �ϳ� �����ؼ� ���� �����Ѵ�
			push(s, spot);
			switch (kind[random])
			{
			case UP:
				printf("%d,%d UP ", spot.x, spot.y);
				DARR(maze->map, (spot.y - 1), spot.x, maze->size.x) = EMPTY;
				break;
			case DOWN:
				printf("%d,%d DOWN ", spot.x, spot.y);
				DARR(maze->map, (spot.y + 1), spot.x, maze->size.x) = EMPTY;
				break;
			case LEFT:
				printf("%d,%d LEFT ", spot.x, spot.y);
				DARR(maze->map, spot.y, (spot.x - 1), maze->size.x) = EMPTY;
				break;
			case RIGHT:
				printf("%d,%d RIGHT ", spot.x, spot.y);
				DARR(maze->map, spot.y, (spot.x + 1), maze->size.x) = EMPTY;
				break;
			} //switch
			spot = neighbour[random];
			visit++;
		} //else
	}//for
	stackfree(s);
	DARR(maze->map, spot.y, spot.x, maze->size.x) = EXIT;
	DARR(maze->map, entrance.y, entrance.x, maze->size.x) = ENTRANCE;
	printf("\n�����Ϸ�\n\n");
}//gen()

///////////////////////////////// �̷� ���� �Լ� ////////////////////////////////////////

int map1[10][10] = {
	{ 2, 2, 2, 2, 2, 2, 2, 2, 2, 2 },
	{ 3, 0, 0, 0, 0, 0, 0, 0, 0, 2 },
	{ 2, 2, 2, 2, 2, 0, 2, 2, 2, 2 },
	{ 2, 0, 0, 2, 2, 0, 2, 0, 0, 2 },
	{ 2, 2, 0, 0, 0, 0, 0, 0, 2, 2 },
	{ 2, 2, 0, 2, 2, 2, 0, 2, 2, 2 },
	{ 2, 0, 0, 2, 0, 0, 0, 0, 0, 2 },
	{ 2, 2, 0, 2, 2, 2, 2, 2, 2, 2 },
	{ 2, 2, 0, 0, 0, 0, 0, 0, 0, 1 },
	{ 2, 2, 2, 2, 2, 2, 2, 2, 2, 2 }
};

int map2[11][13] = {
	{ 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2 },
	{ 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2 },
	{ 2, 2, 2, 0, 2, 0, 2, 2, 2, 2, 2, 0, 2 },
	{ 2, 1, 0, 2, 2, 0, 2, 0, 0, 2, 2, 0, 2 },
	{ 2, 2, 0, 0, 2, 0, 0, 0, 0, 0, 2, 0, 2 },
	{ 2, 2, 0, 2, 2, 2, 2, 2, 2, 0, 2, 0, 2 },
	{ 2, 0, 0, 2, 0, 0, 0, 0, 0, 0, 2, 0, 2 },
	{ 2, 0, 0, 2, 0, 2, 2, 2, 2, 2, 2, 2, 2 },
	{ 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2 },
	{ 2, 2, 2, 2, 2, 0, 2, 2, 2, 2, 2, 0, 2 },
	{ 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2 }
};

///////////////////////////////// �迭 �ϵ� �ڵ� ////////////////////////////////////////

void save(maze *maze, FILE *fout)
{
	unsigned int i;
	unsigned int j;
	for (i = 0; i < maze->size.y; i++)
	{
		for (j = 0; j < maze->size.y; j++)
		{
			switch (DARR(maze->map, i, j, maze->size.x)) // map[i][j]
			{
			case EMPTY:
				fprintf(fout, "��"); // ������ �� �ִ� ����
				break;
			case EXIT:
				fprintf(fout, "��"); //�ⱸ
				break;
			case ENTRANCE:
				fprintf(fout, "��"); //�Ա�
				break;
			case WALL:
				fprintf(fout, "��"); // ������ �� ���� ��
				break;
			case CAMEIN:
				fprintf(fout, "��"); // �ùٸ� �� ���� ������ ��
				break;
			case WENTBACK:
				fprintf(fout, "��"); //������ �� �ִ� ���ٸ� ��
				break;
			case BRANCH:
				fprintf(fout, "��"); //������
				break;
			} // switch
		} // for j
		fprintf(fout, "\n"); // �� �� ����� ��ģ �� ����
	} // for i
}

int *open(unsigned int height, unsigned int width, char *name) // ������ ���� �̸��� ���ڷ� �ް� �� ������ �о �迭�� ��ȯ�Ѵ�
{
	unsigned int i;
	unsigned int j;
	int ch;
	int *map = (int *)malloc(sizeof(int)*height*width); // �迭 ���� �Ҵ�
	FILE *fp = fopen(name, "r"); // ���� ����
	//���� �����

	if (!fp)
	{
		printf("\n�б� ����\n\n");
		exit(1);
	}
	else
	{
		for (i = 0; i < height; i++)
		{
			for (j = 0; j < width; j++)
			{
				ch = fgetc(fp);
				sscanf(&ch, "%d", (map + i*width + j)); // (map + i*width + j) => &map[i][j] // ���ڸ� ���ڷ� ��ȯ�ؼ� ����
				if (j == width - 1) // �ٹٲ� ����
				{
					fgetc(fp);
				}
			}
		}
	}
	fclose(fp);
	return map; //������ �о �������� ������ �迭�� ������ �� ��ȯ
}

///////////////////////////////// ���� ����� ////////////////////////////////////////

void menu(int sel)
{
	switch (sel)
	{
	case 0:
		printf("\n------------�޴�-------------\n");
		printf("1. �̷� �ڵ�����\n");
		printf("2. �̷� �ҷ�����\n");
		printf("\n");
		printf("9. ȭ�� ����\n");
		printf("\n");
		printf("0. ����\n");
		printf("------------�Ѥ�-------------\n");
		printf("���� >> ");
		break;
	case 2:
		printf("\n------------�̷� �ҷ�����-------------\n");
		printf("1. ���Ͽ��� �ҷ�����\n");
		printf("2. �ϵ��ڵ��� �迭 �ҷ�����\n");
		printf("\n");
		printf("9. ȭ�� ����\n");
		printf("\n");
		printf("0. �ڷ�\n");
		printf("------------�Ѥ�-�ѤѤѤ�-------------\n");
		printf("���� >> ");
		break;
	case 22:
		printf("\n--------------\n");
		printf("-> 1\n");
		printf("-> 2\n");
		printf("--------------\n");
		printf("���� >> ");
		break;
	}
}

int main(void)
{
	FILE *fout = NULL;
	stacktype mystack;
	int sel;
	int temp;
	point entrance = { 1, 1 };
	char text[100]; //�ִ� 100����Ʈ ���ڿ�
	maze mymaze;
	maze maze1;
	maze maze2;
	//���� ����

	maze1.map = map1;
	maze1.size.y = 10;
	maze1.size.x = 10;
	maze2.map = map2;
	maze2.size.y = 11;
	maze2.size.x = 13;

	while (1)
	{
		menu(0);
		fflush(stdin); // ���� ���
		scanf("%d", &sel);
		switch (sel)
		{
		case 1:
			do
			{
				printf("\n���� ���� (Ȧ�� �Է�) > ");
				fflush(stdin);
				scanf("%d", &mymaze.size.y);
			} while ((signed)mymaze.size.y < 3 || mymaze.size.y % 2 == 0);
			do
			{
				printf("���� ���� (Ȧ�� �Է�) > ");
				fflush(stdin);
				scanf("%d", &mymaze.size.x);
			} while ((signed)mymaze.size.x < 3 || mymaze.size.x % 2 == 0); //�Է�
			mymaze.map = cell(mymaze.size.y, mymaze.size.x); // �� ����
			entrance.x = ((rand() % ((mymaze.size.x - 1) / 2)) * 2) + 1;
			entrance.y = ((rand() % ((mymaze.size.y - 1) / 2)) * 2) + 1; // �Ա��� ���� ����
			gen(&mystack, &mymaze, entrance); // �� �ձ�
			go(&mystack, &mymaze); // Ž��
			while (1) // ������ ������ �����
			{
				printf("����� ���Ϸ� �����մϱ�? Y/N : ");
				fflush(stdin);
				scanf("%c", &sel);
				if (sel == 'Y' || sel == 'y')
				{
					printf("\n���ϸ� �Է�: ");
					fflush(stdin);
					scanf("%s", text);
					fout = fopen(text, "w");
					save(&mymaze, fout); // ����
					fclose(fout);
					printf("\n [���� �Ϸ�]\n\n");
					break;
				}
				else if (sel == 'N' || sel == 'n')
				{
					printf("\n");
					break;
				}
			}//while
			free(mymaze.map);
			break;
		case 2:
			while (sel != 0)
			{
				menu(2);
				fflush(stdin);
				scanf("%d", &sel);
				switch (sel)
				{
				case 1:
					printf("\n���� ���� > ");
					fflush(stdin);
					scanf("%d", &mymaze.size.y);
					printf("���� ���� > ");
					fflush(stdin);
					scanf("%d", &mymaze.size.x);
					printf("���ϸ� > ");
					fflush(stdin);
					scanf("%s", text);
					//�Է�

					mymaze.map = open(mymaze.size.y, mymaze.size.x, text); // ���� �Ҵ�
					go(&mystack, &mymaze);
					//����
					
					while (1) // ������ ������ �����
					{
						printf("����� ���Ϸ� �����մϱ�? Y/N : ");
						fflush(stdin);
						scanf("%c", &sel);
						if (sel == 'Y' || sel == 'y')
						{
							printf("\n���ϸ� �Է�: ");
							fflush(stdin);
							scanf("%s", text);
							fout = fopen(text, "w");
							save(&mymaze, fout); // ����
							fclose(fout);
							printf("\n [���� �Ϸ�]\n\n");
							break;
						}
						else if (sel == 'N' || sel == 'n')
						{
							printf("\n");
							break;
						}
					}//while

					free(mymaze.map); // �޸� ����
					break;
				case 2:
					while (1) // 1, 2 �Է� �޾Ƽ� ����
					{
						menu(22);
						fflush(stdin);
						scanf("%d", &sel);
						if (sel == 1)
						{
							temp = 1;
							go(&mystack, &maze1);
							break;
						}
						else if (sel == 2)
						{
							temp = 2;
							go(&mystack, &maze2);
							break;
						}
					} //while
					while (1) // ������ ������ �����
					{
						printf("����� ���Ϸ� �����մϱ�? Y/N : ");
						fflush(stdin);
						scanf("%c", &sel);
						if (sel == 'Y' || sel == 'y')
						{
							printf("\n���ϸ� �Է�: ");
							fflush(stdin);
							scanf("%s", text);
							fout = fopen(text, "w");
							if (temp == 1) // �ռ� �� �̷θ� ����
							{
								save(&maze1, fout);
							}
							else if (temp == 2)
							{
								save(&maze2, fout);
							}
							fclose(fout);
							printf("\n [���� �Ϸ�]\n\n");
							break;
						}
						else if (sel == 'N' || sel == 'n')
						{
							printf("\n");
							break;
						}
					}//while
					reset(&maze1);
					reset(&maze2);
					break;
				case 9:
					system("CLS");
					break;
				case 0:
					menu(0);
					break;
				default:
					printf("\n�߸��� �����Դϴ�: %d\n\n", sel);
					break;
				}//switch
			}//while (sel != 0)
		case 9:
			system("CLS");
			break;
		case 0:
			puts(""); printf("\n Coded by 2012154021 \n"); puts("");
			return 0;
		default:
			printf("\n�߸��� �����Դϴ�: %d\n\n", sel);
			break;
		}//switch
	}//while
}//main()
