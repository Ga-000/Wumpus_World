//  2021.06.11
//  main.cpp
//  Wumpus world


/*
******* �ڸ���*******
��������ġ=9
���=8
��Ǫ��=7
Pit=6
�ٶ�=5
����=4
��=3
Ȯ�ο�Ǫ��=2
Ȯ��Pit=1
*/


//���*******************************************************************************
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


//����,shoot, ��Ȳ�Ǵ�, next_cell, move, turn
//maze_print, grab, come_back, die, sensor
//by_pass


//����*********************************************************************************
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

string start("������ �����մϴ�.");
string get_gold("���� ȹ���Ͽ����ϴ�.");
string go_back("�������� ���ư����ϴ�.");
string death_of_agent("������Ʈ�� �׾����ϴ�.");
string shooting("ȭ���� �����ϴ�.");
string turn_left("�������� ȸ��");
string turn_right("���������� ȸ��");
string go_forward("��ĭ ������");
string scream("���!");


int maze[MAZE_SIZE][MAZE_SIZE] = {
    {4, 4, 4, 4, 4, 4},
    {4, 0, 0, 0, 0, 4},
    {4, 0, 0, 0, 0, 4},
    {4, 0, 0, 0, 0, 4},
    {4, 256, 0, 0, 0, 4},
    {4, 4, 4, 4, 4, 4}
};

basic_string<char, char_traits<char>, allocator<char>> maze2[MAZE_SIZE][MAZE_SIZE] = { //���ڿ� 3���� �迭
    {"[ ]", "[ ]", "[ ]", "[ ]", "[ ]", "[ ]"},
    {"[ ]", "U", "U", "U", "U", "[ ]"},
    {"[ ]", "U", "U", "U", "U", "[ ]"},
    {"[ ]", "U", "U", "U", "U", "[ ]"},
    {"[ ]", "U", "U", "U", "U", "[ ]"},
    {"[ ]", "[ ]", "[ ]", "[ ]", "[ ]", "[ ]"}
};

int x = 4, y = 1; //������ǥ
int direction = 2; //����=1, ������=2, �Ʒ���=3, ����=4



//�Լ� �⺻��**************************************************************************
void maze_print_for_us(int maze[MAZE_SIZE][MAZE_SIZE]);
void maze_setting(int maze[MAZE_SIZE][MAZE_SIZE]);
void maze_print(string warning);
void grab();
void come_back();
void die();
int sensor();
void maze_print_none();

//����
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

//����
void bypass();
int pit_goal(int e, int c, int d);
int isit_next(int a, int b, int c, int d);



//main�Լ�*****************************************************************
int main() {
    maze_setting(maze);
    maze_print_for_us(maze);
    maze_print(start);
    st_course();
    return 0;
}



//����� ���� �Լ� ����******************************************************
int en_jud(int a, int b) {
    int sum[20] = { 0 };
    for (int i = 0; a != 0; i++) {
        if (a % 2 == 1)
            sum[i] = 1;
        a = a / 2;
    }
    return sum[b - 1];
}

//�̷θ� ȭ�鿡 ����Ѵ�(�ϼ� ������ �ӽ� Ȯ�ο����� ���)
void maze_print_for_us(int maze[MAZE_SIZE][MAZE_SIZE]) {
    printf("\n");
    for (int r = 0; r < MAZE_SIZE; r++) {
        for (int c = 0; c < MAZE_SIZE; c++) {
            printf("%10d", maze[r][c]);
        }
        printf("\n\n\n");
    }
}

//maze �����ϴ� �Լ�
void maze_setting(int maze[MAZE_SIZE][MAZE_SIZE]) {
    srand((unsigned int)time(NULL));
    int wumpusNum = rand() % 3; //wumpus ���� �������� ����
    int pitNum = 2 - wumpusNum; //pit ����
    int goldNum = 1; //gold ����
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
    maze[x][y] += 512; //������ġ

    for (int i = 0; i < MAZE_SIZE; i++) {
        for (int j = 0; j < MAZE_SIZE; j++) {
            if (en_jud(maze[i][j], 9) == 1) { //���� ������ ĭ�̶��
                maze2[i][j].clear();

                //������ġ ǥ��
                if (en_jud(maze[i][j], 10) == 1) {
                    if (direction == 1) { //����
                        maze2[i][j].append("^ ");
                    }
                    else if (direction == 2) { //������
                        maze2[i][j].append("> ");
                    }
                    else if (direction == 3) { //�Ʒ�
                        maze2[i][j].append("v ");
                    }
                    else if (direction == 4) { //����
                        maze2[i][j].append("< ");
                    }
                    else
                        printf("�߸��� �����Դϴ�\n");
                }

                //������ �� ���� �����ֱ�
                if (en_jud(maze[i][j], 8) == 1) //gold ��ȣ�� ������ ǥ��
                    maze2[i][j].append("g"); //>>glitter
                if (en_jud(maze[i][j], 1) == 1) //Ȯ�� pit�� ������ ǥ��
                    maze2[i][j].append("P");
                if (en_jud(maze[i][j], 2) == 1) //Ȯ�� wumpus�� ������ ǥ��
                    maze2[i][j].append("W");
                if (en_jud(maze[i][j], 4) == 1) //stench ��ȣ�� ������ ǥ��
                    maze2[i][j].append("s");
                if (en_jud(maze[i][j], 5) == 1) //breeze ��ȣ�� ������ ǥ��
                    maze2[i][j].append("b");

                if (maze2[i][j].empty())
                    maze2[i][j].append("-");
            }
        }
    }

    printf("\n\n\n\n\n\n");
    for (int r = 0; r < MAZE_SIZE; r++) {
        for (int c = 0; c < MAZE_SIZE; c++) {
            cout.width(6); // ���� 10���� ����
            cout.fill(' '); // ä�� ���ڴ� ' '
            cout << maze2[r][c];
        }
        printf("\n\n");
    }
    cout << "\t\t\t" << warning << "\n";
    maze[x][y] -= 512; //���� ĭ���� �̵��ϴϱ� ������ġ 512 ������
}

void grab() {
    if ((en_jud(maze[x][y], 9) == 1) && (en_jud(maze[x][y], 8) == 1)) { //������ ���̰�, �� ���� ���� �ִٸ�
        maze_print(get_gold); //"���� ȹ���Ͽ����ϴ�."
        maze[x][y] -= g; //gold �������� �ش� ĭ���� gold �����
        come_back(); // come_back�Լ� ȣ���ؼ� 4, 1�� ���ư�
    }
}

void come_back() {
    x = 4;
    y = 1; //����(4, 1)�� �ٲ�
    maze_print(go_back); //"�������� ���ư����ϴ�."
}

void die() {
    maze_print(death_of_agent); //"������Ʈ�� �׾����ϴ�."
    if (en_jud(maze[x][y], 6) == 1) //pit������ �׾��ٸ�
        maze[x][y] += DP;
    if (en_jud(maze[x][y], 7) == 1) //wumpus������ �׾��ٸ�
        maze[x][y] += DW;
    come_back();
    st_course();
}

int sensor() {
    if (en_jud(maze[x][y], 3) == 1) //���� ĭ�� ���̶��(������ �׷����� ������)
        return 4; //4�� ����
    else if (en_jud(maze[x][y], 6) == 1) //���� ĭ�� pit�� �ִٸ�
        return 2; //die�� �뺯�Ǵ� 2�� ����
    else if (en_jud(maze[x][y], 7) == 1) //���� ĭ�� wumpus�� �ִٸ�
        return 2; //die�� �뺯�Ǵ� 2�� ����
    else if (en_jud(maze[x][y], 8) == 1) //���� ĭ�� gold�� �ִٸ�
        return 1; //1�� ����
    else if ((en_jud(maze[x][y], 6) != 1) && (en_jud(maze[x][y], 7) != 1)) //���� ĭ�� pit�� wumpus�� ���ٸ�(�����ϴٸ�)
        return 0; //0���� ����
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

int ABS(int a) { //���밪
    if (a < 0) return(-a);
    else return a;
}

void go_f() {
    if (direction == 1) x--; //����
    else if (direction == 2) y++; //������
    else if (direction == 3) x++; //�Ʒ���
    else if (direction == 4) y--; //=3, ����=4
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


int next_cell(int e, int c, int d) {//������ġ ����ĭ�� x��ǥ�� �˰������ 1,x,y�Է� y��ǥ�� �˰������ 2,x,y�Է�
    int a[16] = { 0, }, b[16] = { 0, };
    int l = 0;
    for (int i = 4; i >= 1; i--) { //4, 2���� ����������
        if (i % 2 == 0) {
            for (int j = 1; j <= 4; j++) {
                a[l] = i;
                b[l] = j;
                l++;
            }
        }
        else { //3, 1���� ��������
            for (int k = 4; k >= 1; k--) {
                a[l] = i;
                b[l] = k;
                l++;
            }// �ٱ��� ������ ����
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
    maze[x][y] += 512; //������ġ

    for (int i = 0; i < MAZE_SIZE; i++) {
        for (int j = 0; j < MAZE_SIZE; j++) {
            if (en_jud(maze[i][j], 9) == 1) { //���� ������ ĭ�̶��
                maze2[i][j].clear();

                //������ġ ǥ��
                if (en_jud(maze[i][j], 10) == 1) {
                    if (direction == 1) { //����
                        maze2[i][j].append("^ ");
                    }
                    else if (direction == 2) { //������
                        maze2[i][j].append("> ");
                    }
                    else if (direction == 3) { //�Ʒ�
                        maze2[i][j].append("v ");
                    }
                    else if (direction == 4) { //����
                        maze2[i][j].append("< ");
                    }
                    else
                        printf("�߸��� �����Դϴ�\n");
                }

                //������ �� ���� �����ֱ�
                if (en_jud(maze[i][j], 8) == 1) //gold ��ȣ�� ������ ǥ��
                    maze2[i][j].append("g"); //>>glitter
                if (en_jud(maze[i][j], 1) == 1) //Ȯ�� pit�� ������ ǥ��
                    maze2[i][j].append("P");
                if (en_jud(maze[i][j], 2) == 1) //Ȯ�� wumpus�� ������ ǥ��
                    maze2[i][j].append("W");
                if (en_jud(maze[i][j], 4) == 1) //stench ��ȣ�� ������ ǥ��
                    maze2[i][j].append("s");
                if (en_jud(maze[i][j], 5) == 1) //breeze ��ȣ�� ������ ǥ��
                    maze2[i][j].append("b");

                if (maze2[i][j].empty())
                    maze2[i][j].append("-");
            }
        }
    }

    printf("\n\n\n\n\n\n");
    for (int r = 0; r < MAZE_SIZE; r++) {
        for (int c = 0; c < MAZE_SIZE; c++) {
            cout.width(10); // ���� 10���� ����
            cout.fill(' '); // ä�� ���ڴ� ' '
            cout << maze2[r][c];
        }
        printf("\n\n\n\n\n");
    }
    maze[x][y] -= 512; //���� ĭ���� �̵��ϴϱ� ������ġ 512 ������
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

        //������ ���ϱ�
        for (int i = 0, k = 8; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                a1[k] = a2[i];
                b1[k] = b2[i];
            }
            k++;
        }

        //������ ����
        for (int i = 0, k = 0, count = 0; i < 20; i++, k++) {
            count = 0;
            for (int j = i + 1; j < 20; j++) {
                if ((a1[i] == a1[j] && b1[i] == b1[j]) || en_jud(maze[a1[i]][b1[i]], 1) == 1) { count++; a3[k] = 9; b3[k] = 9; }
            }
            if (count == 0) { a3[k] = a1[i]; b3[k] = b1[i]; }
        }
    }


    //a4�� a3����
    for (int i = 0; i < 20; i++) {
        a4[i] = a3[i];
    }
    for (int i = 0; i < 20; i++) {
        b4[i] = b3[i];
    }


    order_a[0] = x;
    order_b[0] = y;

    //x,y ��ǥ a3,b3���� ����
    for (int i = 0; i < 20; i++) {
        if (x == a3[i] && y == b3[i]) {
            a3[i] = 9;
            b3[i] = 9;
        }
    }


    //������� ����
    for (int i = 0, count = 0; i < 19; i++) {
        for (int j = 0; j < 20; j++) {
            if (isit_next(order_a[i], order_b[i], a3[j], b3[j])) {
                order_a[i + 1] = a3[j]; order_b[i + 1] = b3[j]; a3[j] = 9; b3[j] = 9; j = 21;
            }
            else { continue; }
        }
    }

    //order �������ڸ� ã��
    for (int i = 0; i < 20; i++) {
        if (order_a[i] == 9) { start2_index = i; break; }
    }




    //�������� �ٽ�x,y �־��ֱ�
    for (int i = 0; i < 20; i++) {
        if (order_a[i] == 9) { order_a[i] = x; order_b[i] = y; break; }
    }

    arrive_a = pit_goal(1, x, y);
    arrive_b = pit_goal(2, x, y);

    //arrive_indexã��
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




    //��� 1,2�� ���� �ֳ� Ȯ��
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

int pit_goal(int e, int c, int d) {//������ġ ����ĭ�� x��ǥ�� �˰������ 1,x,y�Է� y��ǥ�� �˰������ 2,x,y�Է�
    int a[16] = { 0, }, b[16] = { 0, };
    int l = 0;
    for (int i = 4; i >= 1; i--) { //4, 2���� ����������
        if (i % 2 == 0) {
            for (int j = 1; j <= 4; j++) {
                a[l] = i;
                b[l] = j;
                l++;
            }
        }
        else { //3, 1���� ��������
            for (int k = 4; k >= 1; k--) {
                a[l] = i;
                b[l] = k;
                l++;
            }// �ٱ��� ������ ����
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