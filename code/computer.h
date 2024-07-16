/**
 * @file player.h
 * @author yangboyang@jisuanke.com
 * @copyright jisuanke.com
 * @date 2021-07-01
 */

#include <string.h>
#include "../include/playerbase.h"
#include <stdio.h>

void init(struct Player *player) {
	// This function will be executed at the begin of each game, only once.
}

struct Point walk(struct Player *player) {
	return initPoint(player->your_posx, player->your_posy);
}
