#include <string.h>
#include <stdio.h>
#include <queue>
#include <vector>
#include <stack>
#include "../include/playerbase.h"
#define INT_MAX 0x3f3f3f3f

// 定义方向数组
int step[4][2] = { {0, 1}, {0, -1}, {1, 0}, {-1, 0} };

void init(struct Player *player) {
    // This function will be executed at the beginning of each game, only once.
}

bool isEdge(struct Player *player, int x, int y) {
    return x == 0 || y == 0 || x == player->row_cnt - 1 || y == player->col_cnt - 1;
}

bool isValidMove(struct Player *player, int x, int y) {
    return x >= 0 && x < player->row_cnt && y >= 0 && y < player->col_cnt && (player->mat[x][y] == '.' || player->mat[x][y] == 'o');
}

// DFS 检查是否可以找到一个安全的路径
bool isSafePath(struct Player *player, int startX, int startY, int length) {
    std::vector<std::vector<bool>> visited(player->row_cnt, std::vector<bool>(player->col_cnt, false));
    std::stack<struct Point> stack;
    stack.push(initPoint(startX, startY));
    visited[startX][startY] = true;
    int count = 1;

    while (!stack.empty()) {
        struct Point current = stack.top();
        stack.pop();

        for (int i = 0; i < 4; i++) {
            int dx = current.X + step[i][0];
            int dy = current.Y + step[i][1];

            if (isValidMove(player, dx, dy) && !visited[dx][dy]) {
                stack.push(initPoint(dx, dy));
                visited[dx][dy] = true;
                count++;

                // 如果找到的路径长度大于等于蛇身的长度，则认为是安全的路径
                if (count >= length) {
                    return true;
                }
            }
        }
    }

    return false;
}

struct Point walk(struct Player *player) {
    // This function will be executed in each round.
    int rows = player->row_cnt;
    int cols = player->col_cnt;

    // 如果是第39轮，并且在边缘，则向中心移动一步
    if (player->round == 39 && isEdge(player, player->your_posx, player->your_posy)) {
        for (int i = 0; i < 4; i++) {
            int dx = player->your_posx + step[i][0];
            int dy = player->your_posy + step[i][1];

            // 判断是否在范围内且位置可以走（'.' 或 'o'）
            if (dx >= 1 && dx < rows - 1 && dy >= 1 && dy < cols - 1 && (player->mat[dx][dy] == '.' || player->mat[dx][dy] == 'o')) {
                return initPoint(dx, dy);
            }
        }
    }

    std::queue<struct Point> queue;
    std::vector<std::vector<bool>> visited(rows, std::vector<bool>(cols, false));
    std::vector<std::vector<struct Point>> parent(rows, std::vector<struct Point>(cols, initPoint(-1, -1)));

    // 起始点入队
    struct Point start = initPoint(player->your_posx, player->your_posy);
    queue.push(start);
    visited[start.X][start.Y] = true;

    struct Point bestApple = initPoint(-1, -1);
    int shortestPath = INT_MAX;

    // BFS 寻找所有苹果
    while (!queue.empty()) {
        struct Point current = queue.front();
        queue.pop();

        // 遍历四个方向
        for (int i = 0; i < 4; i++) {
            int dx = current.X + step[i][0];
            int dy = current.Y + step[i][1];

            // 判断是否在边界内且未访问过，且位置可以走（'.' 或 'o'）
            if (dx >= 0 && dx < rows && dy >= 0 && dy < cols && !visited[dx][dy] && (player->mat[dx][dy] == '.' || player->mat[dx][dy] == 'o')) {
                struct Point next = initPoint(dx, dy);
                queue.push(next);
                visited[dx][dy] = true;
                parent[dx][dy] = current;

                // 如果找到苹果'o'
                if (player->mat[dx][dy] == 'o') {
                    int pathLength = 1;
                    struct Point p = next;
                    while (!(parent[p.X][p.Y].X == start.X && parent[p.X][p.Y].Y == start.Y)) {
                        p = parent[p.X][p.Y];
                        pathLength++;
                    }

                    // 检查吃掉苹果后是否有逃逸路径
                    // 仅保留吃苹果路径上的访问标记
                    std::vector<std::vector<bool>> tempVisited = visited;
                    struct Point tempP = next;
                    while (!(parent[tempP.X][tempP.Y].X == start.X && parent[tempP.X][tempP.Y].Y == start.Y)) {
                        tempVisited[tempP.X][tempP.Y] = true;
                        tempP = parent[tempP.X][tempP.Y];
                    }
                    tempVisited[start.X][start.Y] = true;  // 标记起点

                    if (isSafePath(player, dx, dy, pathLength + 1)) {
                        if (pathLength < shortestPath) {
                            shortestPath = pathLength;
                            bestApple = next;
                            visited = tempVisited;  // 更新为保留路径标记的访问标记
                        }
                    }
                }
            }
        }
    }

    // 如果找到安全的苹果路径，返回路径上的第一个点
    if (bestApple.X != -1 && bestApple.Y != -1) {
        struct Point p = bestApple;
        while (!(parent[p.X][p.Y].X == start.X && parent[p.X][p.Y].Y == start.Y)) {
            p = parent[p.X][p.Y];
        }
        return p;
    }

    // 如果没有找到安全的苹果路径，返回原位置
    return initPoint(player->your_posx, player->your_posy);
}
