/**
 * @file main_player.c
 * @author yangboyang@jisuanke.com
 * @copyright jisuanke.com
 * @date 2021-07-01
 */

#include <stdlib.h>
#include <assert.h>

#include "../code/player.h"


int main(int argc, char *argv[]) {
    assert(argc >= 4);
    int playid = atoi(argv[3]);
    int read_fd, write_fd;
    read_fd = atoi(argv[1]);
    write_fd = atoi(argv[2]);
    struct Player red;
    red.mat = NULL;
    _work(&red, &read_fd, &write_fd, playid);
    return 0;
}
