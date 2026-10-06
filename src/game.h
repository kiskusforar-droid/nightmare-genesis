#ifndef GAME_H
#define GAME_H

#include <genesis.h>

#define MAP_W 18
#define MAP_H 12

typedef struct
{
    s16 x;
    s16 y;
    u8 health;
    u8 keys;
} Player;

typedef struct
{
    s16 x;
    s16 y;
    u8 alive;
} Enemy;

void gameInit(void);
void gameReset(void);
void gameUpdate(void);
void gameRender(void);

#endif
