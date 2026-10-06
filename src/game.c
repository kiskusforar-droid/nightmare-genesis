#include <genesis.h>
#include <string.h>

#include "game.h"

#define TITLE_LINE_1 "NIGHTMARE GENESIS"
#define TITLE_LINE_2 "PRESS START"
#define TITLE_LINE_3 "FIND 3 KEYS + ESCAPE"

static char map[MAP_H][MAP_W];
static Player player;
static Enemy enemies[MAX_ENEMIES];
static u8 keyTaken[MAX_KEYS];
static GameState state;
static u16 frameCounter;

static const s16 keyPositions[MAX_KEYS][2] = {
    { 3, 2 },
    { 14, 4 },
    { 8, 10 }
};

static const s16 enemyPositions[MAX_ENEMIES][2] = {
    { 15, 2 },
    { 9, 6 },
    { 16, 8 },
    { 4, 9 },
    { 12, 11 }
};

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
    return keyPositions[keyIndex][0] == x && keyPositions[keyIndex][1] == y;
}

static void buildMap(void)
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

    for (s16 x = 3; x < 9; ++x)
    {
        setTile(x, 4, '#');
    }

    for (s16 x = 11; x < 17; ++x)
    {
        setTile(x, 4, '#');
    }

    for (s16 y = 2; y < 8; ++y)
    {
        setTile(8, y, '#');
    }

    for (s16 y = 8; y < 12; ++y)
    {
        setTile(11, y, '#');
    }

    for (s16 x = 5; x < 14; ++x)
    {
        setTile(x, 8, '#');
    }

    setTile(EXIT_X, EXIT_Y, 'E');
}

static void placeEnemies(void)
{
    for (u8 i = 0; i < MAX_ENEMIES; ++i)
    {
        enemies[i].x = enemyPositions[i][0];
        enemies[i].y = enemyPositions[i][1];
        enemies[i].alive = 1;
        enemies[i].speed = 1 + (i % 2);
    }
}

static void tryPlayerMove(s16 dx, s16 dy)
{
    if (state != SCREEN_PLAYING)
    {
        return;
    }

    s16 nx = player.x + dx;
    s16 ny = player.y + dy;

    if (isWall(nx, ny) || isEnemyAt(nx, ny))
    {
        return;
    }

    player.x = nx;
    player.y = ny;

    for (u8 i = 0; i < MAX_KEYS; ++i)
    {
        if (!keyTaken[i] && player.x == keyPositions[i][0] && player.y == keyPositions[i][1])
        {
            keyTaken[i] = 1;
            player.keys++;
        }
    }

    if (player.x == EXIT_X && player.y == EXIT_Y && player.keys >= MAX_KEYS)
    {
        state = SCREEN_WIN;
    }
}

static void moveEnemies(void)
{
    for (u8 i = 0; i < MAX_ENEMIES; ++i)
    {
        if (!enemies[i].alive || state != SCREEN_PLAYING)
        {
            continue;
        }

        if ((frameCounter % (12 - enemies[i].speed)) != (u16)i)
        {
            continue;
        }

        s16 dx = 0;
        s16 dy = 0;

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
            if ((frameCounter + i) & 1)
            {
                dy = 0;
            }
            else
            {
                dx = 0;
            }
        }

        s16 nx = enemies[i].x + dx;
        s16 ny = enemies[i].y + dy;

        if (!isWall(nx, ny) && !(nx == player.x && ny == player.y))
        {
            enemies[i].x = nx;
            enemies[i].y = ny;
        }

        if (enemies[i].x == player.x && enemies[i].y == player.y)
        {
            if (player.invulnerable == 0)
            {
                player.hp--;
                player.invulnerable = 20;
                if (player.hp == 0)
                {
                    state = SCREEN_LOSE;
                }
            }
        }
    }
}

static void resetPlayerInvulnerability(void)
{
    if (player.invulnerable > 0)
    {
        player.invulnerable--;
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
    buildMap();
    player.x = PLAYER_START_X;
    player.y = PLAYER_START_Y;
    player.hp = 5;
    player.keys = 0;
    player.invulnerable = 0;
    frameCounter = 0;

    for (u8 i = 0; i < MAX_KEYS; ++i)
    {
        keyTaken[i] = 0;
    }

    placeEnemies();
    state = SCREEN_TITLE;
}

void gameUpdate(void)
{
    u16 pad = JOY_readJoypad(JOY_1);

    if (state == SCREEN_TITLE)
    {
        if (pad & BUTTON_START)
        {
            state = SCREEN_PLAYING;
            gameReset();
            player.x = PLAYER_START_X;
            player.y = PLAYER_START_Y;
            buildMap();
        }
        return;
    }

    if (state == SCREEN_WIN || state == SCREEN_LOSE)
    {
        if (pad & BUTTON_START)
        {
            gameReset();
            state = SCREEN_PLAYING;
        }
        return;
    }

    frameCounter++;
    resetPlayerInvulnerability();

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

static void drawTileMap(void)
{
    char line[MAP_W + 1];

    for (s16 y = 0; y < MAP_H; ++y)
    {
        for (s16 x = 0; x < MAP_W; ++x)
        {
            char tile = map[y][x];

            if (player.x == x && player.y == y)
            {
                tile = 'P';
            }
            else if (x == EXIT_X && y == EXIT_Y)
            {
                tile = 'E';
            }
            else if (isEnemyAt(x, y))
            {
                tile = 'M';
            }
            else if (isKeyAt(x, y, 0) || isKeyAt(x, y, 1) || isKeyAt(x, y, 2))
            {
                tile = 'K';
            }

            line[x] = tile;
        }

        line[MAP_W] = '\0';
        VDP_drawText(line, 2, 2 + y);
    }
}

void gameRender(void)
{
    VDP_clearTextArea(BG_A, 0, 0, 40, 28);

    if (state == SCREEN_TITLE)
    {
        VDP_drawText(TITLE_LINE_1, 7, 4);
        VDP_drawText(TITLE_LINE_2, 10, 8);
        VDP_drawText(TITLE_LINE_3, 5, 10);
        VDP_drawText("A hospital has gone silent.", 4, 12);
        VDP_drawText("The keys are hidden in the dark.", 4, 13);
        return;
    }

    drawTileMap();

    if (state == SCREEN_PLAYING)
    {
        char hud[32];
        sprintf(hud, "HP:%u  KEYS:%u/3", player.hp, player.keys);
        VDP_drawText(hud, 2, 1);
        VDP_drawText("GOAL: GET 3 KEYS + ESCAPE", 2, 17);
        return;
    }

    if (state == SCREEN_WIN)
    {
        VDP_drawText("YOU ESCAPED!", 10, 6);
        VDP_drawText("THE HOSPITAL IS STILL THERE.", 2, 8);
        VDP_drawText("PRESS START TO PLAY AGAIN", 4, 11);
    }
    else if (state == SCREEN_LOSE)
    {
        VDP_drawText("THE HOSPITAL WON.", 8, 6);
        VDP_drawText("PRESS START TO TRY AGAIN", 5, 9);
    }
}
