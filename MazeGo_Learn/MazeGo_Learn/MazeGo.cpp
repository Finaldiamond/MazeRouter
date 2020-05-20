#include <iostream>
#include <ctime>
#include <list>
#include <stack>
#include <vector>
#include <algorithm>
#include <easyx.h>
using namespace std;


// 迷宫格子状态
enum CellState：int { PATH = 0, WALL, FLAG };

// 迷宫格二维点结构
struct Point2
{
    int x, y;
    Point2(int _x, int _y) :x(_x), y(_y) {}
    bool operator == (const Point2& p2) { return x == p2.x && y == p2.y; }
};

// 迷宫大小（要求为奇数）
const int mazeSize = 21;


// 迷宫生成接口--递归版
void DFS_generator(int _x, int _y, std::vector<std::vector<int>>& maze)
{
    // 定义方向容器
    std::vector<std::vector<int>> dir{ {1,0},{-1,0},{0,1},{0,-1} };
    // 随机打乱方向
    std::random_shuffle(dir.begin(), dir.end());
    // 递归生成迷宫
    maze[_x][_y] = PATH;
    for (int i = 0; i < 4; ++i)
    {
        if (_x + 2 * dir[i][0] >= 1 && _x + 2 * dir[i][0] <= mazeSize - 2 && _y + 2 * dir[i][1] >= 1 && _y + 2 * dir[i][1] <= mazeSize - 2
            && maze[_x + 2 * dir[i][0]][_y + 2 * dir[i][1]] == WALL)
        {
            maze[_x + dir[i][0]][_y + dir[i][1]] = PATH;
            DFS_generator(_x + 2 * dir[i][0], _y + 2 * dir[i][1], maze);
        }
    }
}

// 迷宫生成接口--迭代版
void DFS_iterative_generator(std::vector<std::vector<int>>& maze)
{
    // 定义栈容器
    std::stack<Point2> sp;
    // 定义方向容器
    std::vector<std::vector<int>> dir{ {1,0},{-1,0},{0,1},{0,-1} };
    // 要求参数为奇数
    Point2 temp((rand() % (mazeSize - 2) + 1) | 1, (rand() % (mazeSize - 2) + 1) | 1);
    sp.push(temp);
    // 后续迭代生成迷宫，并绘制
    while (!sp.empty())
    {
        if (maze[temp.x][temp.y] != PATH)
            maze[temp.x][temp.y] = PATH;
        // 随机打乱方向
        std::random_shuffle(dir.begin(), dir.end());
        int i = 0;
        for (; i < 4; ++i)
        {
            if (temp.x + 2 * dir[i][0] >= 1 && temp.x + 2 * dir[i][0] <= mazeSize - 2 && temp.y + 2 * dir[i][1] >= 1 && temp.y + 2 * dir[i][1] <= mazeSize - 2
                && maze[temp.x + 2 * dir[i][0]][temp.y + 2 * dir[i][1]] == WALL)
            {
                maze[temp.x + dir[i][0]][temp.y + dir[i][1]] = PATH;
                temp.x += 2 * dir[i][0];
                temp.y += 2 * dir[i][1];
                sp.push(temp);
                break;
            }
        }
        if (i == 4) sp.pop();
        if (!sp.empty()) temp = sp.top();
    }
}

// DFS 寻路
std::list<Point2> DFS_find(const Point2& start, const Point2& end, std::vector<std::vector<int>>& maze)
{
    // 定义方向容器（右、下、左、上）
    std::vector<std::vector<int>> dir{ {1,0},{0,1},{-1,0},{0,-1} };
    // 定义栈容器
    std::list<Point2> sp;
    Point2 temp = start;
    sp.push_back(temp);
    // 与生成算法相似，迭代 DFS 遍历
    while (!sp.empty())
    {
        int i = 0;
        // 若可向其他方向前进，则继续
        for (; i < 4; ++i)
        {
            if (temp.x + dir[i][0] >= 0 && temp.x + dir[i][0] <= mazeSize - 1 && temp.y + dir[i][1] >= 0 && temp.y + dir[i][1] <= mazeSize - 1
                && maze[temp.x + dir[i][0]][temp.y + dir[i][1]] == PATH)
            {
                // 对走过的路进行标记
                maze[temp.x][temp.y] = FLAG;
                temp.x += dir[i][0];
                temp.y += dir[i][1];
                sp.push_back(temp);
                if (temp == end) return sp;
                break;
            }
        }
        // 无路可走时回溯
        if (i == 4)
        {
            maze[temp.x][temp.y] = FLAG;
            sp.pop_back();
            if (!sp.empty()) temp = sp.back();
        }
    }
    // 将被标记的通路还原
    for (auto& v1 : maze)
        for (auto& v2 : v1)
            if (v2 == FLAG) v2 = PATH;

    return sp;
}


// main 函数
int main()
{
    srand((unsigned)time(nullptr));

    // 入口出口
    Point2 start(0, 1);
    Point2 end(mazeSize - 1, mazeSize - 2);

    // 二维迷宫容器
    std::vector<std::vector<int>> maze;

    // 初始化迷宫
    for (int i = 0; i < mazeSize; ++i) maze.push_back(std::vector<int>());
    for (int i = 0; i < mazeSize; ++i)
        for (int j = 0; j < mazeSize; ++j)
            maze[i].push_back(WALL);
    maze[start.x][start.y] = maze[end.x][end.y] = PATH;

    // 生成迷宫
    DFS_iterative_generator(maze);

    // 打印迷宫
    for (int j = 0; j < mazeSize; ++j)
    {
        for (int i = 0; i < mazeSize; ++i)
            cout << maze[i][j] << " ";
        cout << endl;
    }

    // 寻路
    auto sp = DFS_find(start, end, maze);
    // 打印路径
    cout << endl << "寻路路径：" << endl;
    int i = 0;
    for (auto p2 : sp)
    {
        if (i++ == 5)
        {
            i = 0;
            cout << "(" << p2.x << ", " << p2.y << ")" << endl;
            continue;
        }
        cout << "(" << p2.x << ", " << p2.y << ")" << "、";
    }
    cout << endl;

    // EasyX
    {
        auto ret = _getwch();
        const int width = 15;
        initgraph(mazeSize * width, mazeSize * width);
        setlinecolor(DARKGRAY);
        setfillcolor(LIGHTGRAY);
        for (int j = 0; j < mazeSize; ++j)
            for (int i = 0; i < mazeSize; ++i)
                if (maze[i][j] == WALL)
                    fillrectangle(i * width, j * width, i * width + width - 1, j * width + width - 1);
        setfillcolor(YELLOW);
        for (auto p2 : sp) fillrectangle(p2.x * width, p2.y * width, p2.x * width + width - 1, p2.y * width + width - 1);
        // saveimage(_T("D:\\maze.png"));
        ret = _getwch();
        closegraph();
    }

    return 0;
}