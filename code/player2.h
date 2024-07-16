#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/playerbase.h"
#define INT_MAX 0x3f3f3f3f

// Define direction array
int step[4][2] = { {0, 1}, {0, -1}, {1, 0}, {-1, 0} };

void init(struct Player *player) {
    // This function will be executed at the beginning of each game, only once.
}

int isEdge(struct Player *player, int x, int y) {
    return x == 0 || y == 0 || x == player->row_cnt - 1 || y == player->col_cnt - 1;
}

int isValidMove(struct Player *player, int x, int y) {
    return x >= 0 && x < player->row_cnt && y >= 0 && y < player->col_cnt && (player->mat[x][y] == '.' || player->mat[x][y] == 'o' || player->mat[x][y] == 'O');
}

struct Point {
    int X, Y;
};

struct Point initPoint(int x, int y) {
    struct Point p;
    p.X = x;
    p.Y = y;
    return p;
}

// DFS check for a safe path
int isSafePath(struct Player *player, int startX, int startY, int **visited) {
    struct Point stack[player->row_cnt * player->col_cnt];
    int stack_size = 0;

    stack[stack_size++] = initPoint(startX, startY);
    int **localVisited = (int **)malloc(player->row_cnt * sizeof(int *));
    for (int i = 0; i < player->row_cnt; ++i) {
        localVisited[i] = (int *)malloc(player->col_cnt * sizeof(int));
        memcpy(localVisited[i], visited[i], player->col_cnt * sizeof(int));
    }

    localVisited[startX][startY] = 1;
    int count = 1;

    while (stack_size > 0) {
        struct Point current = stack[--stack_size];

        for (int i = 0; i < 4; i++) {
            int dx = current.X + step[i][0];
            int dy = current.Y + step[i][1];

            if (isValidMove(player, dx, dy) && !localVisited[dx][dy]) {
                stack[stack_size++] = initPoint(dx, dy);
                localVisited[dx][dy] = 1;
                count++;

                // If found a path longer than the snake's length, it's a safe path
                if (count > player->your_score) {
                    for (int i = 0; i < player->row_cnt; ++i) free(localVisited[i]);
                    free(localVisited);
                    return 1;
                }
            }
        }
    }

    for (int i = 0; i < player->row_cnt; ++i) free(localVisited[i]);
    free(localVisited);
    return 0;
}

struct Point walk(struct Player *player) {
    int rows = player->row_cnt;
    int cols = player->col_cnt;

    // Move to the center on the 39th round if at the edge
    if (player->round == 39 && isEdge(player, player->your_posx, player->your_posy)) {
        for (int i = 0; i < 4; i++) {
            int dx = player->your_posx + step[i][0];
            int dy = player->your_posy + step[i][1];

            // Check if within range and position is walkable ('.' or 'o' or 'O')
            if (dx >= 1 && dx < rows - 1 && dy >= 1 && dy < cols - 1 && (player->mat[dx][dy] == '.' || player->mat[dx][dy] == 'o' || player->mat[dx][dy] == 'O')) {
                return initPoint(dx, dy);
            }
        }
    }

    struct Point queue[rows * cols];
    int front = 0, rear = 0;
    int **visited = (int **)malloc(rows * sizeof(int *));
    for (int i = 0; i < rows; ++i) {
        visited[i] = (int *)calloc(cols, sizeof(int));
    }

    struct Point **parent = (struct Point **)malloc(rows * sizeof(struct Point *));
    for (int i = 0; i < rows; ++i) {
        parent[i] = (struct Point *)malloc(cols * sizeof(struct Point));
        for (int j = 0; j < cols; ++j) {
            parent[i][j] = initPoint(-1, -1);
        }
    }

    // Enqueue starting point
    struct Point start = initPoint(player->your_posx, player->your_posy);
    queue[rear++] = start;
    visited[start.X][start.Y] = 1;

    // BFS to find all apples
    while (front != rear) {
        struct Point current = queue[front++];

        // Traverse four directions
        for (int i = 0; i < 4; i++) {
            int dx = current.X + step[i][0];
            int dy = current.Y + step[i][1];

            // Check if within bounds, not visited, and walkable position ('.' or 'o' or 'O')
            if (isValidMove(player, dx, dy) && !visited[dx][dy] && player->mat[dx][dy] != '2') {
                struct Point next = initPoint(dx, dy);
                queue[rear++] = next;
                visited[dx][dy] = 1;
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
                    int **tempVisited = (int **)malloc(rows * sizeof(int *));
                    for (int i = 0; i < rows; ++i) {
                        tempVisited[i] = (int *)malloc(cols * sizeof(int));
                        memcpy(tempVisited[i], visited[i], cols * sizeof(int));
                    }

                    struct Point tempP = next;
                    while (!(parent[tempP.X][tempP.Y].X == start.X && parent[tempP.X][tempP.Y].Y == start.Y)) {
                        tempVisited[tempP.X][tempP.Y] = 1;
                        tempP = parent[tempP.X][tempP.Y];
                    }
                    tempVisited[start.X][start.Y] = 1;  // Mark the start point

                    if (isSafePath(player, dx, dy, tempVisited)) {
                        for (int i = 0; i < rows; ++i) free(tempVisited[i]);
                        free(tempVisited);
                        while (!(parent[p.X][p.Y].X == start.X && parent[p.X][p.Y].Y == start.Y)) {
                            p = parent[p.X][p.Y];
                        }
                        for (int i = 0; i < rows; ++i) {
                            free(parent[i]);
                            free(visited[i]);
                        }
                        free(parent);
                        free(visited);
                        return p;
                    }

                    for (int i = 0; i < rows; ++i) free(tempVisited[i]);
                    free(tempVisited);
                }
            }
        }
    }

    // If no safe apple path found, return the best possible move or stay in place
    struct Point current = initPoint(player->your_posx, player->your_posy);
    for (int i = 0; i < 4; i++) {
        int dx = current.X + step[i][0];
        int dy = current.Y + step[i][1];

        // Check if within bounds and walkable position ('.' or 'o' or 'O')
        if (isValidMove(player, dx, dy) && player->mat[dx][dy] != '2') {
            for (int i = 0; i < rows; ++i) {
                free(parent[i]);
                free(visited[i]);
            }
            free(parent);
            free(visited);
            return initPoint(dx, dy);
        }
    }

    // Free allocated memory
    for (int i = 0; i < rows; ++i) {
        free(parent[i]);
        free(visited[i]);
    }
    free(parent);
    free(visited);

    // Stay in place if no valid moves
    return current;
}
