#include <genesis.h>
#include <stdlib.h>

#include "game.h"

#define MAX_KEYS 3
#define MAX_ENEMIES 4
#define PLAYER_START_X 1
#define PLAYER_START_Y 1
#define EXIT_X 16
#define EXIT_Y 10

static char map[MAP_H][MAP_W];
static Player player;
static Enemy enemies[MAX_ENEMIES];
static u8 keyTaken[MAX_KEYS];
static u8 gameOver;
static u8 victory;
static u16 frameCount;

static void setTile(s16 x, s16 y, char value)
{
    if (x >= 0 && x < MAP_W && y >= 0 && y < MAP_H)
    {
        map[y][x] = value;
    }
}

static u8 isWall(s16 x, s16 y)
{
    if (x < 0 || x >= MAP_W || y < 0 || y >= MAP_H)
    {
        return 1;
    }

    return map[y][x] == '#';
}

static u8 isEnemyAt(s16 x, s16 y)
{
    for (u8 i = 0; i < MAX_ENEMIES; ++i)
    {
        if (enemies[i].alive && enemies[i].x == x && enemies[i].y == y)
        {
            return 1;
        }
    }

    return 0;
}

static u8 isKeyAt(s16 x, s16 y, u8 keyIndex)
{
    if (keyTaken[keyIndex])
    {
        return 0;
    }

    static const s16 keyPos[MAX_KEYS][2] = {
        { 3, 2 },
        { 12, 7 },
        { 7, 9 }
    };

    return keyPos[keyIndex][0] == x && keyPos[keyIndex][1] == y;
}

static void buildLevel(void)
{
    for (s16 y = 0; y < MAP_H; ++y)
    {
        for (s16 x = 0; x < MAP_W; ++x)
        {
            map[y][x] = '.';
        }
    }

    for (s16 x = 0; x < MAP_W; ++x)
    {
        setTile(x, 0, '#');
        setTile(x, MAP_H - 1, '#');
    }

    for (s16 y = 0; y < MAP_H; ++y)
    {
        setTile(0, y, '#');
        setTile(MAP_W - 1, y, '#');
    }

    for (s16 x = 4; x < 14; ++x)
    {
        setTile(x, 4, '#');
    }

    for (s16 y = 2; y < 9; ++y)
    {
        setTile(8, y, '#');
    }

    for (s16 x = 11; x < 16; ++x)
    {
        setTile(x, 7, '#');
    }

    for (s16 x = 5; x < 9; ++x)
    {
        setTile(x, 9, '#');
    }

    setTile(EXIT_X, EXIT_Y, 'E');
}

static void placeEnemies(void)
{
    static const s16 startPositions[MAX_ENEMIES][2] = {
        { 14, 2 },
        { 13, 8 },
        { 4, 7 },
        { 15, 5 }
    };

    for (u8 i = 0; i < MAX_ENEMIES; ++i)
    {
        enemies[i].x = startPositions[i][0];
        enemies[i].y = startPositions[i][1];
        enemies[i].alive = 1;
    }
}

void gameInit(void)
{
    VDP_setScreenWidth320();
    VDP_setScreenHeight224();
    VDP_setTextPlane(BG_A);
    gameReset();
}

void gameReset(void)
{
    buildLevel();
    player.x = PLAYER_START_X;
    player.y = PLAYER_START_Y;
    player.health = 5;
    player.keys = 0;
    gameOver = 0;
    victory = 0;
    frameCount = 0;
    for (u8 i = 0; i < MAX_KEYS; ++i)
    {
        keyTaken[i] = 0;
    }
    placeEnemies();
}

static void tryPlayerMove(s16 dx, s16 dy)
{
    if (gameOver || victory)
    {
        return;
    }

    s16 nextX = player.x + dx;
    s16 nextY = player.y + dy;

    if (!isWall(nextX, nextY) && !isEnemyAt(nextX, nextY))
    {
        player.x = nextX;
        player.y = nextY;
    }

    for (u8 i = 0; i < MAX_KEYS; ++i)
    {
        static const s16 keyPos[MAX_KEYS][2] = {
            { 3, 2 },
            { 12, 7 },
            { 7, 9 }
        };

        if (!keyTaken[i] && player.x == keyPos[i][0] && player.y == keyPos[i][1])
        {
            keyTaken[i] = 1;
            player.keys++;
        }
    }

    if (player.x == EXIT_X && player.y == EXIT_Y && player.keys >= MAX_KEYS)
    {
        victory = 1;
    }
}

static void moveEnemies(void)
{
    for (u8 i = 0; i < MAX_ENEMIES; ++i)
    {
        if (!enemies[i].alive)
        {
            continue;
        }

        s16 dx = 0;
        s16 dy = 0;

        if (frameCount % 18 == i)
        {
            if (player.x > enemies[i].x)
            {
                dx = 1;
            }
            else if (player.x < enemies[i].x)
            {
                dx = -1;
            }

            if (player.y > enemies[i].y)
            {
                dy = 1;
            }
            else if (player.y < enemies[i].y)
            {
                dy = -1;
            }

            if (dx != 0 && dy != 0)
            {
                if (rand() & 1)
                {
                    dy = 0;
                }
                else
                {
                    dx = 0;
                }
            }

            s16 nextX = enemies[i].x + dx;
            s16 nextY = enemies[i].y + dy;

            if (!isWall(nextX, nextY) && !(nextX == player.x && nextY == player.y))
            {
                enemies[i].x = nextX;
                enemies[i].y = nextY;
            }

            if (enemies[i].x == player.x && enemies[i].y == player.y)
            {
                player.health--;
                if (player.health == 0)
                {
                    gameOver = 1;
                }
            }
        }
    }
}

void gameUpdate(void)
{
    if (gameOver || victory)
    {
        u16 pad = JOY_readJoypad(JOY_1);
        if (pad & BUTTON_START)
        {
            gameReset();
        }
        return;
    }

    frameCount++;

    u16 pad = JOY_readJoypad(JOY_1);
    s16 dx = 0;
    s16 dy = 0;

    if (pad & BUTTON_LEFT)
    {
        dx = -1;
    }
    else if (pad & BUTTON_RIGHT)
    {
        dx = 1;
    }

    if (pad & BUTTON_UP)
    {
        dy = -1;
    }
    else if (pad & BUTTON_DOWN)
    {
        dy = 1;
    }

    if (dx != 0 || dy != 0)
    {
        tryPlayerMove(dx, dy);
    }

    moveEnemies();
}

void gameRender(void)
{
    VDP_clearTextArea(BG_A, 0, 0, 40, 28);

    char line[MAP_W + 1];
    char finalText[64];

    for (s16 y = 0; y < MAP_H; ++y)
    {
        for (s16 x = 0; x < MAP_W; ++x)
        {
            char c = map[y][x];

            if (player.x == x && player.y == y)
            {
                c = 'P';
            }
            else if (isEnemyAt(x, y))
            {
                c = 'M';
            }
            else if (x == EXIT_X && y == EXIT_Y && player.keys >= MAX_KEYS)
            {
                c = 'E';
            }
            else if (x == EXIT_X && y == EXIT_Y)
            {
                c = 'E';
            }
            else if (isKeyAt(x, y, 0) || isKeyAt(x, y, 1) || isKeyAt(x, y, 2))
            {
                c = 'K';
            }

            line[x] = c;
        }

        line[MAP_W] = '\0';
        VDP_drawText(line, 2, 2 + y);
    }

    VDP_drawText("NIGHTMARE GENESIS", 10, 1);
    VDP_drawText("OBJECTIVE: GET 3 KEYS", 2, 15);

    sprintf(finalText, "HP:%u  KEYS:%u/3", player.health, player.keys);
    VDP_drawText(finalText, 2, 16);

    if (victory)
    {
        VDP_drawText("YOU ESCAPED!", 10, 20);
        VDP_drawText("PRESS START TO TRY AGAIN", 5, 22);
    }
    else if (gameOver)
    {
        VDP_drawText("THE HOUSE WON", 10, 20);
        VDP_drawText("PRESS START TO RESTART", 5, 22);
    }
}

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
