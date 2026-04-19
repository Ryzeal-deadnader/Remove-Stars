#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <easyx.h>
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

int main() {
    srand(time(NULL));//生成地图种子确保每关不一样
}

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
    for (int j = 0; j < COLS; j++) {
        int temp[100] = { 0 };
        int idx = ROWS - 1;

        for (int i = ROWS - 1; i >= 0; i--) {
            if (board[i][j] != empty) {
                temp[idx--] = board[i][j];
            }
        }
        for (int i = 0; i < ROWS; i++) {
            board[i][j] = temp[i];
        }
    }
}