#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "glad.h" // graphics api
#include <GLFW/glfw3.h> // graphics api

#ifndef GAME_TYPES_H
#define GAME_TYPES_H

#define TRUE 1 // boolean value true
#define FALSE 0 // boolean value false

#define ROOM_NUM 4 // a definition for the number of rooms per map
#define SMALL_MAP // definition for size of map

#ifdef SMALL_MAP
    #define MAP_X 8 // number of tiles in the x direction
    #define MAP_Y 8 // number of tiles in the y direction
#else
    #define MAP_X 16
    #define MAP_Y 16
#endif

#define DOF MAP_X
#define MAP_S (MAP_X * MAP_Y) // area of a given room

#define PLAYER_SPEED 35 // the constant in the player move speed
#define SPRITE_SPEED 30 // speed of a sprites
#define VIEWING_ANGLE_CHANGE 5 // just don't touch this
#define PI 3.14159265335
#define P2 PI/2 // pi over 2 
#define P3 3*PI/2 // 3 pi over 2
#define DR 0.0174533 // one degree in radians 
#define MIN_DISTANCE 30 // the minimum distance to a wall

#define STRETCH 8 // the width of each pixel
#define WINDOW_OFFSET 0 // offset for drawing the screen

typedef enum
{ // enum used to set button buffer
    W_DOWN = 0x1,
    A_DOWN = 0x2,
    S_DOWN = 0x4,
    D_DOWN = 0x8,
    E_DOWN = 0x10, 
    COMMA_DOWN = 0x20,
    PERIOD_DOWN = 0x40,
    R_DOWN = 0x80
} ACTION_KEYS;

// ----------- MAPS -------------

typedef struct Map{
    // a map is a doubly linked list that contains an array of level segments, which are 8 x 8 sectors connected by walls of type 5
    struct Map * previous;
    int * m[ROOM_NUM]; // the map of all the walls
    int * map; // pointer to the current walls
    int * c[ROOM_NUM]; // the map of all the ceilings
    int * mapC; // the map of the current ceiling
    int * f[ROOM_NUM]; // the map of all the floors
    int * mapF; // the map of the current floor
    struct Map * next;
} Map;

// --------- PLAYER ------------

typedef struct {
    float plX, plY; // x and y coordinates
    float pdX, pdY, pa; // directions in each axis and an angle for the raycaster
} Player;

// ---------- SPRITE ---------------

typedef struct Sprite {
    // another doubly linked list
    struct Sprite * previous; 
    int type; // type of sprite 
    int state;
    int map;
    float x, y, z;
    int health;
    struct Sprite * next;

} Sprite;

// ------------- MAP FUNCTIONS

void drawMap2D(Map m);
Map *newMap(int m1[MAP_S], int m2[MAP_S], int m3[MAP_S], int m4[MAP_S]);
Map *mapAdd(Map *tail, int m1[MAP_S], int m2[MAP_S], int m3[MAP_S], int m4[MAP_S]);
void mapRemove(Map *item, int * dataCopy[ROOM_NUM]);

// ------------- PLAYER FUNCTIONS

void shootGun(Sprite * s, Player p, int bTravel, int * flashTimer);
void drawBullet();
void bulletScan(Sprite * s, Player p, int bTravel, int * flashTimer);
void movePlayer(Player * p, float deltaTime, Sprite * s, unsigned char * buttonBuffer, Map * m, int bTravel, int * flashTimer);
void drawPlayer(Player p);

// ------------- SPRITE FUNCTIONS

Sprite *newSprite(int state, int type, int map, int health, float x, float y, float z);
Sprite *spriteAdd(Sprite * tail, int state, int type, int map, int health, float x, float y, float z);
void spriteRemove(Sprite * item, Sprite * sCopy);
void drawSpriteMap(Sprite s);
void drawSprite(Sprite * sp, Player p, Map m, int * flashTimer, int depth[120]);
void moveSprite(Sprite * s, float dt, Player p, Map m);

#endif