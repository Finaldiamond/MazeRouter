#include <iostream>
#include <ctime>
#include <list>
#include <stack>
#include <vector>
#include <algorithm>
#include <easyx.h>
using namespace std;


// �Թ�����״̬
enum CellState��int { PATH = 0, WALL, FLAG };

// �Թ����ά��ṹ
struct Point2
{
    int x, y;
    Point2(int _x, int _y) :x(_x), y(_y) {}
    bool operator == (const Point2& p2) { return x == p2.x && y == p2.y; }
};

// �Թ���С��Ҫ��Ϊ������
const int mazeSize = 21;


// �Թ����ɽӿ�--�ݹ��
void DFS_generator(int _x, int _y, std::vector<std::vector<int>>& maze)
{
    // ���巽������
    std::vector<std::vector<int>> dir{ {1,0},{-1,0},{0,1},{0,-1} };
    // ������ҷ���
    std::random_shuffle(dir.begin(), dir.end());
    // �ݹ������Թ�
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

// �Թ����ɽӿ�--������
void DFS_iterative_generator(std::vector<std::vector<int>>& maze)
{
    // ����ջ����
    std::stack<Point2> sp;
    // ���巽������
    std::vector<std::vector<int>> dir{ {1,0},{-1,0},{0,1},{0,-1} };
    // Ҫ�����Ϊ����
    Point2 temp((rand() % (mazeSize - 2) + 1) | 1, (rand() % (mazeSize - 2) + 1) | 1);
    sp.push(temp);
    // �������������Թ���������
    while (!sp.empty())
    {
        if (maze[temp.x][temp.y] != PATH)
            maze[temp.x][temp.y] = PATH;
        // ������ҷ���
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

// DFS Ѱ·
std::list<Point2> DFS_find(const Point2& start, const Point2& end, std::vector<std::vector<int>>& maze)
{
    // ���巽���������ҡ��¡����ϣ�
    std::vector<std::vector<int>> dir{ {1,0},{0,1},{-1,0},{0,-1} };
    // ����ջ����
    std::list<Point2> sp;
    Point2 temp = start;
    sp.push_back(temp);
    // �������㷨���ƣ����� DFS ����
    while (!sp.empty())
    {
        int i = 0;
        // ��������������ǰ���������
        for (; i < 4; ++i)
        {
            if (temp.x + dir[i][0] >= 0 && temp.x + dir[i][0] <= mazeSize - 1 && temp.y + dir[i][1] >= 0 && temp.y + dir[i][1] <= mazeSize - 1
                && maze[temp.x + dir[i][0]][temp.y + dir[i][1]] == PATH)
            {
                // ���߹���·���б��
                maze[temp.x][temp.y] = FLAG;
                temp.x += dir[i][0];
                temp.y += dir[i][1];
                sp.push_back(temp);
                if (temp == end) return sp;
                break;
            }
        }
        // ��·����ʱ����
        if (i == 4)
        {
            maze[temp.x][temp.y] = FLAG;
            sp.pop_back();
            if (!sp.empty()) temp = sp.back();
        }
    }
    // ������ǵ�ͨ·��ԭ
    for (auto& v1 : maze)
        for (auto& v2 : v1)
            if (v2 == FLAG) v2 = PATH;

    return sp;
}


// main ����
int main()
{
    srand((unsigned)time(nullptr));

    // ��ڳ���
    Point2 start(0, 1);
    Point2 end(mazeSize - 1, mazeSize - 2);

    // ��ά�Թ�����
    std::vector<std::vector<int>> maze;

    // ��ʼ���Թ�
    for (int i = 0; i < mazeSize; ++i) maze.push_back(std::vector<int>());
    for (int i = 0; i < mazeSize; ++i)
        for (int j = 0; j < mazeSize; ++j)
            maze[i].push_back(WALL);
    maze[start.x][start.y] = maze[end.x][end.y] = PATH;

    // �����Թ�
    DFS_iterative_generator(maze);

    // ��ӡ�Թ�
    for (int j = 0; j < mazeSize; ++j)
    {
        for (int i = 0; i < mazeSize; ++i)
            cout << maze[i][j] << " ";
        cout << endl;
    }

    // Ѱ·
    auto sp = DFS_find(start, end, maze);
    // ��ӡ·��
    cout << endl << "Ѱ··����" << endl;
    int i = 0;
    for (auto p2 : sp)
    {
        if (i++ == 5)
        {
            i = 0;
            cout << "(" << p2.x << ", " << p2.y << ")" << endl;
            continue;
        }
        cout << "(" << p2.x << ", " << p2.y << ")" << "��";
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