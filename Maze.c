#define _CRT_SECURE_NO_WARNINGS
#define FALSE 0
#define TRUE !FALSE
#define DARR(name, y, x) (*(name + y*width + x)) // ��ũ�� �Լ�
// ���:
// ARR(map,����,����) => map[����][����]
// ex) map[3][4] => map �迭�� ��ǥ (4,3)�� ���
// �ο�:
// ������ �迭 ��Ҹ� ���� �����ͷ� ǥ���� ���̴�.
// �̰��� �ʿ��� ������ �Լ����� ������ �迭�� �̷θ� ���ڷ� ���� �� �� �迭�� ��Ȯ�� ���� ũ�⸦ �̸� �� �� �����Ƿ�
// ���� ������ ��� ���� ������(int *map)�� �Ű������� ����ϰ� �ֱ� �����̴�.

#include "stdio.h" //�⺻ �����
#include "stdlib.h" //malloc, exit

///////////////////////////////// ��ó�� ////////////////////////////////////////

enum{ EMPTY = 0, EXIT = 1, WALL = 2, CAMEIN = 3, WENTBACK = 4}; // SPACE TYPE // 2 �̻��� ���� ������ �� ���� // ������ ���� 3 �̻����� ǥ�õȴ�

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

///////////////////////////////// ���� ����ü ////////////////////////////////////////

void init_stacktype(stacktype *s)
{
	s->top = NULL;
}

void push(stacktype *s, element data)
{
	stacknode *temp = (stacknode *)malloc(sizeof(stacknode));
	if (!temp)
	{
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
		printf("������ ��� ����\n");
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

void display(unsigned int height, unsigned int width, int *map)
{
	unsigned int i;
	unsigned int j;
	for (i = 0; i < height; ++i)
	{
		for (j = 0; j < width; ++j)
		{
			switch (DARR(map, i, j)) // map[i][j]
			{
			case EMPTY:
				printf("��"); // ������ �� �ִ� ����
				break;
			case EXIT:
				printf("��"); //�ⱸ
				break;
			case WALL:
				printf("��"); // ������ �� ���� ��
				break;
			case CAMEIN:
				printf("��"); //�Ա�, �ùٸ� �� ���� ������ ��
				break;
			case WENTBACK:
				printf("��"); //������ �� �ִ� ���ٸ� ��
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

point find(unsigned int height, unsigned int width, int *map, int lookingfor)
{
	unsigned int i;
	unsigned int j;
	point here;
	for (i = 0; i < height; ++i)
	{
		for (j = 0; j < width; ++j)
		{
			if (DARR(map,i,j) == lookingfor) // map[i][j] == lookingfor // ã�� ���̸�
			{
				here.x = j;
				here.y = i;
				return here; // �� ������ ��ȯ
			}
		} // for j
	} // for i
	printf("\n�߸��� �̷�\n");
	system("pause");
	exit(1); 
}

point go(stacktype *s, unsigned int height, unsigned int width, int *map) // �� ĭ�� ���� �� ã��
{
	point spot = find(height, width, map, CAMEIN); // ���� ��ǥ�� ��Ƶ� ���� ���� // ��� ���� Ž��
	point entrance = spot; // ��ȯ�� �� ���� (��� ����)
	init_stacktype(s); // ���� �ʱ�ȭ
	push(s, spot); // ��� ���� Ǫ��
	system("cls"); // ȭ�� û��
	DARR(map, spot.y, spot.x) = EMPTY; // ��� ������ ��� ǥ�ø� ���⵵��
	while (DARR(map, spot.y, spot.x) != EXIT) // map[spot.y][spot.x] != �ⱸ�� ã�� ������ �ݺ�
	{
		switch (DARR(map, spot.y, spot.x)) // ������ ���� ǥ�� ������ �׷���
		{
		case EMPTY:
			DARR(map, spot.y, spot.x) = CAMEIN;
			break;
		case CAMEIN:
			if
				(
				DARR(map, (spot.y + 1), spot.x) > 1
				&& DARR(map, (spot.y - 1), spot.x) > 1
				&& DARR(map, spot.y, (spot.x - 1)) > 1
				&& DARR(map, spot.y, (spot.x + 1)) > 1
				)//�� ĭ�� ��� ��𿡵� �� ������ ���ٸ� ���׶�� ǥ�ø� �����. �� �����濡���� ǥ������ �ʴ´�.
			{
				DARR(map, spot.y, spot.x) = WENTBACK;
			}
			break;
		}
		display(height, width, map); // �׸� ���
		show(s); // ���� ���
		if (DARR(map, (spot.y + 1), spot.x) < 2) // ������ �ʿ� ���̳� ���� ������ ���ٸ� �� ������ ����.
		{
			spot.y++;
			push(s, spot);
		}//��
		else if (DARR(map, (spot.y - 1), spot.x) < 2)
		{
			spot.y--;
			push(s, spot);
		}//��
		else if (DARR(map, spot.y, (spot.x - 1)) < 2)
		{
			spot.x--;
			push(s, spot);
		}//��
		else if (DARR(map, spot.y, (spot.x + 1)) < 2)
		{
			spot.x++;
			push(s, spot);
		}//��
		else
		{
			spot = pop(s);
		}//���ٸ� ��
		system("pause"); // �� ���ʸ� ������ �Է��� ��ٸ���
		system("cls"); // ���� ���� ����
	} // while
	display(height, width, map);
	printf("\n\n<<<<< ������ ���� !! >>>>>\n\n\n");
	stackfree(s);
	return entrance;
}

void reset(unsigned int height, unsigned int width, int *map, point entrance) // �̷θ� ������ ǥ�� ����
{
	unsigned int i;
	unsigned int j;
	for (i = 0; i < height; i++)
	{
		for (j = 0; j < width; j++)
		{
			if (DARR(map, i, j) > 2)
			{
				DARR(map, i, j) = EMPTY;
			}
		}
	}
	DARR(map, entrance.y, entrance.x) = CAMEIN;
}

///////////////////////////////// �̷� ã�� �Լ� ////////////////////////////////////////

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
	{ 2, 0, 0, 2, 2, 2, 2, 2, 2, 2, 2, 0, 2 },
	{ 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2 },
	{ 2, 2, 2, 2, 2, 0, 2, 2, 2, 2, 2, 0, 2 },
	{ 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2 }
};

///////////////////////////////// �迭 �ϵ� �ڵ� ////////////////////////////////////////

void save(unsigned int height, unsigned int width, int *map, FILE *fout)
{
	unsigned int i;
	unsigned int j;
	for (i = 0; i < height; i++)
	{
		for (j = 0; j < width; j++)
		{
			switch (DARR(map, i, j)) // map[i][j]
			{
			case EMPTY:
				fprintf(fout,"��"); // ������ �� �ִ� ����
				break;
			case EXIT:
				fprintf(fout, "��"); //�ⱸ
				break;
			case WALL:
				fprintf(fout, "��"); // ������ �� ���� ��
				break;
			case CAMEIN:
				fprintf(fout, "��"); //�Ա�, �ùٸ� �� ���� ������ ��
				break;
			case WENTBACK:
				fprintf(fout, "��"); //������ �� �ִ� ���ٸ� ��
				break;
			} // switch
		} // for j
		fprintf(fout, "\n"); // �� �� ����� ��ģ �� ����
	} // for i
}

int *open(unsigned int height, unsigned int width, char *name) // ������ ���� �̸��� ���ڷ� �ް� �� ������ �о �迭�� ��ȯ�Ѵ�
{
	int i;
	int j;
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
		printf("------------�޴�-------------\n");
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
		printf("------------�̷� �ҷ�����-------------\n");
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
	int height;
	int width;
	point entrance;
	char text[100];
	int *map;
	//���� ����

	while (1)
	{
		menu(0);
		fflush(stdin); // ���� ���
		scanf("%d", &sel);
		switch (sel)
		{
		case 1:
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
					scanf("%d", &height);
					printf("���� ���� > ");
					fflush(stdin);
					scanf("%d", &width);
					printf("���ϸ� > ");
					fflush(stdin);
					scanf("%s", text);
					//�Է�

					map = open(height, width, text);
					go(&mystack, height, width, map);
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
							save(height, width, map, fout); // ����
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
							entrance = go(&mystack, 10, 10, map1);
							break;
						}
						else if (sel == 2)
						{
							temp = 2;
							entrance = go(&mystack, 11, 13, map2);
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
								save(10, 10, map1, fout);
							}
							else if (temp == 2)
							{
								save(11, 13, map2, fout);
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
					reset(10, 10, map1, entrance);
					reset(11, 13, map2, entrance);
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
			return 0;
		default:
			printf("\n�߸��� �����Դϴ�: %d\n\n", sel);
			break;
		}//switch
	}//while

	return 0;
}
