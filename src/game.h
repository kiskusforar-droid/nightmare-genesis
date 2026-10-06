#ifndef GAME_H
#define GAME_H

#include <genesis.h>

#define MAP_W 20
#define MAP_H 15
#define MAX_KEYS 3
#define MAX_ENEMIES 5

#define PLAYER_START_X 1
#define PLAYER_START_Y 1
#define EXIT_X 17
#define EXIT_Y 12

typedef struct
{
    s16 x;
    s16 y;
    u8 hp;
    u8 keys;
    u8 invulnerable;
} Player;

typedef struct
{
    s16 x;
    s16 y;
    u8 alive;
    u8 speed;
} Enemy;

typedef enum
{
    SCREEN_TITLE,
    SCREEN_PLAYING,
    SCREEN_WIN,
    SCREEN_LOSE
} GameState;

void gameInit(void);
void gameReset(void);
void gameUpdate(void);
void gameRender(void);

#endif
