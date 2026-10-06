#include <genesis.h>

#include "game.h"

int main(void)
{
    SYS_init();
    VDP_init();
    gameInit();

    while (1)
    {
        VDP_waitVSync();
        gameUpdate();
        gameRender();
    }

    return 0;
}
