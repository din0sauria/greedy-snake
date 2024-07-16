/**
 * @file check_player.cpp
 * @author zhengaohong@jisuanke.com
 * @copyright jisuanke.com
 * @version 0.0
 * @date 2016-08-01
 */

#include <assert.h>

#include "../code/player.h"

int main(int argc, char **argv) {
    assert(argc > 1);
    struct Player player;
    player.mat = NULL;
    _syscall_check(&player, argv[1]);
    return 0;
}
