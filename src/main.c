#include "game_types.h" // this includes stdio and stdlib
#include "../assets/gun2.c"
#include "../assets/grass.c"
#include "../assets/bricks.c"
#include "../assets/malane2.c"
#include "../assets/leigha.c"
#include "../assets/alice.c"
#include "../assets/lava.c"
#include "../assets/MinecraftBricks.c"
#include "../assets/grass_2.c"
#include "../assets/wood_planks.c"
// put all these textures into one C file

//#define DEBUG

#ifdef DEBUG
static char db = TRUE;
#else
static char db = FALSE;
#endif

typedef enum {
    TITLE_SCREEN = 0,
    PLAYING_GAME,
    END_SCREEN
} State; static State gamestate = PLAYING_GAME;

static enum
{ // enum used in the handle keys function
    W = 87,
    A = 65,
    S = 83,
    D = 68,
    E = 69,
    COMMA = 44,
    PERIOD = 46,
    R = 82

} INPUT_KEYS;

static unsigned char buttonBuffer = 0x0;
// it doesn't matter if it's signed or not but I think it's funny how long the declaration is

static int flashTimer = 0; // this will be kept in main
static int bTravel; // set during the ray cast, vector for bullet travel
static char bulletTravel = FALSE; // this could be replaced with flashtimer

static int depth[120];

//Sprite * entities[4] = {NULL};

 // some map tile that is not used
int map_walls_big[] = {
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
    1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
    1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
    1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
    1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
    1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
    1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
    1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
    1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
    1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
    1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
    1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
    1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
    1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1
};

int map_floor_big[] = {
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1
};

int map_ceiling_big[] = {
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1
};

int map_floor_small[] = { // for the floors
    1,1,1,1,1,1,1,1,
    1,2,2,2,2,2,2,1,
    1,2,2,2,2,2,2,1,
    1,2,2,2,2,2,2,1,
    1,2,2,2,2,2,2,1,
    1,2,2,2,2,2,2,1,
    1,2,2,2,2,2,2,1,
    1,1,1,1,1,1,1,1
};

int map_ceiling_small[] = { // for the ceiling, perimeter is irrelevent 
    1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1
};
 // these are just random right now
int map_walls_small[] = { // for the walls
    // open walls don't work, but that's not the end of the world
    1,1,1,1,1,5,1,1,
    1,0,0,0,1,0,0,1,
    1,0,0,0,1,0,0,1,
    1,0,0,0,4,0,0,1,
    1,1,1,1,1,0,0,1,
    1,0,0,0,0,0,1,1,
    1,0,0,0,0,0,0,5,
    1,1,1,1,1,1,1,1     
};

int map_walls_small_2[] = {
    1,1,1,5,1,1,1,1,
    1,0,0,0,0,0,0,1,
    1,0,0,0,1,0,0,1,
    1,1,1,1,1,0,0,1,
    1,0,0,0,1,0,0,1,
    1,0,0,0,0,0,0,1,
    5,0,0,0,0,0,0,1,
    1,1,1,1,1,1,1,1
};

int map_walls_small_3[] = {
    1,1,1,1,1,1,1,1,
    1,0,0,0,1,0,0,1,
    5,0,0,0,1,0,0,1,
    1,1,1,0,0,0,0,1,
    1,0,0,0,0,0,0,1,
    1,0,1,0,0,0,1,1,
    1,0,1,0,0,0,0,1,
    1,1,1,5,1,1,1,1
};

int map_walls_small_4[] = {
    1,1,1,1,1,1,1,1,
    1,0,0,0,0,0,0,1,
    1,0,0,0,0,0,0,5,
    1,1,1,1,1,0,0,1,
    1,0,0,0,1,0,0,1,
    1,0,0,0,1,0,0,1,
    1,0,0,0,0,0,0,1,
    1,1,1,1,1,5,1,1
};

float dist(float ax, float ay, float bx, float by, float ang) {
    return (sqrt((bx - ax) * (bx-ax) + (by - ay) * (by - ay)) );
}

void drawGun() {
    int clmit = 181;
    for (int y = 0; y<32; y++) {
        for (int x = 0; x < 32; x++) {
            int pixel = (32*y*3)+(x*3);
            int r = T_GUN[pixel];
            int g = T_GUN[pixel+1];
            int b = T_GUN[pixel+2];

            glPointSize(8);
            glColor3ub(r, g, b);
            glBegin(GL_POINTS);
            if (!(r > clmit && g > clmit && b > clmit)) {
                //printf("r: %d\ng: %d\nb: %d\n", r, g, b);
                glVertex2i(350+x*8, 300+y*8);
            }
            //glColor3ub(255,0,255);
            //glVertex2d(512,324);
            glEnd();
        }
    }

}

// this code is fun and easy to follow
void drawRays3D(Player p, Map map) {

    int r, mx, my, mp, dof;
    // loop counter, 
    float rx, ry, ra, xo, yo;
    float distT = 1000000;
    // rx is the rays x pos
    // xo  and yo are offset

    ra=p.pa-(30*DR);   // change back to 30
    
    if (ra <= 0) {
            ra+=2*PI;
    }
    if (ra > 2*PI) {
        ra-= 2*PI;
    }
   // printf("ra = %f\n",ra);
    // the ray angle will fan out from 30 degrees below player to
    // 30 degrees past player angle 
    if (ra < 0) {
        ra+=2*PI;
    }
    if (ra > 2*PI) {
        ra-= 2*PI;
    }


    for (r = 0; r < 120; ++r) { // number of rays drawn (
        
        int vmt=0,hmt=0; // verticle and horizonal map texture numbers
        int cmt = 0, fmt = 0;


        // check horizontal lines
        dof = 0; // number of times we check for a surface
        float distH = 100000, hx = p.plX, hy = p.plY;
        
  

        float aTan = -1.0/tan(ra); // negative inverse of tangent for the current ray angle 
        
        if (ra > PI && ra < 2*PI) { // ray shooting downwards -- used to be ra > PI

            ry = ((((int)p.plY)/MAP_S)*MAP_S)-0.0001; // this was originally smaller, but this value fixed it
            
            //  ry=(((int)py>>6)<<6) -0.0001;
            
            //  yo=-64; 
            //  xo=-yo*Tan;
            // round to the nearest 64th value
            rx = (p.plY - ry) * aTan+p.plX;
            //  rx=(py-ry)*Tan+px
            yo = -MAP_S; // yo and xo are y-offset and x-offset
            xo = -yo * aTan;
        }
        else if (ra < PI && ra > 0) { // looking up     
            ry = ((((int)p.plY)/MAP_S)*MAP_S)+MAP_S; 
            //ry=(((int)py>>6)<<6)+64;
            rx = (p.plY - ry) * aTan+p.plX;
            //rx=(py-ry)*Tan+px; 
            yo = MAP_S; // yo and xo are y-offset and x-offset
            xo = -yo * aTan;
        }
        else { // looking straight left or right
            rx = p.plX; 
            ry = p.plY;
            dof = DOF;
        }
        
        // this is still for the horizontal lines
        while (dof < DOF) {
            mx = ((int)rx)/MAP_S; 
            my = ((int)ry)/MAP_S; 
            mp = (my*MAP_X)+mx; // my*mapX will set your row, mx will move a number into the row
            
            if (mp >= 0 && mp<MAP_X*MAP_Y && map.map[mp] > 0) { // add in a "mp > 0"
            
            // if the ray's coordinates are inside of a wall
               
                hmt=map.map[mp];
                
                dof = DOF; // hit wall

                hx = rx; 
                hy = ry; 
                distH = dist(p.plX, p.plY, hx, hy, ra);                 
            }
            else {
                // scan for the next sector
                rx += xo;
                ry += yo;
                dof+=1;
            }
        }
        
        /*
        
        ray drawing 

        glColor3f(0,1,0);
        glLineWidth(1);
        glBegin(GL_LINES);
        glVertex2d(plX, plY);
        glVertex2d(rx, ry);
        glEnd(); 
        */
        // for drawing both lines

        // end check horiz


        // check verticle lines
        dof = 0;
        // int side = 0;
        float distV = 100000, vx = p.plX, vy = p.plY; 
        float Tan = -tan(ra);
        if (ra > P2 && ra < P3) { // looking left -- ra > P2 && ra < P3)
            rx = (((int)p.plX/MAP_S)*MAP_S)-0.0001;
            ry = (p.plX - rx) * Tan+p.plY;
            xo = -MAP_S; // yo and xo are y-offset and x-offset
            yo = -xo * Tan;
        }
        else if (ra < P2 || ra > P3) { // looking right
        
            rx = (((int)p.plX/MAP_S)*MAP_S)+MAP_S; 
            
            ry = (p.plX - rx) * Tan+p.plY;
            xo = MAP_S; // yo and xo are y-offset and x-offset
            yo = -xo * Tan;
        }
        else { // looking straight up or down
            ry = p.plY; 
            rx = p.plX;
            dof = DOF;
        }
        while (dof < DOF) {
            mx = (int)(rx)/MAP_S;
            my = (int)(ry)/MAP_S;
            mp = my*MAP_X+mx;
            //mx=(int)(rx)>>6; my=(int)(ry)>>6; mp=my*mapX+mx;
            if (mp >= 0 && mp<MAP_X*MAP_Y && map.map[mp]>0) {
                // needs to be within the bounds of map
                vmt=map.map[mp];
                
                // could add some debugging here
                //printf("rx: %f\nry: %f\nmp: %d\n", rx, ry, mp);
                dof = DOF; // hit wall
            
                vx = rx; 
                vy = ry;

                distV = dist(p.plX, p.plY, vx, vy, ra); // original
                //distV=cos(ra)*(rx-plX)-sin(ra)*(ry-plY);
            }
            else {
                rx += xo;
                ry += yo;
                dof+=1;
            }
        }
        char drawColor;
        if (distV < distH) {
            rx = vx;
            ry = vy;
            hmt = vmt;
             // just to reuse a variable
            distT = distV;
            drawColor = 1;
            glColor3f(1,0,0);
            
        }
        else if (distH < distV) {
            rx = hx;
            ry = hy;
            
            distT = distH;
            drawColor = 0;
            glColor3f(0.8,0,0);
        }
        //printf("hmt: %d\n", hmt);
        int * tile;
        switch (hmt) {
            
            case 1:
                tile = T_BRICKS_2;
                break;
            case 2:
                tile = T_MALANE2;
                break;
            case 5:
                tile = T_LEIGHA;
                break;
            default:
                tile = T_MALANE2;
                break;
        }

        //printf("mp: %d\n", mp);

        // color thing used to be here
        
        
        /*
        glLineWidth(1);
        glBegin(GL_LINES);
        glVertex2d(plX, plY);
        glVertex2d(rx, ry);
        glEnd(); 
        */
        
        // end check vert

        // draw 3d component 

        float ca = p.pa - ra;
        
        // player angle minus ray angle

        if (ca < 0) {
            ca+=2*PI;
        }
        if (ca > 2*PI) {
            ca-= 2*PI;
        }

        distT = distT * cos(ca); // fix the fisheye effect
        // as ray angle moves away from player viewing angle, shorten
        // the lines to not create a fisheye
        
        if (r == 60) {
            bTravel = (int)distT;
            //printf("travel: %d\n", bTravel);
        }

        int lineH, lineO;

        // if this breaks, get rid of the conditional

        lineH = (MAP_S * 512) / distT; // lines get taller the closer you are. 320 keeps everything cubic
        float ty_step = 32.0/(float)lineH;
        float ty_off = 0;
        if (lineH > 512) {
            ty_off = (lineH-512)/2;
            lineH = 512;
        }

        lineO = 256 - lineH/2; // this fixes their position on the screen
        if (lineO < 0) {
            lineO = 0;
        }

        depth[r] = distT;
       // printf("distT: %f\nLineH: %d\nLineO: %d\n", distT, lineH, lineO);
        
    
        // do color change before drawing main lines

        if (drawColor) {
            glColor3f(0.9, 0, 0);
        }
        else {
            glColor3f(0.7, 0, 0);
        }

        /*
        glLineWidth(7);
        glBegin(GL_LINES);
        glVertex2i(r*7+530,lineO); // 530 is to offset it onto the other side of the screen
        glVertex2i(r*7+530,lineH+lineO); 
        glEnd();
        */

        int y;
        // fix something here
        float ty = ty_off*ty_step; // texture's y values
        float tx;
        if(!drawColor) {
        tx=(int)(rx/2.0)%32;
        if(!(ra>PI)) {
            tx = 31 - tx;
        }
        }
        else {
        tx=(int)(ry/2.0)%32;
        if(ra>P2 && ra<P3) {
            tx=31-tx;
        }
        }
        
        // walls

        for (y = 0; y<lineH; y++) {

            int pixel = ((int)ty*32+(int)tx)*3;
            int red = tile[pixel];// swap to T_GRASS if you want 
            int green = tile[pixel+1];
            int blue = tile[pixel+2];
            glPointSize(STRETCH);
            glColor3ub(red, green, blue);
            glBegin(GL_POINTS);
            glVertex2i(r*STRETCH+WINDOW_OFFSET, y+lineO);
            glEnd();
        
            ty+=ty_step;
        }

        // floor
        
        for (int x = lineO+lineH; x < 512; x++) {

            float dx = x - (512/2.0); // 512 is equal to screen height 
            // ca is equal to player angle minus ray angle 
            // ra is ray angle
            // plX is player X pos
            // plY is player y pos
            tx = p.plX/2 + cos(ra)*255*32/dx/cos(ca); // 255
           
            ty = p.plY/2 + sin(ra)*255*32/dx/cos(ca);
            mp = ((int)(ty / 32.0) * MAP_X + (int)(tx / 32.0));
            //printf("tx: %f\nty: %f\n", tx, ty);
            int pixel = (((int)(ty)&31)*32 + ((int)(tx)&31))*3;
            /*
            eventually use a floor and ceiling array + number checker
            to set different tiles and such
            */
            //int mf = mapF[(int)(ty/32.0)*mapX+(int)(tx/32.0)]*32*32;
            //int mf = mapF[mp];
            if (mp >= 0 && mp <= MAP_S) {
                fmt = map.mapF[mp];
            }
            int * floorTile;
            //fmt = hmt;
            switch (fmt) {
                case 1:
                floorTile = T_LEIGHA;
                break;

                case 2:
                floorTile = T_GRASS_2;
                break;

                default:
                floorTile = T_MALANE2;
                break;
            }

            //int mc = mapC[(int)(ty/32.0)*mapX+(int)(tx/32.0)]*32*32;
            //int mc = mapC[mp];
            if (mp >= 0 && mp <= MAP_S) {
                cmt = map.mapC[mp];
            }
            int * ceilingTile;
            //cmt = hmt;
            switch (cmt) {
                case 1:
                    ceilingTile = T_WOOD_PLANKS;
                break;
                case 2:
                    ceilingTile = T_MALANE2;
                break;
                default:
                    ceilingTile = T_BRICKS_2;
                break;
            }
            //printf("cmt: %d\nfmt: %d\n", cmt, fmt);
            
            
            int red = floorTile[pixel];
            int green = floorTile[pixel+1];
            int blue = floorTile[pixel+2];
            glPointSize(STRETCH);
            glColor3ub(red, green, blue);
            glBegin(GL_POINTS);
            glVertex2i(r*STRETCH+WINDOW_OFFSET, x);
            glEnd();
            
            // ceilings

            int pixelC = (((int)(ty)&31)*32 + ((int)(tx)&31))*3;
            unsigned int redC = ceilingTile[pixelC];
            unsigned int greenC = ceilingTile[pixelC+1];
            unsigned int blueC = ceilingTile[pixelC+2];
            glPointSize(STRETCH);
            glColor3ub(redC, greenC, blueC);
            glBegin(GL_POINTS);
            glVertex2i(r*STRETCH+WINDOW_OFFSET, 512-x);
            glEnd();
            
            //printf("floor blue: %d\nceiling blue: %d\n", green, greenC);
            
            
        }

        ra += (DR/2.0); // add one degree to the ray angle 
        if (ra <= 0) {
            ra+=2*PI;
        }
        if (ra > 2*PI) {
            ra-= 2*PI;
        }

    }
}

void handleKeys(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS)
    {
        // printf("Keypress: %d\n", key);

        switch (key)
        {
        case GLFW_KEY_ESCAPE:
            glfwSetWindowShouldClose(window, GLFW_TRUE);
            break;    
        case W:
            buttonBuffer ^= W_DOWN;
            break;
        case A:
            buttonBuffer ^= A_DOWN;
            break;
        case S:
            buttonBuffer ^= S_DOWN;
            break;
        case D:
            buttonBuffer ^= D_DOWN;
            break;
        case E:
            buttonBuffer ^= E_DOWN;
            break;
        case R:
            buttonBuffer ^= R_DOWN;
            break;
        case COMMA:
            buttonBuffer ^= COMMA_DOWN;
            break;
        case PERIOD:
            buttonBuffer ^= PERIOD_DOWN;
            break;
        default:
            break;
        }
    }
    else if (action == GLFW_RELEASE)
    {
        // printf("Key release: %d\n", key);
        switch (key)
        {
        case W:
            buttonBuffer ^= W_DOWN;
            break;
        case A:
            buttonBuffer ^= A_DOWN;
            break;
        case S:
            buttonBuffer ^= S_DOWN;
            break;
        case D:
            buttonBuffer ^= D_DOWN;
            break;
        case E:
            buttonBuffer ^= E_DOWN;
            break;
        case PERIOD:
            buttonBuffer ^= PERIOD_DOWN;
            break;
        default:
            break;
        }
    }
}

void levelInit(Sprite ** s) {
    *s = newSprite(1,1,1,3,96,96,30);
    //printf("test\n");
}
/*
This requires some refactoring, ideally a map could contain a list of the sprites that will exist per level
*/

int main()
{
    // START INITIALIZATION ------------------------------------

    GLFWwindow *window;

    if (!glfwInit())
    {
        printf("OpenGL initialization failed\n");
        glfwTerminate(); // I don't know if this function can be called here, but I can always remove it
        return -1;
    }

    //glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE); // you can't resize the window
    window = glfwCreateWindow(960, 512, "Fruity Game", NULL, NULL); // name the window anything applicable 
    if (window == NULL)
    {
        printf("Could not create window\n");
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, handleKeys);
    

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {   
        printf("Couldn't load opengl\n");
        glfwTerminate();
        return -1;
    }

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f); // default screen color
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, 960, 512, 0, -1, 1);
    glMatrixMode(GL_MODELVIEW);

    // END INITIALIZATION ------------------------------------------------------

    Sprite * headSprite;
    levelInit(&headSprite);
    // While it's annoying to have such a long function header, this allows for dynamic creation of sprites
    Map * headMap = newMap(map_walls_small, map_walls_small_2, map_walls_small_3, map_walls_small_4);

    for (int i = 0; i < ROOM_NUM; i++) {
        headMap->c[i] = map_ceiling_small;
        headMap->f[i] = map_floor_small;
    }
    headMap->mapC = headMap->c[0];
    headMap->mapF = headMap->f[0];
    // this could be its own function

    Player player = {};
    player.plX = 100;
    player.plY = 400;
    player.pdX = cos(0.0001)*VIEWING_ANGLE_CHANGE; 
    player.pdY = sin(0.0001)*VIEWING_ANGLE_CHANGE;

    double lastTime = glfwGetTime();
    double currentTime;
    double deltaTime;

    Sprite * hS; // copies of the head for each list
    Map * hM;

    //Sprite * hT = headSprite; // these will be copies of the tail for use in functions that create enemies
    //Map * hT = headMap; 

    // GAME LOOP

    while (!glfwWindowShouldClose(window))
    { // main loop
        currentTime = glfwGetTime();
        deltaTime = (float)(currentTime - lastTime);

        //printf("dt: %f\n", deltaTime);


        switch (gamestate) {

            case TITLE_SCREEN:

                break;

            case PLAYING_GAME:

                hS = headSprite;
                hM = headMap;
                //taking input
                glfwPollEvents();
                movePlayer(&player, deltaTime, headSprite, &buttonBuffer, hM, bTravel, &flashTimer);
                hS = headSprite;
                while (hS != NULL) {
                    
                    moveSprite(hS, deltaTime, player, *hM);
                    // it should be something like shootgun takes in the sprite and 
                    // runs a loop for each of the sprites, regardless of if they're NULL
                    // and bulletScan simply fires into the wall if so
                    hS = hS->next;    
                }
                

                hS = headSprite;
                hM = headMap;
                bulletTravel = FALSE;
                

                //rendering 
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);      
                
                drawRays3D(player, *hM);

                while (hS != NULL) {
                    drawSprite(hS, player, *hM, &flashTimer, depth);   
                    hS = hS->next;
                }

                hS = headSprite;
                hM = headMap;

                if (db) {

                    drawMap2D(*hM); // gonna take in the current map
                
                    drawPlayer(player); 
                    drawBullet();

                    while (hS != NULL) {
                        drawSpriteMap(*hS);
                        hS = hS->next;
                    }

                    hS = headSprite;
                    hM = headMap;
                
                }
                else {
                    // if we are not in debug mode, clear the memory
                    /*
                    while(hS != NULL) { // free all sprites in state 0
                    
                    if (hS->state == 0) {
                        //printf("Hi\n");
                        if (hS == headSprite) {
                            headSprite = headSprite->next; // only modify the head when memory is being managed
                        }
                        spriteRemove(hS, NULL);
                        hS = headSprite;
                    }
                    
                    else {
                        hS = hS->next;
                    }
                    
                    }
                    */
                }

                drawGun();
                glfwSwapBuffers(window); 

                break;

            case END_SCREEN:

                break;
        }

        lastTime = currentTime;
        
    }

    glfwTerminate(); // get rid of the window at the end

    return 0;
}
