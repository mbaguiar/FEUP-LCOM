#include "Game.h"

void gameStart() {
        vg_init(0x117);
        gameCtrl_init();
        gameRun();
}

void gameRun() {
        handler_loop();
}
