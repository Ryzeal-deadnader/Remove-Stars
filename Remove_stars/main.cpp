#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <easyx.h>
#include <windows.h>
/*傻瓜式教程，全程注释，希望你看得懂
 *重要的事情说三遍
 *不要忘了安装图形库！
 *不要忘了安装图形库！
 *不要忘了安装图形库！
 *（不会就自己去网上查easyx）
 * by LZR（觉得有注释羞耻就把注释删了）
 */
#define empty 0

int COLS;
int ROWS;
int **board;
int score = 0;
//全局变量定义

int dx[] = { -1,1,0,0 };
int dy[] = { 0,0,-1,1 };



void initgame(int rows, int cols) {//初始化
    ROWS = rows;
    COLS = cols;

    int i, j;
    for (i = 0; i < ROWS; i++) {
        for (j = 0; j < COLS; j++) {
            board[i][j] = rand() % 5 + 1;//填充颜色
        }
    }
}

int vaildmove() {
    /*可用步检测
     在检测游戏是否结束或检测关卡生成是否合法是进行复用*/
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            if (board[i][j]== 0)
                continue;
            if (i+1 < ROWS && board[i+1][j] == board[i][j] ) //行检测
                return 1;
            if (j+1 < COLS && board[i][j+1] == board[i][j] )//列检测
                return 1;
        }
    }
    return 0;
}
void CreatBoard(int rows,int cols) {
    do {
        initgame(rows,cols);
    }while (!vaildmove());//生成到有可用步数为止
}

void allocBoard(int rows, int cols) {
    ROWS = rows;
    COLS = cols;

    board = (int**)malloc(rows * sizeof(int*));//内存分配

    for (int i = 0; i < rows; i++) {
        board[i] = (int*)malloc(cols * sizeof(int));
    }
}

void startGame(int rows, int cols) {
    allocBoard(rows, cols);   //分配内存
    CreatBoard(rows, cols); //生成地图
    score = 0;              //分数清零
}

int removeStars(int x, int y) {//核心功能实现
    if (board[x][y] == empty) return 0;//判断有没有块

    int color = board[x][y];//标记颜色
    int visit[100][100] = { 0 };
    int q[10000][2];//队列
    int front = 0, rear = 0;//头和尾

    q[rear][0] = x;
    q[rear][1] = y;
    rear++;
    visit[x][y] = 1;//标记是否找过

    int get_score = 0;

    while (front < rear) {
        int cx = q[front][0];
        int cy = q[front][1];
        front++;//取出队头星星

        board[cx][cy] = empty;//消除对应色块
        get_score++;

        for (int d = 0; d < 4; d++) {
            int nx = cx + dx[d];
            int ny = cy + dy[d];//查找，对应上面dx和dy的四个值，即完成左右上下的查找
            if (nx >= 0 && nx < ROWS && ny >= 0 && ny < COLS && board[nx][ny] == color && !visit[nx][ny]) {//旁边有块而且颜色对上了并且没查过
                visit[nx][ny] = 1;//找过了
                q[rear][0] = nx;
                q[rear][1] = ny;
                rear++;//放入队尾等待下一次消除
            }
        }
    }//消完之后front就是0，查找的新块放入队尾导致rear为1继续循环，达成找着了全给消除的作用
    return get_score;//返回得分
}

void fallStars() {
    for (int j = 0; j < COLS; j++) {//对每一列工作
        int temp[100] = { 0 };//预设
        int idx = ROWS - 1;
        for (int i = ROWS - 1; i >= 0; i--) {
            if (board[i][j] != empty) {
                temp[idx--] = board[i][j];//非空则该点设置为对应预设
            }
        }
        for (int i = 0; i < ROWS; i++) {
            board[i][j] = temp[i];
        }
    }
}
void leftShift() {
    int temp[100][100] = { 0 };
    int newCol = 0;

    for (int j = 0; j < COLS; j++) {
        int has = 0;
        for (int i = 0; i < ROWS; i++) {
            if (board[i][j] != empty) {
                has = 1;
                break;
            }
        }
        if (has) {
            for (int i = 0; i < ROWS; i++) {
                temp[i][newCol] = board[i][j];
            }
            newCol++;
        }
    }

    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            board[i][j] = temp[i][j];
        }
    }
}

void drawBoard() {//easyx制作图形化界面
    cleardevice();
    setbkmode(TRANSPARENT);
    settextcolor(WHITE);
    settextstyle(20, 0, _T("宋体"));
    TCHAR s[50];
    wsprintf(s, _T("分数：%d"), score);
    outtextxy(10, 10, s);

    int w = 40;
    int h = 40;

    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            int x = 50 + j * w;
            int y = 50 + i * h;

            if (board[i][j] == 1) setfillcolor(YELLOW);
            else if (board[i][j] == 2) setfillcolor(RED);
            else if (board[i][j] == 3) setfillcolor(BLUE);
            else if (board[i][j] == 4) setfillcolor(GREEN);
            else if (board[i][j] == 5) setfillcolor(RGB(255, 165, 0));
            else setfillcolor(BLACK);

            solidcircle(x + 20, y + 20, 16);
        }
    }
}

void mouseClick() {//鼠标检测给我整力竭了，ai出来的懒得注释了
    MSG msg;

    if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
        if (msg.message == WM_LBUTTONDOWN) {
            int mx = LOWORD(msg.lParam);
            int my = HIWORD(msg.lParam);

            int j = (mx - 50) / 40;
            int i = (my - 50) / 40;

            if (i >= 0 && i < ROWS && j >= 0 && j < COLS) {
                int num = removeStars(i, j);
                if (num > 1) {
                    score += num * num * 5;
                    fallStars();
                    leftShift();
                }
            }
        }
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

int main() {
    srand(time(NULL));//生成地图种子确保每关不一样
    int rows, cols;
    printf("请输入行数：");
    scanf("%d", &rows);
    printf("请输入列数：");
    scanf("%d", &cols);

    startGame(rows, cols);

    initgraph(800, 600);
    setbkcolor(BLACK);

    while (1) {
        drawBoard();
        mouseClick();

        if (!vaildmove()) {
            cleardevice();
            settextcolor(RED);
            settextstyle(50, 0, _T("宋体"));
            outtextxy(200, 200, _T("游戏结束！"));
            TCHAR s[50];
            wsprintf(s, _T("最终得分：%d"), score);
            outtextxy(200, 300, s);
            getchar();
            getchar();
            closegraph();
            return 0;
        }
    }
}