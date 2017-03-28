#define _CRT_SECURE_NO_WARNINGS
#define FALSE 0
#define TRUE !FALSE
#define DARR(name, y, x) (*(name + y*width + x)) // 매크로 함수
// 용법:
// ARR(map,세로,가로) => map[세로][가로]
// ex) map[3][4] => map 배열의 좌표 (4,3)의 요소
// 부연:
// 이차원 배열 요소를 단일 포인터로 표현한 것이다.
// 이것이 필요한 이유는 함수에서 이차원 배열인 미로를 인자로 받을 때 그 배열의 정확한 가로 크기를 미리 알 수 없으므로
// 이중 포인터 대신 단일 포인터(int *map)를 매개변수로 사용하고 있기 때문이다.

#include "stdio.h" //기본 입출력
#include "stdlib.h" //malloc, exit

///////////////////////////////// 전처리 ////////////////////////////////////////

enum{ EMPTY = 0, EXIT = 1, WALL = 2, CAMEIN = 3, WENTBACK = 4}; // SPACE TYPE // 2 이상인 곳은 지나갈 수 없다 // 지나온 길은 3 이상으로 표시된다

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

///////////////////////////////// 스택 구조체 ////////////////////////////////////////

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
	} //할당 불가
	if (s->top != NULL)
	{
		temp->data = data;
		temp->link = s->top;
	}
	else
	{
		temp->data = data;
		temp->link = NULL;
	} // 빈 스택인 경우
	s->top = temp;	// 동적 생성, 초기화, top 설정
}

element pop(stacktype *s)
{
	if (s->top == NULL) 
	{
		printf("스택이 비어 있음\n");
		exit(1);
	}
	else
	{
		stacknode *temp = s->top; //삭제할 요소 저장
		element data = temp->data; //얻으려는 값을 저장
		s->top = s->top->link;
		free(temp);
		return data; //링크 설정, 삭제, 반환
	}
}

int stackfree(stacktype *s) //스택 비우기
{
	stacknode *temp = s->top; // 삭제할 노드
	if (s->top == NULL) // 이미 빈 스택인 경우
	{
		return FALSE;
	}
	while (s->top != NULL) // 텅 빈 스택이 될 때까지 반복
	{
		s->top = s->top->link; // top 한 칸 내림
		free(temp); // 동적 메모리 해제
		temp = s->top; // 다음 삭제 대상 설정
	}
	return TRUE;
}// 비워지면 TRUE, 이미 빈 스택이면 FALSE 반환

///////////////////////////////// 스택 함수 ////////////////////////////////////////

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
				printf("□"); // 지나갈 수 있는 공간
				break;
			case EXIT:
				printf("出"); //출구
				break;
			case WALL:
				printf("■"); // 지나갈 수 없는 벽
				break;
			case CAMEIN:
				printf("入"); //입구, 올바른 길 따라 지나온 곳
				break;
			case WENTBACK:
				printf("○"); //지나온 적 있는 막다른 길
				break;
			} // switch
		} // for j
		printf("\n"); // 한 줄 출력을 마친 후 개행
	} // for i
}

void show(stacktype *s) // 스택을 전부 peek해서 지나온 자취를 좌표로 표시
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
			if (DARR(map,i,j) == lookingfor) // map[i][j] == lookingfor // 찾는 값이면
			{
				here.x = j;
				here.y = i;
				return here; // 그 지점을 반환
			}
		} // for j
	} // for i
	printf("\n잘못된 미로\n");
	system("pause");
	exit(1); 
}

point go(stacktype *s, unsigned int height, unsigned int width, int *map) // 한 칸씩 더듬어서 길 찾기
{
	point spot = find(height, width, map, CAMEIN); // 진행 좌표를 담아둘 변수 선언 // 출발 지점 탐색
	point entrance = spot; // 반환할 값 저장 (출발 지점)
	init_stacktype(s); // 스택 초기화
	push(s, spot); // 출발 지점 푸시
	system("cls"); // 화면 청소
	DARR(map, spot.y, spot.x) = EMPTY; // 출발 지점에 출발 표시를 남기도록
	while (DARR(map, spot.y, spot.x) != EXIT) // map[spot.y][spot.x] != 출구를 찾을 때까지 반복
	{
		switch (DARR(map, spot.y, spot.x)) // 지나온 곳을 표시 헨젤과 그레텔
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
				)//그 칸의 사방 어디에도 빈 공간이 없다면 동그라미 표시를 남긴다. 즉 갈래길에서는 표시하지 않는다.
			{
				DARR(map, spot.y, spot.x) = WENTBACK;
			}
			break;
		}
		display(height, width, map); // 그림 출력
		show(s); // 자취 출력
		if (DARR(map, (spot.y + 1), spot.x) < 2) // 가려는 쪽에 벽이나 들어온 흔적이 없다면 그 쪽으로 간다.
		{
			spot.y++;
			push(s, spot);
		}//하
		else if (DARR(map, (spot.y - 1), spot.x) < 2)
		{
			spot.y--;
			push(s, spot);
		}//상
		else if (DARR(map, spot.y, (spot.x - 1)) < 2)
		{
			spot.x--;
			push(s, spot);
		}//좌
		else if (DARR(map, spot.y, (spot.x + 1)) < 2)
		{
			spot.x++;
			push(s, spot);
		}//우
		else
		{
			spot = pop(s);
		}//막다른 곳
		system("pause"); // 한 차례를 끝내고 입력을 기다린다
		system("cls"); // 다음 차례 시작
	} // while
	display(height, width, map);
	printf("\n\n<<<<< 목적지 도착 !! >>>>>\n\n\n");
	stackfree(s);
	return entrance;
}

void reset(unsigned int height, unsigned int width, int *map, point entrance) // 미로를 지나온 표시 제거
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

///////////////////////////////// 미로 찾기 함수 ////////////////////////////////////////

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

///////////////////////////////// 배열 하드 코딩 ////////////////////////////////////////

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
				fprintf(fout,"□"); // 지나갈 수 있는 공간
				break;
			case EXIT:
				fprintf(fout, "出"); //출구
				break;
			case WALL:
				fprintf(fout, "■"); // 지나갈 수 없는 벽
				break;
			case CAMEIN:
				fprintf(fout, "入"); //입구, 올바른 길 따라 지나온 곳
				break;
			case WENTBACK:
				fprintf(fout, "○"); //지나온 적 있는 막다른 길
				break;
			} // switch
		} // for j
		fprintf(fout, "\n"); // 한 줄 출력을 마친 후 개행
	} // for i
}

int *open(unsigned int height, unsigned int width, char *name) // 열려는 파일 이름을 인자로 받고 그 파일을 읽어서 배열로 반환한다
{
	int i;
	int j;
	int ch;
	int *map = (int *)malloc(sizeof(int)*height*width); // 배열 동적 할당
	FILE *fp = fopen(name, "r"); // 파일 열기
	//변수 선언부

	if (!fp)
	{
		printf("\n읽기 오류\n\n");
		exit(1);
	}
	else
	{
		for (i = 0; i < height; i++)
		{
			for (j = 0; j < width; j++)
			{
				ch = fgetc(fp);
				sscanf(&ch, "%d", (map + i*width + j)); // (map + i*width + j) => &map[i][j] // 문자를 숫자로 변환해서 저장
				if (j == width - 1) // 줄바꿈 문자
				{
					fgetc(fp);
				}
			}
		}
	}
	fclose(fp);
	return map; //파일을 읽어서 동적으로 생성된 배열에 복사한 후 반환
}

///////////////////////////////// 파일 입출력 ////////////////////////////////////////

void menu(int sel)
{
	switch (sel)
	{
	case 0:
		printf("------------메뉴-------------\n");
		printf("1. 미로 자동생성\n");
		printf("2. 미로 불러오기\n");
		printf("\n");
		printf("9. 화면 정리\n");
		printf("\n");
		printf("0. 종료\n");
		printf("------------ㅡㅡ-------------\n");
		printf("선택 >> ");
		break;
	case 2:
		printf("------------미로 불러오기-------------\n");
		printf("1. 파일에서 불러오기\n");
		printf("2. 하드코딩된 배열 불러오기\n");
		printf("\n");
		printf("9. 화면 정리\n");
		printf("\n");
		printf("0. 뒤로\n");
		printf("------------ㅡㅡ-ㅡㅡㅡㅡ-------------\n");
		printf("선택 >> ");
		break;
	case 22:
		printf("\n--------------\n");
		printf("-> 1\n");
		printf("-> 2\n");
		printf("--------------\n");
		printf("선택 >> ");
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
	//변수 선언

	while (1)
	{
		menu(0);
		fflush(stdin); // 버퍼 비움
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
					printf("\n행의 갯수 > ");
					fflush(stdin);
					scanf("%d", &height);
					printf("열의 갯수 > ");
					fflush(stdin);
					scanf("%d", &width);
					printf("파일명 > ");
					fflush(stdin);
					scanf("%s", text);
					//입력

					map = open(height, width, text);
					go(&mystack, height, width, map);
					//열기
					
					while (1) // 저장할 것인지 물어보기
					{
						printf("결과를 파일로 저장합니까? Y/N : ");
						fflush(stdin);
						scanf("%c", &sel);
						if (sel == 'Y' || sel == 'y')
						{
							printf("\n파일명 입력: ");
							fflush(stdin);
							scanf("%s", text);
							fout = fopen(text, "w");
							save(height, width, map, fout); // 저장
							fclose(fout);
							printf("\n [저장 완료]\n\n");
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
					while (1) // 1, 2 입력 받아서 실행
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
					while (1) // 저장할 것인지 물어보기
					{
						printf("결과를 파일로 저장합니까? Y/N : ");
						fflush(stdin);
						scanf("%c", &sel);
						if (sel == 'Y' || sel == 'y')
						{
							printf("\n파일명 입력: ");
							fflush(stdin);
							scanf("%s", text);
							fout = fopen(text, "w");
							if (temp == 1) // 앞서 고른 미로를 저장
							{
								save(10, 10, map1, fout);
							}
							else if (temp == 2)
							{
								save(11, 13, map2, fout);
							}
							fclose(fout);
							printf("\n [저장 완료]\n\n");
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
					printf("\n잘못된 선택입니다: %d\n\n", sel);
					break;
				}//switch
			}//while (sel != 0)
		case 9:
			system("CLS");
			break;
		case 0:
			return 0;
		default:
			printf("\n잘못된 선택입니다: %d\n\n", sel);
			break;
		}//switch
	}//while

	return 0;
}
