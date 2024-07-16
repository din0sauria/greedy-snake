/**
 * @file playerbase.h
 * @author yangboyang@jisuanke.com
 * @copyright jisuanke.com
 * @date 2021-07-01
 */

#ifndef INCLUDE_PLAYER_BASE_H_
#define INCLUDE_PLAYER_BASE_H_

struct Point {
    int X, Y;
};

struct Player {
    char **mat;  // Game Data

    int row_cnt;  // Count of rows in current map

    int col_cnt;  // Count of columns in current map

    int your_posx;  // X-corordinate of your AI's position, index from 0
    int your_posy;  // Y-corordinate of your AI's position, index from 0

    int opponent_posx;  // X-corordinate of opponent AI's position, index from 0
    int opponent_posy;  // Y-corordinate of opponent AI's position, index from 0

    int your_status;  // Your AI's status, -1 means died, x(x > 0) means your AI's
                      // super status will sustain in x more rounds, 0 means normal status.

    int opponent_status;  // Opponent AI's status, -1 means died, x(x > 0) means
                          // opponent AI's super status will sustain in x more rounds, 0 means normal status.

    int your_score;  // Your AI's score

    int opponent_score;  // Opponent AI's score
    
    int round;   // current round, begin from 1

    int round_to_shrink;  // current round to shrink, 1 means shrinking next round
};

struct Point walk(struct Player *player);  // You need to implement

struct Point initPoint(int x, int y);

void init(struct Player *player);  // init function

void _recv(struct Player *player, int *read_fd);  // Recieve data from judge process

void _send(struct Player *player, int *write_fd, char *sendbuf);  // Send data to judge process

void _work(struct Player *player, int *read_fd, int *write_fd, int playid);  // Entrance of current process

void* _init_thread(void *arg);  // Init your AI or opponent's AI

void* _walk_thread(void *arg);  // Simulate AI's walking

void _syscall_check(struct Player *player, const char* data_path);

#endif  // INCLUDE_PLAYERBASE_H_
