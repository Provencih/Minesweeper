// Minesweeper.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//


#include <iostream>
#include <iomanip>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>

using namespace std;

class Minesweeper
{
private:
    int ROW, COL, N;

    int dx[8] = { -1, 0, 1, -1, 1, -1, 0, 1 };
    int dy[8] = { -1, -1, -1, 0, 0, 1, 1, 1 };
    int recurCnt = 0;
    bool defeat = false;
    bool victory = false;
    void DFS(vector<vector<char>>&, int, int);
    vector<vector<char>> updateBoard(vector<vector<char>>&, int, int);
    vector<vector<char>> init();
public:
    Minesweeper(int row, int col, int n) :ROW(row), COL(col), N(n) {}
    void interact();
};

int main()
{
    int row, col, n;
    cout << "请输入地图行数（1 - 50 的整数）：" << endl;
    cin >> row;
    cout << "请输入地图列数（1 - 50 的整数）：" << endl;
    cin >> col;
    cout << "请输入地雷数量（1 - 99 的整数）：" << endl;
    cin >> n;
    while (row > 50 || col > 50 || n > 99 || row < 10 || col < 10 || n < 1)
    {
        system("cls");
        cout << "输入非法：地图行列数或地雷数量超出范围。" << endl;
        cout << "请输入地图行数（10 - 50 的整数）：" << endl;
        cin >> row;
        cout << "请输入地图列数（10 - 50 的整数）：" << endl;
        cin >> col;
        cout << "请输入地雷数量（1 - 99 的整数）：" << endl;
        cin >> n;
    }
    system("cls");
    Minesweeper A = Minesweeper(row, col, n);
    A.interact();
    return 0;
}

void Minesweeper::DFS(vector<vector<char>>& board, int x, int y)
{
    if (board[0].size() * board.size() == 1)
    {
        board[0][0] = 'B';
        return;
    }
    recurCnt++;
    if (recurCnt > board[0].size() * board.size()) //防止栈溢出
        return;
    //两种逻辑
    //1. 如果当前位置（即输入的参数x和y）周围有雷计数（不管有没有雷），如果计数结果非零，则修改本位置为数字
    //2. 如果计数结果为0，则将修改本位置为B，并对周围8内未被挖出的格子进行递归执行1和2
    int sx, sy;
    int cnt = 0;
    for (int i = 0; i < 8; i++)
    {
        sx = x + dx[i];
        sy = y + dy[i];
        if (sx < 0 || sy < 0 || sx >= board.size() || sy >= board[0].size()) //碰到面板边界
            continue;
        else if (board[sx][sy] == 'M')
            cnt++;
    }
    if (cnt)
        board[x][y] = '0' + cnt; //递归出口
    else
    {
        board[x][y] = 'B';
        for (int i = 0; i < 8; i++)
        {
            sx = x + dx[i];
            sy = y + dy[i];
            if (sx < 0 || sy < 0 || sx >= board.size() || sy >= board[0].size() || board[sx][sy] != 'E') //碰到面板边界或碰到已经挖出的格子
                continue;
            else
                DFS(board, sx, sy); //深度优先递归
        }
    }
    return;
}

vector<vector<char>> Minesweeper::updateBoard(vector<vector<char>>& board, int x, int y)
{
    if (board[x][y] == 'M') //踩雷，直接人没了
    {
        board[x][y] += 11;
        defeat = true;
    }
    else //没踩雷
        DFS(board, x, y);
    return board;
}

vector<vector<char>> Minesweeper::init()
{
    srand(time(0));
    int x = Minesweeper::ROW;
    int y = Minesweeper::COL;
    int n = Minesweeper::N;
    int* mine = new int[n];
    int* total = new int[x * y];
    int index, temp;
    for (int i = 0; i < x * y; i++)
        total[i] = i;
    for (int i = 0; i < n; i++)
    {
        index = rand() % (x * y - 1 - i);
        mine[i] = total[index]; //随机选雷，并记录雷的位置
        //避免重复
        temp = total[index];
        total[index] = total[x * y - 1 - i];
        total[x * y - 1 - i] = temp;
    }
    vector<vector<char>> board;
    vector<char> subBoard;
    for (int j = 0; j < y; j++)
        subBoard.push_back('E');
    for (int i = 0; i < x; i++)
        board.push_back(subBoard);
    for (int i = 0; i < n; i++)
        board[mine[i] / y][mine[i] % y] = 'M';
    delete[] mine;
    delete[] total;
    return board;
}

void Minesweeper::interact()
{
    int x = Minesweeper::ROW;
    int y = Minesweeper::COL;
    int n = Minesweeper::N;
    int clickx, clicky;
    vector<vector<char>> board = init();

    //界面
    cout << "地雷数量：" << Minesweeper::N << endl << endl;
    cout << setiosflags(ios::left) << setw(4);
    cout << "  ";
    for (int i = 0; i < y; i++)
    {
        cout << setiosflags(ios::left) << setw(3);
        cout << i + 1;
    }
    cout << endl << endl;
    for (int i = 0; i < x; i++)
    {
        cout << setiosflags(ios::left) << setw(4);
        cout << i + 1;
        for (int j = 0; j < y; j++)
        {
            cout << setiosflags(ios::left) << setw(3);
            if (board[i][j] == 'M' || board[i][j] == 'E')
                cout << "??";
            else
                cout << board[i][j];
        }
        cout << endl;
    }

    int mineCnt;
    while (!(defeat || victory))
    {
        mineCnt = 0;
        cout << "输入行标和列标，以空格隔开：" << endl;
        cin >> clickx >> clicky;
        while (clickx > Minesweeper::ROW || clicky > Minesweeper::COL)
        {
            cout << "越界，请重新输入。" << endl;
            cin >> clickx >> clicky;
        }
        board = updateBoard(board, clickx - 1, clicky - 1);
        system("cls");

        //界面
        cout << "地雷数量：" << Minesweeper::N << endl << endl;
        cout << setiosflags(ios::left) << setw(4);
        cout << "  ";
        for (int i = 0; i < y; i++)
        {
            cout << setiosflags(ios::left) << setw(3);
            cout << i + 1;
        }
        cout << endl << endl;
        for (int i = 0; i < x; i++)
        {
            cout << setiosflags(ios::left) << setw(4);
            cout << i + 1;
            for (int j = 0; j < y; j++)
            {
                cout << setiosflags(ios::left) << setw(3);
                if (board[i][j] == 'M' || board[i][j] == 'E')
                {
                    cout << "??";
                    mineCnt++;
                }
                else
                    cout << board[i][j];
            }
            cout << endl;
        }

        if (mineCnt == n && !defeat)
        {

            system("cls");
            cout << "地雷数量：" << Minesweeper::N << endl << endl;
            Minesweeper::victory = true;
            cout << setiosflags(ios::left) << setw(4);
            cout << "  ";
            for (int i = 0; i < y; i++)
            {
                cout << setiosflags(ios::left) << setw(3);
                cout << i + 1;
            }
            cout << endl << endl;
            for (int i = 0; i < x; i++)
            {
                cout << setiosflags(ios::left) << setw(4);
                cout << i + 1;
                for (int j = 0; j < y; j++)
                {
                    cout << setiosflags(ios::left) << setw(3);
                    if (board[i][j] == 'M' || board[i][j] == 'E')
                        cout << "X";
                    else
                        cout << board[i][j];
                }
                cout << endl;
            }
        }
    }
    cout << endl;
    if (Minesweeper::defeat)
        cout << "DEFEAT!" << endl;
    if (Minesweeper::victory)
        cout << "VICTORY!" << endl;
    return;
}
