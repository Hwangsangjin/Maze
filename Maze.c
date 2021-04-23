/*
	# 미로찾기

	1) 미로 배열과 Mark 배열을 생성한다. (전역 변수)
		- 미로배열은 1과 0으로 되어 있지만 Mark 배열은 초기값이 다 0이다.

	2) 옮겨온 자취를 남길 Stack 을 생성하고, 관련 함수를 만든다.
		- 좌표와 방향을 남겨야 하므로, x, y, dir을 멤버변수로 한다
		- Pop과 Push 기능을 만들고, Top을 0으로 초기화 한다.

	3) 옮겨갈 조건 검색
		- 검색방향을 차례대로 돌며 그 배열값이 0인지 확인한다.
		- Mark 배열의 값을 확인해 왔던 길인지도 확인한다.
		- 조건이 만족되지 않는다면 후퇴한다.

	4) 이동
		- 이동할 곳의 좌표를 구한다.
		- 자취를 남기기 위해 Stack에 Push를 해둔다.
		- 현재 위치를 옮긴다.

	5) 후퇴
		- Stack에서 Pop을 통해 전 위치로 옮겨간다.
		- 검색은 Pop된 값의 방향에서 부터 시작한다.
*/

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#define MAZESIZE_X	12		// 미로 X size + 2;
#define MAZESIZE_Y	22		// 미로 Y size + 2;
#define EXIT_X		11		// 출구 X 좌표
#define EXIT_Y		16		// 출구 Y 좌표

// 자취를 표현하기 위한 구조체
typedef struct Position
{
	int x;
	int y;
	int d;
} Position;

// 옮길 좌표를 구하기 위한 구조체
typedef struct Mtable
{
	int x;
	int y;
} Mtable;

// 미로 배열 생성 및 초기화
int Maze[MAZESIZE_X][MAZESIZE_Y] =
{
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
	{1,0,0,0,0,0,0,0,0,0,0,1,0,1,0,1,0,1,0,1,0,1},
	{1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1},
	{1,0,0,1,0,1,0,1,0,0,1,1,1,1,1,0,0,1,1,1,0,1},
	{1,0,1,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,1,0,1,1,1,0,1,0,1,1,1,1,1,1,0,1,1,1,1,1,1},
	{1,0,0,0,0,0,0,1,0,0,0,1,0,1,0,1,0,1,0,1,0,1},
	{1,1,1,0,1,1,0,1,1,1,0,1,0,1,0,1,0,1,0,1,0,1},
	{1,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,1,0,0,1,0,1,1,1,0,1,0,0,0,1,0,1,0,1,0,1},
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
};

// 다음 좌표를 구하기 위한 계산표
Mtable Move[4] =
{
	// 아래
	{0,1},
	// 오른쪽
	{1,0},
	// 위로
	{0,-1},
	// 왼쪽
	{-1,0}
};

// Mark 배열 생성 (전역 변수이므로 0으로 초기화)
int Mark[MAZESIZE_X][MAZESIZE_Y];
// Stack의 index
int top;
// 자취를 담기위한 Stack 생성
Position Trace[(MAZESIZE_X - 2) * (MAZESIZE_Y - 2)];
// 미로를 보여준다.
void ShowMaze(Position);
// 자취를 Trace Stack에 입력
void Push(Position);
// Trace Stack의 최상위 값을 Return 후 삭제
void Pop(Position* P);

int main(void)
{
	int isFound = FALSE;
	int i, dir;

	// 미로 입구 및 검색 방향 초기화 
	Position Now = { 1, 1, 0 };
	Position Next;
	// 현재 값으로 스택에 푸쉬후 시작한다.(스택(top)이 1 증가한다.)
	Push(Now);

	// 스택에서 팝업해서 길을 찾을때까지 돌린다.
	// !isFound상태...즉, 아직 도착하지 않는상태이고 두 번째 반복문에서 길이 막힌상태이면 
	// top > 0는 스택에 값이 있는 상태이므로 스택에서 그 전 값을 가져와서 다시 길 찾기
	while (!isFound && top > 0)
	{
		// 시작 반복문에서 Trace 스택에서 값과 방향을 가져온다.
		Pop(&Now);
		dir = Now.d;

		// 총 4방향으로 검색해야한다.
		while (dir < 4)
		{
			// 일단, 현재 위치를 Move(이동 방향으로 1 프레임 이동)
			// 여기서 Now값은 변경 안된다는거에 주의
			Next.x = Now.x + Move[dir].x;
			Next.y = Now.y + Move[dir].y;

			// 만약 탈출구하면 탈출
			if (Next.x == EXIT_X && Next.y == EXIT_Y)
			{
				// Next 구조체의 dir은 탈출 방향이니 그대로 세팅
				Next.d = dir;
				// 탈출 위치를 스택에 넣어둔다.
				Push(Next);
				// 시작 반복문 안타게
				isFound = TRUE;
				// 현재 반복문 나가기
				break;
			}
			// 길이 막히지 않을때...여기서 Now값 변경 및 푸쉬
			else if ((Maze[Next.x][Next.y] == 0) && (Mark[Next.x][Next.y] == 0))
			{
				// 방향값을 1증가하는 이유는?
				// 스택에서 값을 팝업한다는것은 잘못된 길이니까 그 다음 길부터 찾도록한다.
				Now.d = ++dir;
				// 스택에 저장
				Push(Now);

				// 현재값을 변경된 값으로 변경
				Now.x = Next.x;				
				Now.y = Next.y;
				// 다시 검색을 위해 초기화
				dir = 0;
				// 마크 배열도 셋팅
				Mark[Next.x][Next.y] = 1;
			}
			// 그게 아니라면(도착지도 아니고 길막이라면)
			else
				// 갈수 있을때까지 검색
				dir++;

			// 현재 찾는 미로 보여주기(이거 없으면 결과만)
			ShowMaze(Now);
		}
	}
	// 만약 찾았다면 스택에서 값을 하나씩 꺼내면서 길찾기 보여주기
	if (isFound == TRUE)
		for (i = 0; i < top; i++)
			// 스택에서 거꾸로 인덱스 0부터 가져온다.
			ShowMaze(Trace[i]);
	else
		printf("Not Found!\n");
}

// 미로 전체를 보여주는 함수
void ShowMaze(Position P)
{
	int i, j;

	// 1000은 1초
	Sleep(50);
	// 화면 갱신
	system("cls");

	// 현재 위치와 방향
	printf("%d, %d, %d\n", P.x, P.y, P.d);

	for (i = 0; i < MAZESIZE_X; i++)
	{
		for (j = 0; j < MAZESIZE_Y; j++)
		{
			// 벽 #
			// 플레이어는 &
			// 이동 가능구역 ' '
			if (Maze[i][j] == 1) printf("%c", '#');				
			else if (P.x == i && P.y == j) printf("%c", '&');	
			else printf("%c", ' ');													
		}
		printf("\n");
	}
}

// 자취를 Trace Stack에 입력하는 함수
void Push(Position P)
{
	// 값을 넣었으니
	Trace[top].x = P.x;
	Trace[top].y = P.y;
	Trace[top].d = P.d;
	// 다음 인덱스를 참조
	top++;
}

// 이전 자취를 return하는 함수
void Pop(Position* P)
{
	// 하나 가져갔으니
	top--;
	// 스택에서 가장 마지막  들어온 값이 먼저 해제된다.
	// 참조로 호출하는 것이므로 전달된 구조체 Now의 값이 아래처럼 스택의 해제순서로 대체된다.
	P->x = Trace[top].x;
	P->y = Trace[top].y;
	P->d = Trace[top].d;
}