//  2021.06.11
//  main.cpp
//  Wumpus world


/*
******* 자리수*******
지나간위치=9
골드=8
움푸스=7
Pit=6
바람=5
냄새=4
벽=3
확인움푸스=2
확인Pit=1
*/


//헤더*******************************************************************************
#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <time.h>
#include <cstring>
#pragma warning(disable:4996)
using namespace std;

//Define*********************************************************************************
#define MAZE_SIZE 6
#define MAX_SIZE 15


//역ㄹ,shoot, 상황판단, next_cell, move, turn
//maze_print, grab, come_back, die, sensor
//by_pass


//변수*********************************************************************************
int current_state = 512, passed = 256, g = 128,
w = 64, p = 32, breeze = 16,
stench = 8, wall = 4, DW = 2, DP = 1;

//
char wumpus[10] = "W";
char pit[10] = "P";
char char_breeze[10] = "b";
char char_stench[10] = "s";

char up[10] = "^ |";
char right[10] = "> |";
char down[10] = "v |";
char left[10] = "< |";

string start("게임을 시작합니다.");
string get_gold("금을 획득하였습니다.");
string go_back("원점으로 돌아갔습니다.");
string death_of_agent("에이전트가 죽었습니다.");
string shooting("화살을 쐈습니다.");
string turn_left("왼쪽으로 회전");
string turn_right("오른쪽으로 회전");
string go_forward("한칸 앞으로");
string scream("비명!");


int maze[MAZE_SIZE][MAZE_SIZE] = {
    {4, 4, 4, 4, 4, 4},
    {4, 0, 0, 0, 0, 4},
    {4, 0, 0, 0, 0, 4},
    {4, 0, 0, 0, 0, 4},
    {4, 256, 0, 0, 0, 4},
    {4, 4, 4, 4, 4, 4}
};

basic_string<char, char_traits<char>, allocator<char>> maze2[MAZE_SIZE][MAZE_SIZE] = { //문자열 3차원 배열
    {"[ ]", "[ ]", "[ ]", "[ ]", "[ ]", "[ ]"},
    {"[ ]", "U", "U", "U", "U", "[ ]"},
    {"[ ]", "U", "U", "U", "U", "[ ]"},
    {"[ ]", "U", "U", "U", "U", "[ ]"},
    {"[ ]", "U", "U", "U", "U", "[ ]"},
    {"[ ]", "[ ]", "[ ]", "[ ]", "[ ]", "[ ]"}
};

int x = 4, y = 1; //현재좌표
int direction = 2; //위쪽=1, 오른쪽=2, 아래쪽=3, 왼쪽=4



//함수 기본형**************************************************************************
void maze_print_for_us(int maze[MAZE_SIZE][MAZE_SIZE]);
void maze_setting(int maze[MAZE_SIZE][MAZE_SIZE]);
void maze_print(string warning);
void grab();
void come_back();
void die();
int sensor();
void maze_print_none();

//정욱
int en_jud(int a, int b);
void tr();
void tl();
int ABS(int a);
void go_f();
void turn(int nx, int ny);
void move(int nx, int ny);
void shoot();
int next_cell(int e, int c, int d);
void st_course();

//가영
void bypass();
int pit_goal(int e, int c, int d);
int isit_next(int a, int b, int c, int d);



//main함수*****************************************************************
int main() {
    maze_setting(maze);
    maze_print_for_us(maze);
    maze_print(start);
    st_course();
    return 0;
}



//사용자 지정 함수 정의******************************************************
int en_jud(int a, int b) {
    int sum[20] = { 0 };
    for (int i = 0; a != 0; i++) {
        if (a % 2 == 1)
            sum[i] = 1;
        a = a / 2;
    }
    return sum[b - 1];
}

//미로를 화면에 출력한다(완성 전까지 임시 확인용으로 사용)
void maze_print_for_us(int maze[MAZE_SIZE][MAZE_SIZE]) {
    printf("\n");
    for (int r = 0; r < MAZE_SIZE; r++) {
        for (int c = 0; c < MAZE_SIZE; c++) {
            printf("%10d", maze[r][c]);
        }
        printf("\n\n\n");
    }
}

//maze 세팅하는 함수
void maze_setting(int maze[MAZE_SIZE][MAZE_SIZE]) {
    srand((unsigned int)time(NULL));
    int wumpusNum = rand() % 3; //wumpus 개수 랜덤으로 지정
    int pitNum = 2 - wumpusNum; //pit 개수
    int goldNum = 1; //gold 개수
    int obstacleNum = 0;
    int count = 0;

    while (count < goldNum) {
        obstacleNum = rand() % 36 + 1;
        for (int x = 0; x < 6; x++) {
            for (int y = 0; y < 6; y++) {
                if (obstacleNum == 6 * x + y + 1) {
                    if ((en_jud(maze[x][y], 3) != 1) && (x != 4 && y != 1)) {
                        maze[x][y] += g;
                        count++;
                    }
                }
            }
        }
    }
    count = 0;

    while (count < wumpusNum) {
        obstacleNum = rand() % 36 + 1;
        if (obstacleNum == 20 || obstacleNum == 27) {
            continue;
        }
        for (int x = 0; x < 6; x++) {
            for (int y = 0; y < 6; y++) {
                if (obstacleNum == 6 * x + y + 1) {
                    if ((en_jud(maze[x][y], 3) != 1) && (x != 4 && y != 1) && (en_jud(maze[x][y], 8) != 1) && (en_jud(maze[x][y], 7) != 1)) {
                        maze[x][y] += w;
                        if (maze[x + 1][y] != wall && en_jud(maze[x + 1][y], 4) != 1)maze[x + 1][y] = maze[x + 1][y] + stench;
                        if (maze[x - 1][y] != wall && en_jud(maze[x - 1][y], 4) != 1)maze[x - 1][y] = maze[x - 1][y] + stench;
                        if (maze[x][y + 1] != wall && en_jud(maze[x][y + 1], 4) != 1)maze[x][y + 1] = maze[x][y + 1] + stench;
                        if (maze[x][y - 1] != wall && en_jud(maze[x][y - 1], 4) != 1)maze[x][y - 1] = maze[x][y - 1] + stench;
                        count++;
                    }
                }
            }
        }
    }
    count = 0;


    while (count < pitNum) {
        obstacleNum = rand() % 36 + 1;
        if (obstacleNum == 20 || obstacleNum == 27) {
            continue;
        }
        for (int x = 0; x < 6; x++) {
            for (int y = 0; y < 6; y++) {
                if (obstacleNum == 6 * x + y + 1) {
                    if ((en_jud(maze[x][y], 3) != 1) && (x != 4 && y != 1) && (en_jud(maze[x][y], 8) != 1) && (en_jud(maze[x][y], 7) != 1) && (en_jud(maze[x][y], 6) != 1)) {
                        maze[x][y] += p;
                        if (maze[x + 1][y] != wall && en_jud(maze[x + 1][y], 5) != 1)maze[x + 1][y] = maze[x + 1][y] + breeze;
                        if (maze[x - 1][y] != wall && en_jud(maze[x - 1][y], 5) != 1)maze[x - 1][y] = maze[x - 1][y] + breeze;
                        if (maze[x][y + 1] != wall && en_jud(maze[x][y + 1], 5) != 1)maze[x][y + 1] = maze[x][y + 1] + breeze;
                        if (maze[x][y - 1] != wall && en_jud(maze[x][y - 1], 5) != 1)maze[x][y - 1] = maze[x][y - 1] + breeze;

                        if ((en_jud(maze[4][3], 6) == 1) && (en_jud(maze[3][4], 6) == 1)) {
                            int ran_num = rand() % 2;
                            if (ran_num == 0)
                                maze[4][3] = 0;
                            else
                                maze[3][4] = 0;
                            continue;
                        }
                        else if ((en_jud(maze[2][4], 6) == 1) && (en_jud(maze[1][3], 6) == 1)) {
                            int ran_num = rand() % 2;
                            if (ran_num == 0)
                                maze[2][4] = 0;
                            else
                                maze[1][3] = 0;
                            continue;
                        }
                        else if ((en_jud(maze[1][2], 6) == 1) && (en_jud(maze[2][1], 6) == 1)) {
                            int ran_num = rand() % 2;
                            if (ran_num == 0)
                                maze[1][2] = 0;
                            else
                                maze[2][1] = 0;
                            continue;
                        }
                        count++;
                    }
                }
            }
        }
    }
}

void maze_print(string warning) {
    printf("\n");
    maze[x][y] += 512; //현재위치

    for (int i = 0; i < MAZE_SIZE; i++) {
        for (int j = 0; j < MAZE_SIZE; j++) {
            if (en_jud(maze[i][j], 9) == 1) { //만약 지나간 칸이라면
                maze2[i][j].clear();

                //현재위치 표시
                if (en_jud(maze[i][j], 10) == 1) {
                    if (direction == 1) { //위쪽
                        maze2[i][j].append("^ ");
                    }
                    else if (direction == 2) { //오른쪽
                        maze2[i][j].append("> ");
                    }
                    else if (direction == 3) { //아래
                        maze2[i][j].append("v ");
                    }
                    else if (direction == 4) { //왼쪽
                        maze2[i][j].append("< ");
                    }
                    else
                        printf("잘못된 방향입니다\n");
                }

                //경험한 길 정보 보여주기
                if (en_jud(maze[i][j], 8) == 1) //gold 신호가 있으면 표시
                    maze2[i][j].append("g"); //>>glitter
                if (en_jud(maze[i][j], 1) == 1) //확인 pit이 있으면 표시
                    maze2[i][j].append("P");
                if (en_jud(maze[i][j], 2) == 1) //확인 wumpus가 있으면 표시
                    maze2[i][j].append("W");
                if (en_jud(maze[i][j], 4) == 1) //stench 신호가 있으면 표시
                    maze2[i][j].append("s");
                if (en_jud(maze[i][j], 5) == 1) //breeze 신호가 있으면 표시
                    maze2[i][j].append("b");

                if (maze2[i][j].empty())
                    maze2[i][j].append("-");
            }
        }
    }

    printf("\n\n\n\n\n\n");
    for (int r = 0; r < MAZE_SIZE; r++) {
        for (int c = 0; c < MAZE_SIZE; c++) {
            cout.width(6); // 폭을 10으로 지정
            cout.fill(' '); // 채움 문자는 ' '
            cout << maze2[r][c];
        }
        printf("\n\n");
    }
    cout << "\t\t\t" << warning << "\n";
    maze[x][y] -= 512; //다음 칸으로 이동하니까 현재위치 512 지워줘
}

void grab() {
    if ((en_jud(maze[x][y], 9) == 1) && (en_jud(maze[x][y], 8) == 1)) { //지나간 곳이고, 이 곳에 금이 있다면
        maze_print(get_gold); //"금을 획득하였습니다."
        maze[x][y] -= g; //gold 만났으니 해당 칸에서 gold 지우고
        come_back(); // come_back함수 호출해서 4, 1로 돌아가
    }
}

void come_back() {
    x = 4;
    y = 1; //원점(4, 1)로 바꿔
    maze_print(go_back); //"원점으로 돌아갔습니다."
}

void die() {
    maze_print(death_of_agent); //"에이전트가 죽었습니다."
    if (en_jud(maze[x][y], 6) == 1) //pit때문에 죽었다면
        maze[x][y] += DP;
    if (en_jud(maze[x][y], 7) == 1) //wumpus때문에 죽었다면
        maze[x][y] += DW;
    come_back();
    st_course();
}

int sensor() {
    if (en_jud(maze[x][y], 3) == 1) //현재 칸이 벽이라면(구조상 그럴일은 없지만)
        return 4; //4로 리턴
    else if (en_jud(maze[x][y], 6) == 1) //현재 칸에 pit이 있다면
        return 2; //die로 대변되는 2로 리턴
    else if (en_jud(maze[x][y], 7) == 1) //현재 칸에 wumpus가 있다면
        return 2; //die로 대변되는 2로 리턴
    else if (en_jud(maze[x][y], 8) == 1) //현재 칸에 gold가 있다면
        return 1; //1로 리턴
    else if ((en_jud(maze[x][y], 6) != 1) && (en_jud(maze[x][y], 7) != 1)) //현재 칸에 pit도 wumpus도 없다면(안전하다면)
        return 0; //0으로 리턴
    else
        return 0;
}

//**************************************************************************
void tr() {
    direction++;
    if (direction == 5) direction = 1;
    maze_print(turn_right);
}
void tl() {
    direction--;
    if (direction == 0) direction = 4;
    maze_print(turn_left);
}

int ABS(int a) { //절대값
    if (a < 0) return(-a);
    else return a;
}

void go_f() {
    if (direction == 1) x--; //위쪽
    else if (direction == 2) y++; //오른쪽
    else if (direction == 3) x++; //아래쪽
    else if (direction == 4) y--; //=3, 왼쪽=4
    //else break;
    if (en_jud(maze[x][y], 9) != 1)maze[x][y] += 256;
    maze_print(go_forward);
}

void turn(int nx, int ny) {
    int post_direction = 0, temp;
    if (nx < x) post_direction = 1;
    else if (x < nx) post_direction = 3;
    else if (y < ny) post_direction = 2;
    else if (ny < y) post_direction = 4;

    temp = post_direction - direction;

    if (temp == 3) tl();
    else if (temp == -3) tr();
    else if (temp > 0) {
        for (int i = 0; i < ABS(temp); i++) {
            tr();
        }
    }
    else if (temp < 0) {
        for (int i = 0; i < ABS(temp); i++) {
            tl();
        }
    }
}

void move(int nx, int ny) {
    turn(nx, ny);
    go_f();
}

void shoot() {
    turn(next_cell(1, x, y), next_cell(2, x, y));
    maze_print(shooting);
    if (direction == 1) {
        for (int i = x - 1; i > 0; i--) {
            if (en_jud(maze[i][y], 7) == 1) {
                maze_print(scream);
                maze[i][y] -= 64;
                if (en_jud(maze[i][y], 2) == 1) {
                    maze[i][y] -= 2;
                }break;
            }
        }
    }
    else if (direction == 2) {
        for (int i = y + 1; i < 5; i++) {
            if (en_jud(maze[x][i], 7) == 1) {
                maze_print(scream);
                maze[x][i] -= 64;
                if (en_jud(maze[x][i], 2) == 1) {
                    maze[x][i] -= 2;
                }break;
            }
        }
    }
    else if (direction == 3) {
        for (int i = x + 1; i < 5; i++) {
            if (en_jud(maze[i][y], 7) == 1) {
                maze_print(scream);
                maze[i][y] -= 64;
                if (en_jud(maze[i][y], 2) == 1) {
                    maze[i][y] -= 2;
                }break;
            }
        }
    }
    else if (direction == 4) {
        for (int i = y - 1; i > 0; i--) {
            if (en_jud(maze[x][i], 7) == 1) {
                maze_print(scream);
                maze[x][i] -= 64;
                if (en_jud(maze[x][i], 2) == 1) {
                    maze[x][i] -= 2;
                }break;
            }
        }
    }
    st_course();
}


int next_cell(int e, int c, int d) {//현재위치 다음칸의 x좌표를 알고싶으면 1,x,y입력 y좌표를 알고싶으면 2,x,y입력
    int a[16] = { 0, }, b[16] = { 0, };
    int l = 0;
    for (int i = 4; i >= 1; i--) { //4, 2행은 오른쪽으로
        if (i % 2 == 0) {
            for (int j = 1; j <= 4; j++) {
                a[l] = i;
                b[l] = j;
                l++;
            }
        }
        else { //3, 1행을 왼쪽으로
            for (int k = 4; k >= 1; k--) {
                a[l] = i;
                b[l] = k;
                l++;
            }// 바깥쪽 루프를 끝냄
        }
    }

    for (int i = 0; i < 16; i++) {
        if (e == 1 && a[i] == c && b[i] == d) {
            return a[i + 1];
            break;
        }
        else if (e == 2 && a[i] == c && b[i] == d) {
            return b[i + 1];
            break;
        }
    }
    return 0;
}

void st_course() {
    int temp1 = 0, temp2 = 0, temp3 = 0;
    while (!(sensor() == 1 || sensor() == 2 || en_jud(maze[next_cell(1, x, y)][next_cell(2, x, y)], 1) == 1 || en_jud(maze[next_cell(1, x, y)][next_cell(2, x, y)], 2) == 1)) {
        move(next_cell(1, x, y), next_cell(2, x, y));
    }
    if (sensor() == 1) grab();
    else if (sensor() == 2) die();
    else if (en_jud(maze[next_cell(1, x, y)][next_cell(2, x, y)], 1) == 1) bypass();
    else if (en_jud(maze[next_cell(1, x, y)][next_cell(2, x, y)], 2) == 1) shoot();
}



void maze_print_none() {
    printf("\n");
    maze[x][y] += 512; //현재위치

    for (int i = 0; i < MAZE_SIZE; i++) {
        for (int j = 0; j < MAZE_SIZE; j++) {
            if (en_jud(maze[i][j], 9) == 1) { //만약 지나간 칸이라면
                maze2[i][j].clear();

                //현재위치 표시
                if (en_jud(maze[i][j], 10) == 1) {
                    if (direction == 1) { //위쪽
                        maze2[i][j].append("^ ");
                    }
                    else if (direction == 2) { //오른쪽
                        maze2[i][j].append("> ");
                    }
                    else if (direction == 3) { //아래
                        maze2[i][j].append("v ");
                    }
                    else if (direction == 4) { //왼쪽
                        maze2[i][j].append("< ");
                    }
                    else
                        printf("잘못된 방향입니다\n");
                }

                //경험한 길 정보 보여주기
                if (en_jud(maze[i][j], 8) == 1) //gold 신호가 있으면 표시
                    maze2[i][j].append("g"); //>>glitter
                if (en_jud(maze[i][j], 1) == 1) //확인 pit이 있으면 표시
                    maze2[i][j].append("P");
                if (en_jud(maze[i][j], 2) == 1) //확인 wumpus가 있으면 표시
                    maze2[i][j].append("W");
                if (en_jud(maze[i][j], 4) == 1) //stench 신호가 있으면 표시
                    maze2[i][j].append("s");
                if (en_jud(maze[i][j], 5) == 1) //breeze 신호가 있으면 표시
                    maze2[i][j].append("b");

                if (maze2[i][j].empty())
                    maze2[i][j].append("-");
            }
        }
    }

    printf("\n\n\n\n\n\n");
    for (int r = 0; r < MAZE_SIZE; r++) {
        for (int c = 0; c < MAZE_SIZE; c++) {
            cout.width(10); // 폭을 10으로 지정
            cout.fill(' '); // 채움 문자는 ' '
            cout << maze2[r][c];
        }
        printf("\n\n\n\n\n");
    }
    maze[x][y] -= 512; //다음 칸으로 이동하니까 현재위치 512 지워줘
}

//**************************************************************************
void bypass() {
    int a1[20] = { 9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9 };
    int b1[20] = { 9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9 };
    int a2[20] = { 9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9 };
    int b2[20] = { 9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9 };
    int a3[20] = { 9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9 };
    int b3[20] = { 9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9 };
    int a4[20] = { 9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9 };
    int b4[20] = { 9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9 };
    int order_a[20] = { 9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9 };
    int order_b[20] = { 9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9 };
    int check_pit = 0;
    int pit_x, pit_y;
    int four1_a[4];
    int four1_b[4];
    int four2_a[4];
    int four2_b[4];
    int arrive_a;
    int arrive_b;
    int start1_index = 0;
    int start2_index;
    int arrive_index;
    int bypass1_a[15] = { 9,9,9,9,9,9,9,9,9,9,9,9,9,9,9 };
    int bypass1_b[15] = { 9,9,9,9,9,9,9,9,9,9,9,9,9,9,9 };
    int bypass2_a[15] = { 9,9,9,9,9,9,9,9,9,9,9,9,9,9,9 };
    int bypass2_b[15] = { 9,9,9,9,9,9,9,9,9,9,9,9,9,9,9 };
    int check_wall1 = 0;
    int check_wall2 = 0;



    a1[0] = next_cell(1, x, y) - 1;
    a1[1] = next_cell(1, x, y) - 1;
    a1[2] = next_cell(1, x, y) - 1;
    b1[0] = next_cell(2, x, y) - 1;
    b1[1] = next_cell(2, x, y);
    b1[2] = next_cell(2, x, y) + 1;
    a1[3] = next_cell(1, x, y);
    a1[4] = next_cell(1, x, y);
    b1[3] = next_cell(2, x, y) - 1;
    b1[4] = next_cell(2, x, y) + 1;
    a1[5] = next_cell(1, x, y) + 1;
    a1[6] = next_cell(1, x, y) + 1;
    a1[7] = next_cell(1, x, y) + 1;
    b1[5] = next_cell(2, x, y) - 1;
    b1[6] = next_cell(2, x, y);
    b1[7] = next_cell(2, x, y) + 1;


    for (int i = 0; i < 8; i++) {
        if (en_jud(maze[a1[i]][b1[i]], 1) == 1) {
            check_pit++;
            pit_x = a1[i];
            pit_y = b1[i];
        }
    }



    if (check_pit == 1) {

        a2[0] = pit_x - 1;
        a2[1] = pit_x - 1;
        a2[2] = pit_x - 1;
        b2[0] = pit_y - 1;
        b2[1] = pit_y;
        b2[2] = pit_y + 1;
        a2[3] = pit_x;
        a2[4] = pit_x;
        b2[3] = pit_y - 1;
        b2[4] = pit_y + 1;
        a2[5] = pit_x + 1;
        a2[6] = pit_x + 1;
        a2[7] = pit_x + 1;
        b2[5] = pit_y - 1;
        b2[6] = pit_y;
        b2[7] = pit_y + 1;

        //두집합 더하기
        for (int i = 0, k = 8; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                a1[k] = a2[i];
                b1[k] = b2[i];
            }
            k++;
        }

        //교집합 제거
        for (int i = 0, k = 0, count = 0; i < 20; i++, k++) {
            count = 0;
            for (int j = i + 1; j < 20; j++) {
                if ((a1[i] == a1[j] && b1[i] == b1[j]) || en_jud(maze[a1[i]][b1[i]], 1) == 1) { count++; a3[k] = 9; b3[k] = 9; }
            }
            if (count == 0) { a3[k] = a1[i]; b3[k] = b1[i]; }
        }
    }


    //a4에 a3복사
    for (int i = 0; i < 20; i++) {
        a4[i] = a3[i];
    }
    for (int i = 0; i < 20; i++) {
        b4[i] = b3[i];
    }


    order_a[0] = x;
    order_b[0] = y;

    //x,y 좌표 a3,b3에서 제거
    for (int i = 0; i < 20; i++) {
        if (x == a3[i] && y == b3[i]) {
            a3[i] = 9;
            b3[i] = 9;
        }
    }


    //순서대로 정렬
    for (int i = 0, count = 0; i < 19; i++) {
        for (int j = 0; j < 20; j++) {
            if (isit_next(order_a[i], order_b[i], a3[j], b3[j])) {
                order_a[i + 1] = a3[j]; order_b[i + 1] = b3[j]; a3[j] = 9; b3[j] = 9; j = 21;
            }
            else { continue; }
        }
    }

    //order 마지막자리 찾기
    for (int i = 0; i < 20; i++) {
        if (order_a[i] == 9) { start2_index = i; break; }
    }




    //마지막에 다시x,y 넣어주기
    for (int i = 0; i < 20; i++) {
        if (order_a[i] == 9) { order_a[i] = x; order_b[i] = y; break; }
    }

    arrive_a = pit_goal(1, x, y);
    arrive_b = pit_goal(2, x, y);

    //arrive_index찾기
    for (int i = 0; i < 20; i++) {
        if (order_a[i] == arrive_a && order_b[i] == arrive_b) { arrive_index = i; break; }
    }


    for (int i = 1, j = 0; j < arrive_index; i++, j++) {
        bypass1_a[j] = order_a[i];
        bypass1_b[j] = order_b[i];
    }

    for (int i = start2_index - 1, j = 0; j < (start2_index - arrive_index); i--, j++) {
        bypass2_a[j] = order_a[i];
        bypass2_b[j] = order_b[i];
    }




    //경로 1,2에 벽이 있나 확인
    for (int i = 0; i < 15; i++) {
        if (bypass1_a[i] == 0 || bypass1_a[i] == 5 || bypass1_b[i] == 0 || bypass1_a[i] == 5)
        {
            check_wall1++;
        }
    }
    for (int i = 0; i < 15; i++) {
        if (bypass2_a[i] == 0 || bypass2_a[i] == 5 || bypass2_b[i] == 0 || bypass2_a[i] == 5)
        {
            check_wall2++;
        }
    }


    if ((check_wall1 + check_wall2) > 0) {
        if (check_wall1 == 0) {
            for (int i = 0; sensor() != 1 || sensor() != 2 || en_jud(maze[bypass2_a[i + 1]][bypass2_a[i + 1]], 1) != 1; i++) {
                move(bypass1_a[i], bypass1_b[i]);
            }

            if (sensor() == 1) grab();
            else if (sensor() == 2) die();
            else if (en_jud(maze[next_cell(1, x, y)][next_cell(2, x, y)], 2) == 1) shoot();

        }
        else {
            for (int i = 0; sensor() != 1 || sensor() != 2 || en_jud(maze[bypass2_b[i + 1]][bypass2_b[i + 1]], 1) != 1; i++) {
                move(bypass2_a[i], bypass2_b[i]);
            }

            if (sensor() == 1) grab();
            else if (sensor() == 2) die();
            else if (en_jud(maze[next_cell(1, x, y)][next_cell(2, x, y)], 2) == 1) shoot();
        }

    }
    else {
        if ((arrive_index - start1_index) < (start2_index - arrive_index)) {
            for (int i = 0; sensor() != 1 || sensor() != 2 || en_jud(maze[bypass2_a[i + 1]][bypass2_a[i + 1]], 1) != 1; i++) {
                move(bypass1_a[i], bypass1_b[i]);
            }

            if (sensor() == 1) grab();
            else if (sensor() == 2) die();
            else if (en_jud(maze[next_cell(1, x, y)][next_cell(2, x, y)], 2) == 1) shoot();

        }
        else {
            for (int i = 0; sensor() != 1 || sensor() != 2 || en_jud(maze[bypass2_b[i + 1]][bypass2_b[i + 1]], 1) != 1; i++) {
                move(bypass2_a[i], bypass2_b[i]);
            }

            if (sensor() == 1) grab();
            else if (sensor() == 2) die();
            else if (en_jud(maze[next_cell(1, x, y)][next_cell(2, x, y)], 2) == 1) shoot();
        }

    }


}

int isit_next(int a, int b, int c, int d) {
    if ((ABS(a - c) == 1 && b == d) || (ABS(b - d) == 1 && a == c)) { return 1; }
    else { return 0; }
}

int pit_goal(int e, int c, int d) {//현재위치 다음칸의 x좌표를 알고싶으면 1,x,y입력 y좌표를 알고싶으면 2,x,y입력
    int a[16] = { 0, }, b[16] = { 0, };
    int l = 0;
    for (int i = 4; i >= 1; i--) { //4, 2행은 오른쪽으로
        if (i % 2 == 0) {
            for (int j = 1; j <= 4; j++) {
                a[l] = i;
                b[l] = j;
                l++;
            }
        }
        else { //3, 1행을 왼쪽으로
            for (int k = 4; k >= 1; k--) {
                a[l] = i;
                b[l] = k;
                l++;
            }// 바깥쪽 루프를 끝냄
        }
    }


    if ((en_jud(maze[next_cell(1, x, y)][next_cell(2, x, y)], 1) == 1) && (en_jud(maze[next_cell(1, next_cell(1, x, y), next_cell(2, x, y))][next_cell(2, next_cell(1, x, y), next_cell(2, x, y))], 1) == 1)) {
        for (int i = 0; i < 16; i++) {
            if (e == 1 && a[i] == c && b[i] == d) {
                return a[i + 3];
                break;
            }
            else if (e == 2 && a[i] == c && b[i] == d) {
                return b[i + 3];
                break;
            }
        }
    }
    else if (en_jud(maze[next_cell(1, x, y)][next_cell(2, x, y)], 1) == 1) {
        for (int i = 0; i < 16; i++) {
            if (e == 1 && a[i] == c && b[i] == d) {
                return a[i + 2];
                break;
            }
            else if (e == 2 && a[i] == c && b[i] == d) {
                return b[i + 2];
                break;
            }
        }
    }
}