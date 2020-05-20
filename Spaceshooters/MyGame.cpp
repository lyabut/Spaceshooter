#include "MyDirectX.h"


/*
SPACESHOOTER GAME
CREATED BY LESTER YABUT
FOR CSCI 3317 51 COMPUTER GAME PROGRAMMING CLASS
*/



const string APPTITLE = "Spaceshooter";
const int SCREENWIDTH = 1024;
const int SCREENHEIGHT = 768;
const int ASTEROID_NUMBER = 51;
const int BULLET_NUMBER = 50;
const int INVADER_NUMBER = 3;
const int ENEMYBULLET_NUMBER = 4;

//background image
LPDIRECT3DSURFACE9 back_img;

//sprite images
LPDIRECT3DTEXTURE9 ship_img;
LPDIRECT3DTEXTURE9 enemybullet_image;
LPDIRECT3DTEXTURE9 bullet_image;
LPDIRECT3DTEXTURE9 invader_image;
LPDIRECT3DTEXTURE9 asteroid_img;
LPD3DXFONT font;

//buffer window variables
const int bufferHeight = SCREENHEIGHT * 2;
const int bufferWidth = SCREENWIDTH;
double scrollx = 0;
double scrolly = 0;
LPDIRECT3DSURFACE9 background = NULL;

//game world size
const int gameworldSize = bufferHeight * 5;
CSound* sound_hit;
CSound* sound_explode;
CSound* sound_fire;
LPD3DXSPRITE sprite_obj;

int start_bullet;         
int enemybulletcount;
int bulletcount;
int enemyCounter = 1;
int asteroidCounter = 51;
int lives = 10;
double enemystart_bullet;

int enemybulletmovey0 = 12;
int enemybulletmovey1 = 12;
int enemybulletmovey2 = 12;
int enemyfirerate = 700;

//transform variables
D3DCOLOR color = D3DCOLOR_XRGB(255, 255, 255);

struct SPRITE
{
    int health;
    bool shoot;
    int collisionCount;
    bool alive;
    float x, y;
    int frame, columns;
    int width, height;
    float scaling, rotation;
    int startframe, endframe;
    int starttime, delay;
    int direction;
    float movex, movey;
    D3DCOLOR color;

    SPRITE()
    {
        health = 0;
        shoot = true;
        collisionCount = 0;
        movex = movey = 0;
        frame = 0;
        columns = 1;
        width = height = 0;
        scaling = 1.0f;
        rotation = 0.0f;
        startframe = endframe = 0;
        direction = 1;
        starttime = delay = 0;
        movex = movey = 0.0f;
        color = D3DCOLOR_XRGB(255, 255, 255);
    }
};

int Collision(SPRITE sprite1, SPRITE sprite2)
{
    RECT rect1;
    rect1.left = (long)sprite1.x;
    rect1.top = (long)sprite1.y;
    rect1.right = (long)(sprite1.x + sprite1.width * sprite1.scaling);
    rect1.bottom = (long)(sprite1.y + sprite1.height * sprite1.scaling);

    RECT rect2;
    rect2.left = (long)sprite2.x;
    rect2.top = (long)sprite2.y;
    rect2.right = (long)(sprite2.x + sprite2.width * sprite2.scaling);
    rect2.bottom = (long)(sprite2.y + sprite2.height * sprite2.scaling);

    RECT dest; //ignored
    return IntersectRect(&dest, &rect1, &rect2);
}

//bounding box collision detection
int Collision(SPRITE sprite1, SPRITE sprite2);

SPRITE enemybullets[ENEMYBULLET_NUMBER];
SPRITE ship;
SPRITE bullets[BULLET_NUMBER];
SPRITE explode;
SPRITE invader[INVADER_NUMBER];
SPRITE die;
SPRITE asteroids[ASTEROID_NUMBER];

float r = 0;
float scale = 0;
int frame = 0, columns;
int startframe, endframe, starttime = 0, delay;

template <class T>
std::string static ToString(const T & t, int places = 2)
{
    ostringstream oss;
    oss.precision(places);
    oss.setf(ios_base::fixed);
    oss << t;
    return oss.str();
}

bool LoadEnemies()
{

    invader_image = LoadTexture("invader.png");
    if (!invader_image) return false;

    invader[0].health = 100;
    invader[0].scaling = 2;
    invader[0].alive = true;
    invader[0].width = 30;
    invader[0].height = 23;
    invader[0].x = 100; 
    invader[0].y = -100;
    invader[0].movey = 0;
    invader[0].movex = 5;

    invader[1].health = 100;
    invader[1].scaling = 2;
    invader[1].alive = true;
    invader[1].width = 30;
    invader[1].height = 23;
    invader[1].x = 500;
    invader[1].y = -100;
    invader[1].movey = 0;
    invader[1].movex = -5;

    invader[2].health = 100;
    invader[2].scaling = 2;
    invader[2].alive = true;
    invader[2].width = 30;
    invader[2].height = 23;
    invader[2].x = 700; 
    invader[2].y = -100;
    invader[2].movey = 0;
    invader[2].movex = 5;
     
    return true;
}
void DrawEnemies()
{
    for (int n = 0; n < INVADER_NUMBER; n++)
    {

        if (invader[n].alive)
        {

            if (invader[n].x > -96.0 && invader[n].x < SCREENWIDTH)
            {

                Sprite_Transform_Draw(
                    invader_image,
                    (int)invader[n].x,
                    (int)invader[n].y,
                    invader[n].width,
                    invader[n].height,
                    invader[n].frame, invader[n].columns,
                    invader[n].rotation,
                    invader[n].scaling,
                    invader[n].color);
            }
        }

    }
}
int enemybulletexists()
{
    int bullet = -1;
    for (int i = 0; i < ENEMYBULLET_NUMBER; i++)
    {
        if (!enemybullets[i].alive)
        {
            bullet = i;
            break;
        }
    }
    return bullet;
}
void EnemyShoot()
{


    if (GetTickCount() < enemystart_bullet + enemyfirerate) return;
    enemystart_bullet = GetTickCount();
    for (int i = 0; i < INVADER_NUMBER; i++) 
    {
        //fire rate
       
        for (int n = 0; n < ENEMYBULLET_NUMBER; n++)
        {
          
            //invader 1 bullets
            if (invader[0].alive) {
                
                int b1 = enemybulletexists();
                if (b1 == -1) return;
                enemybullets[b1].scaling = invader[0].scaling;
                enemybullets[b1].alive = true;
                enemybullets[b1].rotation = 90.0;
                enemybullets[b1].movex = 0.0;
                enemybullets[b1].movey = enemybulletmovey0;
                enemybullets[b1].x = invader[0].x + (invader[0].width / 2 * invader[0].scaling - 8);
                enemybullets[b1].y = invader[0].y + invader[0].height / 2;


                //deals with random bullets on edge
                if (enemybullets[b1].x < 10)
                    enemybullets[b1].alive = false;
            }
            //invader 2 bullets
            if (invader[1].alive) {
                int b2 = enemybulletexists();
                if (b2 == -1) return;
                enemybullets[b2].scaling = invader[1].scaling;
                enemybullets[b2].alive = true;
                enemybullets[b2].rotation = 90.0;
                enemybullets[b2].movex = 0.0;
                enemybullets[b2].movey = enemybulletmovey1;
                enemybullets[b2].x = invader[1].x + (invader[1].width / 2 * invader[1].scaling -8);
                enemybullets[b2].y = invader[1].y + invader[1].height / 2;


                //deals with
                if (enemybullets[b2].x < 10)
                    enemybullets[b2].alive = false;
            }

            if (invader[2].alive) {
                int b3 = enemybulletexists();
                if (b3 == -1) return;
                enemybullets[b3].scaling = invader[2].scaling;
                enemybullets[b3].alive = true;
                enemybullets[b3].rotation = 90.0;
                enemybullets[b3].movex = 0.0f;
                enemybullets[b3].movey = enemybulletmovey2;
                enemybullets[b3].x = invader[2].x + (invader[2].width / 2 * invader[2].scaling- 5);
                enemybullets[b3].y = invader[2].y + invader[2].height / 2;

                
                //deals with
                if (enemybullets[b3].x < 10)
                    enemybullets[b3].alive = false;

            }
          
        }
       
    }

}
void UpdateEnemies()
{
    for (int n = 0; n < INVADER_NUMBER; n++)
    {
        if (invader[n].alive)
        {
           
            //move horiz based on movex
            invader[n].x += invader[n].movex;
           
            if (invader[0].x > SCREENWIDTH - invader[0].width * invader[0].scaling) 
            {
               
                invader[0].y += 30;
                invader[0].x = SCREENWIDTH - invader[0].width * invader[0].scaling;
                invader[0].movex = -5;
                
                   
            }
               if (invader[0].x < 0) 
            {
                invader[0].y += 30;
                invader[0].x = 0;
                invader[0].movex = 5;
               
            }
               if (invader[1].x > SCREENWIDTH - invader[1].width * invader[1].scaling)
               {
                   invader[1].y += 30;
                   invader[1].x = SCREENWIDTH - invader[1].width * invader[1].scaling;
                   invader[1].movex = -5;

               }
               if (invader[1].x < 0)
               {
                   invader[1].y += 30;
                   invader[1].x = 0;
                   invader[1].movex = 5;
               }

               if (invader[2].x > SCREENWIDTH - invader[0].width * invader[0].scaling)
               {
                   invader[2].y += 30;
                   invader[2].x = SCREENWIDTH - invader[0].width * invader[0].scaling;
                   invader[2].movex = -5;

               }
               if (invader[2].x < 0)
               {
                   invader[2].y += 30;
                   invader[2].x = 0;
                   invader[2].movex = 5;
               }

               if (invader[0].y > SCREENHEIGHT || invader[1].y > SCREENHEIGHT || invader[2].y > SCREENHEIGHT)
               {
                   MessageBox(0, "Game Over", "Game Over", MB_OK);
                   gameover = true;
               }
            //ship touching invader
            if (Collision(ship, invader[n]))
            {
               
                PlaySound(sound_hit);
                lives = 0;
             if (lives <= 0)
              {
                    MessageBox(0, "Game Over", "Game Over", MB_OK);
                    gameover = true;
             }
             
            }
      
        }
    }
}
bool LoadAsteroids()
{
    asteroid_img = LoadTexture("asteroid.png");
    if (!asteroid_img) return false;


    for (int n = 0; n < ASTEROID_NUMBER; n++)
    {
        asteroids[n].scaling = (float)((rand() % 5));
        asteroids[n].alive = true;
        asteroids[n].width = 21;
        asteroids[n].height = 21;
        asteroids[n].x = (float)(rand() % SCREENWIDTH);
        asteroids[n].y = (float)(-1000 + rand() % SCREENHEIGHT);
        asteroids[n].movey = (float)((rand() % 7) * 1);
        asteroids[n].movex = (float)(rand() % 2 - 1);
        asteroids[n].frame = 0;
        asteroids[n].startframe = 0;
        asteroids[n].endframe = 1;
        asteroids[n].starttime = 0;
        asteroids[n].delay = 100;
        asteroids[n].columns = 2;


    }
    return true;
}
void UpdateAsteroids()
{
   
    for (int n = 0; n < ASTEROID_NUMBER; n++)
    {
        if (asteroids[n].alive)
        {
            //move horiz based on movex
            asteroids[n].x += asteroids[n].movex;
            if (asteroids[n].x < -96.0f)
                asteroids[n].x = (float)gameworldSize;  
            if (asteroids[n].x > (float)gameworldSize)
                asteroids[n].x = -96.0f;

            //move vert based on movey
            asteroids[n].y += asteroids[n].movey;
            if (asteroids[n].y < -96.0f)
                asteroids[n].y = SCREENHEIGHT - 20;
            if (asteroids[n].y > SCREENHEIGHT - 20)
                asteroids[n].y = -96.0f;

            //ship touching asteroid
            if (Collision(ship, asteroids[n]))
            {
                asteroids[n].alive = false;
                asteroidCounter--;
                

                PlaySound(sound_hit);
               if (lives <= 0)
                {
                   MessageBox(0, "Game Over", "Game Over", MB_OK);
                   gameover = true;
                }
                lives -= 1;

            }

            //if asteroids are stuck offscreen or right at screenedge move them to another x 
            if (asteroids[n].x > SCREENWIDTH - 50)
            {
                asteroids[n].x = (float)(rand() % SCREENWIDTH);
                asteroids[n].y = (float)(-1000 + rand() % SCREENHEIGHT);
            }
                // asteroids happen to have 0 movement off screen, add movement
             if (asteroids[n].movex == 0|| asteroids[n].movey == 0) {
                asteroids[n].movex = (float)(rand() % 2 - 0.02); asteroids[n].y += asteroids[n].movey + (1+rand()%5);
               
           }
        }
    }
}
void DrawAsteroids()
{
    for (int n = 0; n < ASTEROID_NUMBER; n++)
    {
       
        if (asteroids[n].alive)
        {
           
            if (asteroids[n].x > -96.0 && asteroids[n].x < SCREENWIDTH)
            {
              
                Sprite_Transform_Draw(
                    asteroid_img,
                    (int)asteroids[n].x,
                    (int)asteroids[n].y,
                    asteroids[n].width,
                    asteroids[n].height,
                    asteroids[n].frame, asteroids[n].columns,
                    asteroids[n].rotation,
                    asteroids[n].scaling,
                    asteroids[n].color);
             
            }
        }
        
     
    }
}
bool LoadShip()
    {
        D3DXIMAGE_INFO info;
        HRESULT result;

        //load the tank sprite
        ship_img = LoadTexture("animatedShip.bmp");


        if (ship_img == NULL)
            return false;
        //set the tank's properties
        result = D3DXGetImageInfoFromFile("animatedShip.bmp", &info);
        if (result != D3D_OK)
            return false;

        ship.width = info.Width;
        ship.height = info.Height;
        ship.scaling = 1.5;
        ship.x = 300;
        ship.y = 500;
       

        return true;
}
void UpdateShip()
{
    int pressed = 0;
  
    if (Key_Down(DIK_RIGHT)) {
        pressed = 1;
    }
    if (Key_Down(DIK_UP)) {
        pressed = 2;
    }
    if (Key_Down(DIK_LEFT)) {
        pressed = 3;
    }
    if (Key_Down(DIK_DOWN)) {
        pressed = 4;
    }
    if (Key_Down(DIK_UP) && Key_Down(DIK_RIGHT)) {
        pressed = 5;
    }
    if (Key_Down(DIK_UP) && Key_Down(DIK_LEFT)) {
        pressed = 6;
    }
    if (Key_Down(DIK_DOWN) && Key_Down(DIK_LEFT)) {
        pressed = 7;
    }
    if (Key_Down(DIK_DOWN) && Key_Down(DIK_RIGHT)) {
        pressed = 8;
    }

    switch (pressed) {
        
    default:  //Idle animation 
        sprite_obj->Begin(D3DXSPRITE_ALPHABLEND);

       
        scale = 2.0f;       //scale factor
        columns = 2;        //how many columns for .bmp file
        startframe = 0;     //frame start
        endframe = 1;       //frame end
        delay = 100;        //delay between frame switch
        ship.width = 40;    //single sprite width
        ship.height = 29;   //single sprite height

        Sprite_Animate(frame, startframe, endframe, 1, starttime, delay);

        Sprite_Transform_Draw(
        ship_img,
        ship.x,
        ship.y,
        ship.width,
        ship.height, frame,columns, 0,scale, color);

        sprite_obj->End();
        break;


    case (1): //right arrow pressed

        sprite_obj->Begin(D3DXSPRITE_ALPHABLEND);
        Sprite_Animate(frame, startframe, endframe, 1, starttime, delay);

        Sprite_Transform_Draw(
            ship_img,
            ship.x,
            ship.y,
            ship.width,
            ship.height, frame, columns, 0, scale, color);

        ship.movex = 7;
        ship.movey = 0;
        ship.x += ship.movex; // only move when key pressed
        ship.y += ship.movey;
        sprite_obj->End();
        break;

    case (2): //up arrow pressed
        sprite_obj->Begin(D3DXSPRITE_ALPHABLEND);
        Sprite_Animate(frame, startframe, endframe, 1, starttime, delay);

        Sprite_Transform_Draw(
            ship_img,
            ship.x,
            ship.y,
            ship.width,
            ship.height, frame, columns, 0, scale, color);

        ship.movex = 0;
        ship.movey = -7;
        ship.x += ship.movex; // only move when key pressed
        ship.y += ship.movey;
        sprite_obj->End();
        break;

    case (3): // left arrow pressed
        sprite_obj->Begin(D3DXSPRITE_ALPHABLEND);
        Sprite_Animate(frame, startframe, endframe, 1, starttime, delay);

        Sprite_Transform_Draw(
            ship_img,
            ship.x,
            ship.y,
            ship.width,
            ship.height, frame, columns, 0, scale, color);

        ship.movex = -7;
        ship.movey = 0;
        ship.x += ship.movex; // only move when key pressed
        ship.y += ship.movey;
        sprite_obj->End();
        break;

    case(4): //down arrow pressed

        sprite_obj->Begin(D3DXSPRITE_ALPHABLEND);
        Sprite_Animate(frame, startframe, endframe, 1, starttime, delay);
        Sprite_Transform_Draw(
            ship_img,
            ship.x,
            ship.y,
            ship.width,
            ship.height, frame, columns, 0, scale, color);

        ship.movex = 0;
        ship.movey = 7;
        ship.x += ship.movex; // only move when key pressed
        ship.y += ship.movey;
        sprite_obj->End();
        break;

    case(5):  //up and right arrow pressed
        sprite_obj->Begin(D3DXSPRITE_ALPHABLEND);
        Sprite_Animate(frame, startframe, endframe, 1, starttime, delay);

        Sprite_Transform_Draw(
            ship_img,
            ship.x,
            ship.y,
            ship.width,
            ship.height, frame, columns, 0, scale, color);

        ship.movex = 7;
        ship.movey = -7;
        ship.x += ship.movex; // only move when key pressed
        ship.y += ship.movey;
        sprite_obj->End();
        break;

    case(6): // up and left arrow pressed
        sprite_obj->Begin(D3DXSPRITE_ALPHABLEND);
        Sprite_Animate(frame, startframe, endframe, 1, starttime, delay);

        Sprite_Transform_Draw(
            ship_img,
            ship.x,
            ship.y,
            ship.width,
            ship.height, frame, columns, 0, scale, color);

        ship.movex = -7;
        ship.movey = -7;
        ship.x += ship.movex; // only move when key pressed
        ship.y += ship.movey;
        sprite_obj->End();
        break;


    case(7): // down and left arrow pressed
        sprite_obj->Begin(D3DXSPRITE_ALPHABLEND);
        Sprite_Animate(frame, startframe, endframe, 1, starttime, delay);

        Sprite_Transform_Draw(
            ship_img,
            ship.x,
            ship.y,
            ship.width,
            ship.height, frame, columns, 0, scale, color);

        ship.movex = -7;
        ship.movey = 7;
        ship.x += ship.movex; // only move when key pressed
        ship.y += ship.movey;
        sprite_obj->End();
        break;


    case(8): //down and right arrow pressed
        sprite_obj->Begin(D3DXSPRITE_ALPHABLEND);
        Sprite_Animate(frame, startframe, endframe, 1, starttime, delay);

        Sprite_Transform_Draw(
            ship_img,
            ship.x,
            ship.y,
            ship.width,
            ship.height, frame, columns, 0, scale, color);

        ship.movex = 7;
        ship.movey = 7;
        ship.x += ship.movex; // only move when key pressed
        ship.y += ship.movey;
        sprite_obj->End();
        break;

    }

    if (ship.x < 0)
        ship.x = 0;
    if (ship.x > SCREENWIDTH - ship.width * ship.scaling - 20)
        ship.x = SCREENWIDTH - ship.width * ship.scaling - 20;
    if (ship.y < 0)
        ship.y = 0;
    if (ship.y > SCREENHEIGHT - ship.height - 20)
        ship.y = SCREENHEIGHT - ship.height - 20;
   
}
bool LoadBackground()
{
   
    back_img = LoadSurface("background.png");
    if (!back_img) return false;


    //create background
    HRESULT result =
        d3ddev->CreateOffscreenPlainSurface(
            bufferWidth,
            bufferHeight,
            D3DFMT_X8R8G8B8,
            D3DPOOL_DEFAULT,
            &background,
            NULL);
    if (result != D3D_OK) return false;

    //copy image to top portion of background
    RECT source_rect = { 0, 0, SCREENWIDTH, SCREENHEIGHT };
    RECT dest_top = { 0, 0, SCREENWIDTH, SCREENHEIGHT };
    d3ddev->StretchRect(back_img, &source_rect, background, &dest_top, D3DTEXF_NONE);

    //copy image to bottom portion of background while extending bottom
    RECT dest_bot = { 0, SCREENHEIGHT, SCREENWIDTH, SCREENHEIGHT * 2 };
    d3ddev->StretchRect(back_img, &source_rect, background, &dest_bot, D3DTEXF_NONE);
    d3ddev->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &backbuffer);


    return true;
}
void ScrollBackground()
{
    scrolly -= 1.5;
    if (scrolly < 0)
        scrolly = bufferHeight - SCREENHEIGHT;
    if (scrolly > bufferHeight - SCREENHEIGHT)
        scrolly = 0;
    if (scrollx < 0)
        scrollx = bufferWidth - SCREENWIDTH;
    if (scrollx > bufferWidth - SCREENWIDTH)
        scrollx = 0;
}
void DrawEnemyBullets()
{

    D3DCOLOR bulletcolor = D3DCOLOR_ARGB(255, 255, 255, 255);
    for (int n = 0; n < ENEMYBULLET_NUMBER; n++)
    {
        if (enemybullets[n].alive)
        {
            Sprite_Transform_Draw(
                enemybullet_image,
                (int)enemybullets[n].x,
                (int)enemybullets[n].y,
                enemybullets[n].width,
                enemybullets[n].height,
                0, 1,
                0,
                enemybullets[n].scaling,
                bulletcolor);
        }
    }
}
void Draw_Bullets()
{
    D3DCOLOR bulletcolor = D3DCOLOR_ARGB(255, 255, 255, 255);
    for (int n = 0; n < BULLET_NUMBER; n++)
    {
        if (bullets[n].alive)
        {
            Sprite_Transform_Draw(
                bullet_image,
                (int)bullets[n].x,
                (int)bullets[n].y,
                bullets[n].width,
                bullets[n].height,
                0, 1,
                0,
                2.0f,
                bulletcolor);
        }
    }
}

//making sure bullet is alive
int bulletexists()
{
    int bullet = -1;
    for (int i = 0; i < BULLET_NUMBER; i++)
    {
        if (!bullets[i].alive)
        {
            bullet = i;
            break;
        }
    }
    return bullet;
}
void shoot()
{
    //fire rate
    if (GetTickCount() < start_bullet + 100) return;
     start_bullet = GetTickCount();
    
    //bullet 1
    int b1 = bulletexists();
   if (b1 == -1) return;
    bullets[b1].alive = true;
    bullets[b1].rotation = 90.0;
    bullets[b1].movex = 0.0f;
    bullets[b1].movey = -12.0f;
    bullets[b1].x = (ship.x + ship.width/2) + 43; //50 units to the right of the center of sprite
    bullets[b1].y = ship.y + ship.height/2 - 12;
   

    //bullet 2
    int b2 = bulletexists();
   if (b2 == -1) return;
    bullets[b2].alive = true;
    bullets[b2].rotation = 90.0;
    bullets[b2].movex = 0.0f;
    bullets[b2].movey = -12.0f;
    bullets[b2].x = (ship.x + ship.width / 2) - 18; //17 units to the left of the center of the sprite
    bullets[b2].y = ship.y + ship.height / 2 - 12;
  

}
void Update_Bullets()
{

    //update bullets
    bulletcount = 0;
   

        for (int n = 0; n < BULLET_NUMBER; n++)
        {
            if (bullets[n].alive)
            {
                bulletcount++;
                bullets[n].x += bullets[n].movex;
                bullets[n].y += bullets[n].movey;

                if (bullets[n].y < 40 || bullets[n].y > SCREENHEIGHT)
                    bullets[n].alive = false;
            }
        }
    
}
void UpdateEnemyBullets()
{
    //update enmy bullet
    enemybulletcount = 0;
    for (int n = 0; n < ENEMYBULLET_NUMBER; n++)
    {
        if (enemybullets[n].alive)
        {
            enemybulletcount++;
            enemybullets[n].x += enemybullets[n].movex;
            enemybullets[n].y += enemybullets[n].movey;

            if (enemybullets[n].y < 0 || enemybullets[n].y > SCREENHEIGHT)
                enemybullets[n].alive = false;
        }
    }
}
void BulletCollision()
{
    //Asteroid-Bullet
    for (int v = 0; v < ASTEROID_NUMBER; v++)
    {
        
        if (asteroids[v].alive)
        {
            for (int b = 0; b < BULLET_NUMBER; b++)    
            {
                if (bullets[b].alive)
                {
                    if (Collision(asteroids[v], bullets[b]))
                    {
                        bullets[b].alive = false;
                        asteroids[v].collisionCount++;
                        if (asteroids[v].scaling < 2.5)
                        {
                            bullets[b].alive = false;
                            asteroids[v].alive = false;
                            asteroidCounter--;
                            PlaySound(sound_explode);
                        }
                        else if (asteroids[v].collisionCount == 5)
                        {
                            bullets[b].alive = false;
                            asteroids[v].alive = false;
                            asteroidCounter--;
                            PlaySound(sound_explode);
                        }

                    }
                   
                }
                
              
            }
         }
    }

    //Bullet-Enemy Collision

    for (int v = 0; v < INVADER_NUMBER; v++)
    {
        if (invader[v].alive) {

            for (int b = 0; b < BULLET_NUMBER; b++)
            {
                if (enemybullets[b].alive) {
                    {
                       if (Collision(invader[0], bullets[b]))
                        {
                            bullets[b].alive = false;
                            invader[0].health--;
                            

                            if (invader[0].health <= 0)
                            {
                                invader[0].alive = false;

                            }


                        }

                    }
                   if (Collision(invader[1], bullets[b]))
                    {
                        bullets[b].alive = false;
                        invader[1].health--;

                        if (invader[1].health <= 0)
                        {
                            invader[1].alive = false;
                        }


                    }
                    if (Collision(invader[2], bullets[b]))
                    {
                        bullets[b].alive = false;
                        invader[2].health--;

                        if (invader[2].health <= 0)
                        {
                            invader[2].alive = false;

                        }
                    }
                }
            }
        }

    }

    //EnemyBullet-Ship Collision
    for (int v = 0; v < INVADER_NUMBER; v++)
    {
        if (invader[v].alive)
        {
            
            for (int b = 0; b < ENEMYBULLET_NUMBER; b++)
            {
                if (enemybullets[b].alive)
                {
                    if (Collision(ship, enemybullets[b]))
                    {
                        enemybullets[b].alive = false;
                        lives -= 1;
                        if (lives <= 0)
                        {
                            MessageBox(0, "Game Over", "Game Over", MB_OK);
                            gameover = true;
                        }

                    }
                    if (Collision(ship, invader[v]))
                    {
                        ship.alive = false;
                        lives = 0;
                        MessageBox(0, "Game Over", "Game Over", MB_OK);
                        gameover = true;
                        
                    }
                }
            }
        }
    }
}
    
   
bool Game_Init(HWND hwnd)
{
    Direct3D_Init(hwnd, SCREENWIDTH, SCREENHEIGHT, false);
    DirectInput_Init(hwnd);
    DirectSound_Init(hwnd);
    LoadShip();
    LoadBackground();
    font = MakeFont("Arial Bold", 24);
    
    //Load Bullet
     bullet_image = LoadTexture("beams.png",
         D3DCOLOR_XRGB(255, 0, 255));
     //Load Enemy Bullet
     enemybullet_image = LoadTexture("enemybullet.png");
      
     if (enemybullet_image == NULL) return false;
       for (int n = 0; n < ENEMYBULLET_NUMBER; n++)
       {
           enemybullets[n].alive = false;
           enemybullets[n].x = 0;
           enemybullets[n].y = 0;
           enemybullets[n].width = 8;
           enemybullets[n].height = 8;
       }

     if (bullet_image == NULL)      return false;
    for (int n = 0; n < BULLET_NUMBER; n++)
     {
         bullets[n].alive = false;
         bullets[n].x = 0;
         bullets[n].y = 0;
         bullets[n].width = 7;
         bullets[n].height = 15;
     }
    //load explode wave file
    sound_explode = LoadSound("explode.wav");
    if (sound_explode == NULL)   return 0;

    //load hit wave file
    sound_fire = LoadSound("shoot.wav");
    if (sound_fire == NULL)  return 0;

    //load hit sound
    sound_hit = LoadSound("Hit_03.wav");
   if (sound_hit == NULL)  return 0;

    if (!LoadAsteroids()) return false;
    if (!LoadEnemies()) return false;

    return true;
}

void Game_Run(HWND hwnd)
{
    if (!d3ddev) return;
    DirectInput_Update();
    d3ddev->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
        D3DCOLOR_XRGB(0, 0, 100), 1.0f, 0);

    //rotate half of asteroids counter clockwise
    for (int n = 0; n > 25; n++)
    {
        asteroids[n].rotation +=0.004;
    }
    //rotate half of asteroids clockwise
    for (int n = 0; n < 25; n++)
    {
        asteroids[n].rotation -= 0.004;
    }
    int start = 0;
   

   //update enemies size and fire rate

    if (!invader[2].alive)
    {
        enemyfirerate = 500;
        enemybulletmovey1 = 20;
        enemybulletmovey0 = 20;
        invader[1].scaling = 3;
        invader[0].scaling = 3;
       
       
    }
    if (!invader[1].alive)
    {

        enemyfirerate = 500;
        enemybulletmovey0 = 20;
        enemybulletmovey2 = 20;
        invader[2].scaling = 3;
        invader[0].scaling = 3;
        
    }
    if (!invader[0].alive)
    {
        enemyfirerate = 500;
        enemybulletmovey1 = 20;
        enemybulletmovey2 = 20;
        invader[1].scaling = 3;
        invader[2].scaling = 3;
    }
    //third and second dead, first gets stronger
    if (!invader[2].alive && !invader[1].alive)
    {
        enemyfirerate = 200;
        enemybulletmovey0 = 25;
        invader[0].scaling = 5;
    }

    //third and first dead, second gets stronger
    if (!invader[2].alive && !invader[0].alive)
    {
        enemyfirerate = 200;
        enemybulletmovey1 = 25;
        invader[1].scaling = 5;
      
    }
    //first and second dead, third gets stronger
    if (!invader[1].alive && !invader[0].alive)
    {
        enemyfirerate = 200;
        enemybulletmovey1 = 25;
        invader[2].scaling = 5;
    }

    if (GetTickCount() - start >= 100)
    {
        //reset timing
        start = GetTickCount();
        
        if (Key_Down(DIK_SPACE))
        {
            shoot();
            PlaySound(sound_fire);
        }
        EnemyShoot();
         
        ScrollBackground();
        RECT source_rect = { scrollx, scrolly, scrollx + SCREENWIDTH, scrolly + SCREENHEIGHT };
        RECT dest_rect = { 0, 0, SCREENWIDTH, SCREENHEIGHT };

        d3ddev->StretchRect(background, &source_rect, backbuffer, &dest_rect, D3DTEXF_NONE);
    }

    if (d3ddev->BeginScene())
    {
        sprite_obj->Begin(D3DXSPRITE_ALPHABLEND);

        FontPrint(font, 10, 0, "LIVES: " + ToString(lives), color);

        Draw_Bullets();
        DrawEnemyBullets();
        DrawEnemies();
        DrawAsteroids();
        UpdateShip();

        if (asteroidCounter <= 8)
        {
            UpdateEnemies();
            UpdateEnemyBullets();

            for (int b = 0; b < BULLET_NUMBER; b++)
            {
                 bullets[b].alive = false;
                 bullets[b].alive = true; 
             }  
        }
        Update_Bullets();
        UpdateAsteroids();
        BulletCollision();
        sprite_obj->End();
        d3ddev->EndScene();
    }

    d3ddev->Present(NULL, NULL, NULL, NULL);

}

void Game_End()
{
    if (asteroid_img) asteroid_img->Release();
    if (ship_img) ship_img->Release();
    if (back_img) back_img->Release();
    if (bullet_image) bullet_image->Release();
    if (enemybullet_image) enemybullet_image->Release();
    if (invader_image) invader_image->Release();
    if (sound_explode != NULL)	delete sound_explode;
    if (sound_fire != NULL)	delete sound_fire;
    if (sound_hit != NULL) delete sound_hit;
    DirectSound_Shutdown();
    DirectInput_Shutdown();
    Direct3D_Shutdown();
}