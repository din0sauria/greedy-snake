#include <string.h>
#include <stdio.h>
#include <queue>
#include <vector>
#include <stack>
#include "../include/playerbase.h"
#define INT_MAX 0x3f3f3f3f

// Define direction array
int step[4][2] = { {0, 1}, {0, -1}, {1, 0}, {-1, 0} };

void init(struct Player *player) {
    // This function will be executed at the beginning of each game, only once.
}

bool isEdge(struct Player *player, int x, int y) {
    return x == player->round/40 || y == player->round/40 || x == player->row_cnt - 1-player->round/40 || y == player->col_cnt - 1-player->round/40;
}

bool isValidMove(struct Player *player, int x, int y) {
    return x >= player->round/39 && x < player->row_cnt-player->round/39 && y >= player->round/39 && y < player->col_cnt-player->round/39 && (player->mat[x][y] == '.' || player->mat[x][y] == 'o' || player->mat[x][y] == 'O');
}

// DFS check for a safe path
bool isSafePath(struct Player *player, int startX, int startY, const std::vector<std::vector<bool>>& visited) {
    std::stack<struct Point> stack;
    stack.push(initPoint(startX, startY));
    std::vector<std::vector<bool>> localVisited = visited;
    localVisited[startX][startY] = true;
    int count = 1;

    while (!stack.empty()) {
        struct Point current = stack.top();
        stack.pop();

        for (int i = 0; i < 4; i++) {
            int dx = current.X + step[i][0];
            int dy = current.Y + step[i][1];

            if (isValidMove(player, dx, dy) && !localVisited[dx][dy]) {
                stack.push(initPoint(dx, dy));
                localVisited[dx][dy] = true;
                count++;

                // If found a path longer than the snake's length, it's a safe path
                if (count > player->your_score) {
                    return true;
                }
            }
        }
    }

    return false;
}

struct Point walk(struct Player *player) {
    int rows = player->row_cnt;
    int cols = player->col_cnt;

    // Move to the center on the 39th round if at the edge
    if (player->round >= 37&& player->round <40&& isEdge(player, player->your_posx, player->your_posy)) {
        for (int i = 0; i < 4; i++) {
            int dx = player->your_posx + step[i][0];
            int dy = player->your_posy + step[i][1];

            // Check if within range and position is walkable ('.' or 'o' or 'O')
            if (dx >= 1 && dx < rows - 1 && dy >= 1 && dy < cols - 1 && (player->mat[dx][dy] == '.' || player->mat[dx][dy] == 'o' || player->mat[dx][dy] == 'O')) {
                return initPoint(dx, dy);
            }
        }
    }
    if (player->round >= 77&& player->round <80&& isEdge(player, player->your_posx, player->your_posy)) {
        for (int i = 0; i < 4; i++) {
            int dx = player->your_posx + step[i][0];
            int dy = player->your_posy + step[i][1];

            // Check if within range and position is walkable ('.' or 'o' or 'O')
            if (dx >= 2 && dx < rows - 2 && dy >= 2 && dy < cols - 2 && (player->mat[dx][dy] == '.' || player->mat[dx][dy] == 'o' || player->mat[dx][dy] == 'O')) {
                return initPoint(dx, dy);
            }
        }
    }

    std::queue<struct Point> queue;
    std::vector<std::vector<bool>> visited(rows, std::vector<bool>(cols, false));
    std::vector<std::vector<struct Point>> parent(rows, std::vector<struct Point>(cols, initPoint(-1, -1)));

    // Enqueue starting point
    struct Point start = initPoint(player->your_posx, player->your_posy);
    queue.push(start);
    visited[start.X][start.Y] = true;

    // BFS to find all apples
    while (!queue.empty()) {
        struct Point current = queue.front();
        queue.pop();

        // Traverse four directions
        for (int i = 0; i < 4; i++) {
            int dx = current.X + step[i][0];
            int dy = current.Y + step[i][1];

            // Check if within bounds, not visited, and walkable position ('.' or 'o' or 'O')
            if (isValidMove(player, dx, dy) && !visited[dx][dy] && player->mat[dx][dy] != '2') {
                struct Point next = initPoint(dx, dy);
                queue.push(next);
                visited[dx][dy] = true;
                parent[dx][dy] = current;

                // If found an apple ('o' or 'O')
                if (player->mat[dx][dy] == 'o' || player->mat[dx][dy] == 'O') {
                    int pathLength = 1;
                    struct Point p = next;
                    while (!(parent[p.X][p.Y].X == start.X && parent[p.X][p.Y].Y == start.Y)) {
                        p = parent[p.X][p.Y];
                        pathLength++;
                    }

                    // Check if there's an escape path after eating the apple
                    std::vector<std::vector<bool>> tempVisited = visited;
                    struct Point tempP = next;
                    while (!(parent[tempP.X][tempP.Y].X == start.X && parent[tempP.X][tempP.Y].Y == start.Y)) {
                        tempVisited[tempP.X][tempP.Y] = true;
                        tempP = parent[tempP.X][tempP.Y];
                    }
                    tempVisited[start.X][start.Y] = true;  // Mark the start point

                    if (isSafePath(player, dx, dy, tempVisited)) {
                        while (!(parent[p.X][p.Y].X == start.X && parent[p.X][p.Y].Y == start.Y)) {
                            p = parent[p.X][p.Y];
                        }
                        return p;
                    }
                }
            }
        }
    }

    // If no safe apple path found, return the best possible move 
    struct Point current = initPoint(player->your_posx, player->your_posy);
    for (int i = 0; i < 4; i++) {
        int dx = current.X + step[i][0];
        int dy = current.Y + step[i][1];

        // Check if within bounds and walkable position ('.' or 'o' or 'O')
        if (isValidMove(player, dx, dy)) {
            return initPoint(dx, dy);
        }
    }

    // Stay in place if no valid moves
    return current;
}
