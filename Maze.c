#define _CRT_SECURE_NO_WARNINGS
#define FALSE 0
#define TRUE !FALSE
#define DARR(name, y, x, width) (*(name + y*width + x)) // 매크로 함수 // name[y][x]
// 용법:
// DARR(map,세로,가로) => map[세로][가로]
// ex) map[3][4] => map 배열의 좌표 (4,3)의 요소
// 부연:
// 이차원 배열 요소를 단일 포인터로 표현한 것이다.
// 이것이 필요한 이유는 함수에서 이차원 배열인 미로를 인자로 받을 때 그 배열의 정확한 가로 크기를 미리 알 수 없으므로
// 이중 포인터 대신 단일 포인터(int *map)를 매개변수로 사용하고 있기 때문이다.

#include "stdio.h" //기본 입출력
#include "stdlib.h" //malloc, exit

///////////////////////////////// 전처리 ////////////////////////////////////////

enum{ EMPTY = 0, EXIT = 1, WALL = 2, ENTRANCE = 3, CAMEIN = 4, WENTBACK = 5, BRANCH = 6 }; // SPACE TYPE // 2 이상인 곳은 지나갈 수 없다 // 지나온 길은 4 이상으로 표시된다
enum{ UNKNOWN = 0, UP = 1 , DOWN = 2, RIGHT = 3, LEFT = 4 }; // 미로 생성시 사용

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

///////////////////////////////// 구조체 ////////////////////////////////////////

void init_stacktype(stacktype *s)
{
	s->top = NULL;
}

void push(stacktype *s, element data)
{
	stacknode *temp = (stacknode *)malloc(sizeof(stacknode));
	if (!temp)
	{
		printf("\n메모리 부족\n\n");
		system("pause");
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
		printf("\n오류: 비어 있는 스택에 POP 시도\n\n");
		system("pause");
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
				printf("　"); // 지나갈 수 있는 공간
				break;
			case EXIT:
				printf("出"); //출구
				break;
			case ENTRANCE:
				printf("入"); //입구
				break;
			case WALL:
				printf("■"); // 지나갈 수 없는 벽
				break;
			case CAMEIN:
				printf("○"); // 올바른 길 따라 지나온 곳
				break;
			case WENTBACK:
				printf("◎"); //지나온 적 있는 막다른 길
				break;
			case BRANCH:
				printf("☆"); //갈림길
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

point find(maze *maze, int lookingfor)
{
	unsigned int i;
	unsigned int j;
	point here;
	for (i = 0; i < maze->size.y; ++i)
	{
		for (j = 0; j < maze->size.x; ++j)
		{
			if (DARR(maze->map, i, j, maze->size.x) == lookingfor) // map[i][j] == lookingfor // 찾는 값이면
			{
				here.x = j;
				here.y = i;
				return here; // 그 지점을 반환
			}
		} // for j
	} // for i
	printf("\n잘못된 미로: 요구되는 지점을 찾을 수 없습니다.\n");
	system("pause");
	exit(1); 
}

void go(stacktype *s, maze *maze) // 한 칸씩 더듬어서 길 찾기
{
	point spot = find(maze, ENTRANCE); // 진행 좌표를 담아둘 변수 선언 // 출발 지점 탐색
	point entrance = spot; // 반환할 값 저장 (출발 지점)
	int count;
	init_stacktype(s); // 스택 초기화
	system("cls"); // 화면 청소
	while (DARR(maze->map, spot.y, spot.x, maze->size.x) != EXIT) // map[spot.y][spot.x] != 출구를 찾을 때까지 반복
	{
		switch (DARR(maze->map, spot.y, spot.x, maze->size.x)) // 지나온 곳을 표시 헨젤과 그레텔
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
				)//그 칸의 사방 어디에도 빈 공간이 없다면 돌아왔다는 표시를 남긴다. 즉 갈래길에서는 표시하지 않는다.
			{
				DARR(maze->map, spot.y, spot.x, maze->size.x) = WENTBACK;
			}
			else // 전에 왔던 길이지만 다른 방향에 빈 공간이 있는 경우 갈래길이라는 표시를 남긴다.
			{
				DARR(maze->map, spot.y, spot.x, maze->size.x) = BRANCH;
			}
			break;
		}//switch
		display(maze); // 그림 출력
		show(s); // 자취 출력
		if (DARR(maze->map, (spot.y - 1), spot.x, maze->size.x) < 2) // 가려는 쪽에 벽이나 들어온 흔적이 없다면 그 쪽으로 가고 그 위치를 기억(push)한다.
		{
			spot.y--;
			push(s, spot);
		}//상
		else if (DARR(maze->map, spot.y, (spot.x + 1), maze->size.x) < 2)
		{
			spot.x++;
			push(s, spot);
		}//우
		else if (DARR(maze->map, (spot.y + 1), spot.x, maze->size.x) < 2) 
		{
			spot.y++;
			push(s, spot);
		}//하
		else if (DARR(maze->map, spot.y, (spot.x - 1), maze->size.x) < 2)
		{
			spot.x--;
			push(s, spot);
		}//좌
		else
		{
			spot = pop(s);
		}//막다른 곳인 경우 그 기억은 지운다 (pop)
		system("pause"); // 한 차례를 끝내고 입력을 기다린다
		system("cls"); // 다음 차례 시작
	} // while
	display(maze);
	printf("\n\n<<<<< 목적지 도착 !! >>>>>\n\n\n");
	stackfree(s);
}

void reset(maze *maze) // 미로를 지나온 표시 제거
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

///////////////////////////////// 미로 탐색 함수 ////////////////////////////////////////

/*
이하 미로 생성 함수
간략한 설명:
평면(2차원) 미로를 사방이 가로막힌 네모난 방들의 집합으로 표현한다.
각각의 방은 사방(동서남북 or 상하좌우) 4개의 격벽으로 둘러싸여 있다.
인접한 방끼리 길을 트려면 현재 방의 출구이자 다음 방의 입구가 될 위치의 벽을 없앤다.
예를 들어 다음 방이 현재 방의 오른쪽에 있다고 하면 현재 방의 오른쪽 벽이자 다음 방의 왼쪽이 되는 벽을 삭제하는 것이다.
*/

int is_closed(maze *maze, unsigned int x, unsigned int y) // 그 방의 사방이 닫혀 있다
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

int *cell(unsigned int height, unsigned int width) // 크기에 맞춰 동적 생성하고 방을 만든다. 미로 생성에 앞선 지도 초기화 작업.
{
	unsigned int i;
	unsigned int j;
	int *map = (int *)malloc(sizeof(int)*height*width);
	for (i = 0; i < height; i++)
	{
		for (j = 0; j < width; j++)
		{
			if (i % 2 == 0 || j % 2 == 0) // 짝수의 행이거나 열이면 벽을 세워서 방을 만든다. 지도 전반을 # 모양으로 만든다.
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
{ // 미로 생성의 원리는 미로 탐색과 같다. 다만 갈림길에서 어느 방향으로 진행할지를 무작위로 결정한다. 탐색 함수에서는 갈 수 있는 길을 상우하좌(시계방향) 순으로 확인했다.
	unsigned int visit = 1; // 방문 횟수 (길을 틔운 횟수)
	point neighbour[4];
	int kind[4] = { 0, 0, 0, 0 };
	int index = 0;
	int random; // 이상은 이웃한 방에 대한 정보
	point entrance = spot;
	init_stacktype(s); // 스택을 사용한다
	while (visit < ((maze->size.x - 1) / 2)*((maze->size.y - 1) / 2)) //방의 수 = ((가로-1 )/2) * ((세로-1)/2) // 모든 방을 방문할 때까지 반복
	{
		index = 0;
		if ((signed)spot.y - 2 > 0 && is_closed(maze, spot.x, (spot.y - 2)))
		{
			neighbour[index].x = spot.x;
			neighbour[index].y = spot.y - 2;
			kind[index++] = UP;
		} //상
		if (spot.y + 2 < maze->size.y && is_closed(maze, spot.x, (spot.y + 2)))
		{
			neighbour[index].x = spot.x;
			neighbour[index].y = spot.y + 2;
			kind[index++] = DOWN;
		} //하
		if ((signed)spot.x - 2 > 0 && is_closed(maze, (spot.x - 2), spot.y))
		{
			neighbour[index].x = spot.x - 2;
			neighbour[index].y = spot.y;
			kind[index++] = LEFT;
		} //좌
		if (spot.x + 2 < maze->size.x && is_closed(maze, (spot.x + 2), spot.y))
		{
			neighbour[index].x = spot.x + 2;
			neighbour[index].y = spot.y;
			kind[index++] = RIGHT;
		} //우
		// 현재 방에서 이웃한 방 중 손대지 않은 방을 찾고 선택지에 추가한다
		if (index == 0) // 손대지 않은 이웃한 방이 없다면 되돌아간다
		{
			spot = pop(s);
		}
		else
		{
			random = rand() % index; // 후보로 기억해 둔 방 중 하나를 무작위로 하나 선택해서 길을 연결한다
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
	printf("\n생성완료\n\n");
}//gen()

///////////////////////////////// 미로 생성 함수 ////////////////////////////////////////

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

///////////////////////////////// 배열 하드 코딩 ////////////////////////////////////////

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
				fprintf(fout, "　"); // 지나갈 수 있는 공간
				break;
			case EXIT:
				fprintf(fout, "出"); //출구
				break;
			case ENTRANCE:
				fprintf(fout, "入"); //입구
				break;
			case WALL:
				fprintf(fout, "■"); // 지나갈 수 없는 벽
				break;
			case CAMEIN:
				fprintf(fout, "○"); // 올바른 길 따라 지나온 곳
				break;
			case WENTBACK:
				fprintf(fout, "◎"); //지나온 적 있는 막다른 길
				break;
			case BRANCH:
				fprintf(fout, "☆"); //갈림길
				break;
			} // switch
		} // for j
		fprintf(fout, "\n"); // 한 줄 출력을 마친 후 개행
	} // for i
}

int *open(unsigned int height, unsigned int width, char *name) // 열려는 파일 이름을 인자로 받고 그 파일을 읽어서 배열로 반환한다
{
	unsigned int i;
	unsigned int j;
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
		printf("\n------------메뉴-------------\n");
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
		printf("\n------------미로 불러오기-------------\n");
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
	point entrance = { 1, 1 };
	char text[100]; //최대 100바이트 문자열
	maze mymaze;
	maze maze1;
	maze maze2;
	//변수 선언

	maze1.map = map1;
	maze1.size.y = 10;
	maze1.size.x = 10;
	maze2.map = map2;
	maze2.size.y = 11;
	maze2.size.x = 13;

	while (1)
	{
		menu(0);
		fflush(stdin); // 버퍼 비움
		scanf("%d", &sel);
		switch (sel)
		{
		case 1:
			do
			{
				printf("\n행의 갯수 (홀수 입력) > ");
				fflush(stdin);
				scanf("%d", &mymaze.size.y);
			} while ((signed)mymaze.size.y < 3 || mymaze.size.y % 2 == 0);
			do
			{
				printf("열의 갯수 (홀수 입력) > ");
				fflush(stdin);
				scanf("%d", &mymaze.size.x);
			} while ((signed)mymaze.size.x < 3 || mymaze.size.x % 2 == 0); //입력
			mymaze.map = cell(mymaze.size.y, mymaze.size.x); // 방 생성
			entrance.x = ((rand() % ((mymaze.size.x - 1) / 2)) * 2) + 1;
			entrance.y = ((rand() % ((mymaze.size.y - 1) / 2)) * 2) + 1; // 입구를 임의 설정
			gen(&mystack, &mymaze, entrance); // 길 뚫기
			go(&mystack, &mymaze); // 탐색
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
					save(&mymaze, fout); // 저장
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
					printf("\n행의 갯수 > ");
					fflush(stdin);
					scanf("%d", &mymaze.size.y);
					printf("열의 갯수 > ");
					fflush(stdin);
					scanf("%d", &mymaze.size.x);
					printf("파일명 > ");
					fflush(stdin);
					scanf("%s", text);
					//입력

					mymaze.map = open(mymaze.size.y, mymaze.size.x, text); // 동적 할당
					go(&mystack, &mymaze);
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
							save(&mymaze, fout); // 저장
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

					free(mymaze.map); // 메모리 해제
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
								save(&maze1, fout);
							}
							else if (temp == 2)
							{
								save(&maze2, fout);
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
					printf("\n잘못된 선택입니다: %d\n\n", sel);
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
			printf("\n잘못된 선택입니다: %d\n\n", sel);
			break;
		}//switch
	}//while
}//main()
