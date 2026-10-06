#include <genesis.h>
#include <string.h>

#include "game.h"

static Player player;
static GameState state;
static u16 frameCounter;

static void initPlayer(void)
{
    player.x = 1;
    player.y = 1;
    player.hp = 5;
    player.keys = 0;
}

void gameInit(void)
{
    VDP_setScreenWidth320();
    VDP_setScreenHeight224();
    VDP_setTextPlane(BG_A);

    VDP_clearTextArea(BG_A, 0, 0, 40, 28);
    gameReset();
}

void gameReset(void)
{
    initPlayer();
    frameCounter = 0;
    state = SCREEN_TITLE;
}

void gameUpdate(void)
{
    u16 joy = JOY_readJoypad(JOY_1);

    if (state == SCREEN_TITLE)
    {
        if (joy & BUTTON_START)
        {
            state = SCREEN_PLAYING;
        }
        return;
    }

    frameCounter++;

    s16 dx = 0;
    s16 dy = 0;

    if (joy & BUTTON_LEFT)
    {
        dx = -1;
    }
    else if (joy & BUTTON_RIGHT)
    {
        dx = 1;
    }

    if (joy & BUTTON_UP)
    {
        dy = -1;
    }
    else if (joy & BUTTON_DOWN)
    {
        dy = 1;
    }

    if (dx != 0 || dy != 0)
    {
        if (player.x + dx >= 0 && player.x + dx < 20)
        {
            player.x += dx;
        }

        if (player.y + dy >= 0 && player.y + dy < 15)
        {
            player.y += dy;
        }
    }

    if (player.x >= 15 && player.y >= 10)
    {
        state = SCREEN_WIN;
    }
}

void gameRender(void)
{
    char text[32];

    VDP_clearTextArea(BG_A, 0, 0, 40, 28);

    if (state == SCREEN_TITLE)
    {
        VDP_drawText("NIGHTMARE GENESIS", 8, 4);
        VDP_drawText("PRESS START", 10, 8);
        VDP_drawText("MOVE WITH D-PAD", 7, 12);
        return;
    }

    sprintf(text, "HP:%u  KEYS:%u", player.hp, player.keys);
    VDP_drawText(text, 2, 1);

    sprintf(text, "X:%d  Y:%d", player.x, player.y);
    VDP_drawText(text, 2, 2);

    VDP_drawText("P", player.x + 2, player.y + 3);

    if (state == SCREEN_WIN)
    {
        VDP_drawText("YOU ESCAPED!", 10, 10);
        VDP_drawText("PRESS START TO RESTART", 5, 12);
    }
    else
    {
        VDP_drawText("GOAL: REACH EXIT", 8, 18);
    }
}
