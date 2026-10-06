#ifndef GAME_H
#define GAME_H

#include <genesis.h>

typedef struct
{
    s16 x;
    s16 y;
    u8 hp;
    u8 keys;
} Player;

typedef enum
{
    SCREEN_TITLE,
    SCREEN_PLAYING,
    SCREEN_WIN
} GameState;

void gameInit(void);
void gameReset(void);
void gameUpdate(void);
void gameRender(void);

#endif
