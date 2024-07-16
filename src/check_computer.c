/**
 * @file check_computer.cpp
 * @author zhengaohong@jisuanke.com
 * @copyright jisuanke.com
 * @version 0.0
 * @date 2016-08-01
 */


#include <assert.h>

#include "../code/computer.h"

int main(int argc, char **argv) {
    assert(argc > 1);
    struct Player computer;
    computer.mat = NULL;
    _syscall_check(&computer, argv[1]);
    return 0;
}
