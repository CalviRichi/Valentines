#include "game_types.h"
#include "/home/calvin/3D_Game/assets/fruity.c"
#include "../assets/heart1.c"
#include "../assets/heart2.c"
#include "../assets/heart3.c"
#include "../assets/heart4.c"
#include "../assets/heart5.c"
#include "../assets/heart6.c"
#include "../assets/heart7.c"
#include "../assets/heart8.c"
#include "../assets/heart9.c"
#include "../assets/heart10.c"
#include "../assets/heart11.c"

Sprite *newSprite(int state, int type, int map, int health, float x, float y, float z) {
    Sprite *spriteHead = (Sprite*)malloc(sizeof(Sprite));
    if (spriteHead == NULL) {
        printf("Malloc failed!\n");
        return NULL;
    }

    spriteHead->previous = NULL;

    spriteHead->state = state;
    spriteHead->type = type;
    spriteHead->map = map;
    spriteHead->health = health;
    spriteHead->x = x;
    spriteHead->y = y;
    spriteHead->z = z;

    spriteHead->next = NULL;

    return spriteHead;
}

Sprite *spriteAdd(Sprite * tail, int state, int type, int map, int health, float x, float y, float z) {
    Sprite * newSprite = (Sprite*)malloc(sizeof(Sprite));

    if (newSprite == NULL) {
        printf("Malloc failed\n");
        return NULL;
    }

    newSprite->previous = tail;

    newSprite->state = state;
    newSprite->type = type;
    newSprite->map = map;
    newSprite->health = health;
    newSprite->x = x;
    newSprite->y = y;
    newSprite->z = z;

    if (tail->next != NULL) {
        tail->next->previous = newSprite;
    }

    tail->next = newSprite;

    return newSprite;

}

void spriteRemove(Sprite * item, Sprite * sCopy) {

    if (&item == NULL || item == NULL) {
        return;
    }

    if (sCopy != NULL) {
        sCopy->health = item->health;
        sCopy->state = item->state;
        sCopy->map = item->map;
        sCopy->type = item->type;
        sCopy->x = item->x;
        sCopy->y = item->y;
        sCopy->z = item->z;
    }

    if (item->next != NULL) {
        item->next->previous = item->previous;
        // this works regardless of previous item status
    }
    if (item->previous != NULL) {
        item->previous->next = item->next;
        // this works regardless of previous item status
    }

    free(item);
    item = NULL;

    return;
}

void drawSpriteMap(Sprite s) {
    
    glPointSize(8);
    glBegin(GL_POINTS);
    glColor3ub(0,255,0);
    glVertex2d(s.x, s.y);
    glEnd();
}

void drawSprite(Sprite * sp, Player p, Map m, int * flashTimer, int depth[120]) {

    float scalingFactorX = 120 / (2 * tan(60*DR / 2));
    float scalingFactorY = 80 / (2 * tan(60*DR / 2));
    
    if (sp->state == 0 || m.map != m.m[0]) {
        if (*flashTimer == 0) {
            //spriteRemove(&sp, NULL); // this causes a crash
            // memory is being freed, but sp is not set to null
            return;
        }
    }
    //printf("flash: %d\n", *flashTimer);
    Sprite s = *sp;

    float spriteX = s.x - p.plX; // Temp variables for sprite coordinates
    float spriteY = s.y - p.plY;
    float spriteZ = s.z;

    float cosAngle = cos(p.pa);
    float sinAngle = sin(p.pa);

    float transformedX = spriteY * cosAngle - spriteX * sinAngle;
    float transformedY = spriteX * cosAngle + spriteY * sinAngle;
    float transformedZ = spriteZ;

    if (transformedY <= 0) return;

    float distanceToSprite = sqrt(spriteX * spriteX + spriteY * spriteY);

    // investigate this part so that sprites display more accurately
    float screenX = (transformedX * scalingFactorX / transformedY) + (120/ 2); // number of rays
    
    float screenY = ((transformedZ * (scalingFactorY/transformedY)) + (80/2))-(distanceToSprite/64); // -distanceToSprite/64
    
    
    if (distanceToSprite > 216) {
        screenY+= (distanceToSprite - 216)/64;
    }
    
    // this is an estimate

    int x, y;
    int scale = 32*80/transformedY;

    if (scale < 0) {
        scale = 0;
    }
    if (scale > 120) {
        scale = 120; // arbitrary
    }

    float t_x = 0, t_y = 31, t_x_step = 31.5/(float)scale, t_y_step = 32.0/(float)scale;

    for (x = screenX - scale/2; x < screenX+scale/2; x++) {
        t_y = 31;
        for (y = 0; y<scale; y++) {

            if (x> 0 && x < 120 && transformedY<depth[x]) {
                // this makes the sprite draw out of bounds but will be fixed when the screen size issue is fixed
                int pixel = ((int)t_y*32+(int)t_x)*3;

                int * t;

                switch (s.map) {
                    case 1:
                        t = T_HEART_1;
                        break;
                    
                    case 2: 
                        t = T_HEART_2;
                        break;
                    case 3:
                        t = T_HEART_3;
                        break;
                    case 4:
                        t = T_HEART_4;
                        break;
                    case 5:
                        t = T_HEART_5;
                        break;
                    case 6: 
                        t = T_HEART_6;
                        break;
                    case 7: 
                        t = T_HEART_7;
                        break;
                    case 8: 
                        t = T_HEART_8;
                        break;
                    case 9: 
                        t = T_HEART_9;
                        break;
                    case 10: 
                        t = T_HEART_10;
                        break;
                    case 11: 
                        t = T_HEART_11;
                        break;
                    case 12: 
                        t = T_HEART_1;
                        break;
                    default:
                        t = T_FRUITY;
                        break;
                }

                int r = t[pixel];
                int g = t[pixel+1];
                int b = t[pixel+2];
                

                if (*flashTimer > 0) {
                    //printf("%d\n",*flashTimer);
                    --*flashTimer;
                    r = 255;
                }
                else {
                    *flashTimer = 0;
                }
                

                if (!(r == 254) && !(g == 254) && !(b == 254)) { // this means don't draw

                glPointSize(STRETCH);
                glColor3ub(r,g,b);
                glBegin(GL_POINTS);
                glVertex2d((x*STRETCH)+WINDOW_OFFSET, screenY*STRETCH-y*STRETCH);
                glEnd();

                }

                t_y -= t_y_step;
                if (t_y < 0) {
                    t_y = 0;
                }
            }
            
        }
        t_x += t_x_step;
        
    }

    
}

// different functions for different sprites
void moveSprite(Sprite * s, float dt, Player p, Map m) {
    
    
    if (s->state == 0 || m.map!=m.m[0]) {
        return;
    }

    if (s->type == 1) { // if it is an enemy

    int spx = ((int)s->x)>>6, spy = ((int)s->y)>>6; // bitshifting right by 6 is the same as dividing by 64
    int spx_add = ((int)s->x+5)>>6, spy_add = ((int)s->y+5)>>6;
    int spx_sub = ((int)s->x-5)>>6, spy_sub = ((int)s->y-5)>>6;
    //printf("hello\n");

    int boost = 1;

    if (m.map[spy*MAP_X+spx_sub] == 0) {
        //noLeft = FALSE;
    }
    else {
        //noLeft = TRUE;
    }


    //if (noLeft) {

    //}
    if (s->x>p.plX && m.map[spy*MAP_X+spx_sub] == 0) {
        s->x-=SPRITE_SPEED*dt;
    }
    if (s->x<p.plX && m.map[spy*MAP_X+spx_add] == 0) {
        s->x+=SPRITE_SPEED*dt;
        //printf("alsoTRUE\n");
        
    }
    if (s->y>p.plY && m.map[spy_sub*MAP_X+spx] == 0) {
        s->y-=SPRITE_SPEED*dt;
        
    }
    if (s->y<p.plY && m.map[spy_add*MAP_X+spx] == 0) {
        s->y+=SPRITE_SPEED*dt;
    }

    return;

    }

    if (s->type == 2) {
        /*
        Code for detecting when the player has intersected with the sprite
        Could include a sound effect
        Need to add +1 to the score 
        */
        float x1 = s->x - p.plX; float y1 = s->y - p.plY;
        float d = x1*x1 + y1*y1;
        if (d < 100) s->state = 0;
        
        return;
    }
}

/*
Should have a situation where
*/