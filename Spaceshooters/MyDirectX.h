/*
	Beginning Game Programming, Third Edition
	MyDirectX.h
*/
#pragma once

//header files
#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <string>
#include <dinput.h>
#include "DirectSound.h"
#include <sstream>
using namespace std;

#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"dsound.lib")
#pragma comment(lib,"dxerr.lib")
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"legacy_stdio_definitions.lib")

//program values
extern const string APPTITLE;
extern const int SCREENWIDTH;
extern const int SCREENHEIGHT;
extern bool gameover;
extern const bool FULLSCREEN;

//Direct3D objects
extern LPDIRECT3D9 d3d;
extern LPDIRECT3DDEVICE9 d3ddev;
extern LPDIRECT3DSURFACE9 backbuffer;
extern LPD3DXSPRITE sprite_obj;

//primary DirectSound object
extern CSoundManager* dsound;

//game functions
bool Game_Init(HWND window);
void Game_Run(HWND window);
void Game_End();

//Direct3D functions
bool Direct3D_Init(HWND hwnd, int width, int height, bool fullscreen);
void Direct3D_Shutdown();
LPDIRECT3DSURFACE9 LoadSurface(string filename);
void DrawSurface(LPDIRECT3DSURFACE9 dest, float x, float y, LPDIRECT3DSURFACE9 source);
LPDIRECT3DTEXTURE9 LoadTexture(string filename, D3DCOLOR transcolor = D3DCOLOR_XRGB(0, 0, 0));
void Sprite_Draw_Frame(LPDIRECT3DTEXTURE9 texture, int destx, int desty,
	int framenum, int framew, int frameh, int columns);
void Sprite_Animate(int& frame, int startframe, int endframe, int direction, int& starttime, int delay);

void Sprite_Transform_Draw(LPDIRECT3DTEXTURE9 image, int x, int y, int width, int height,
	int frame = 0, int columns = 1, float rotation = 0.0f, float scaling = 1.0f,
	D3DCOLOR color = D3DCOLOR_XRGB(255, 255, 255));

//DirectInput objects, devices, and states
extern LPDIRECTINPUT8 dinput;
extern LPDIRECTINPUTDEVICE8 dimouse;
extern LPDIRECTINPUTDEVICE8 dikeyboard;
extern DIMOUSESTATE mouse_state;


//DirectInput functions
bool DirectInput_Init(HWND);
void DirectInput_Update();
void DirectInput_Shutdown();
int Key_Down(int);
int Mouse_Button(int);
int Mouse_X();
int Mouse_Y();

//function prototypes
bool DirectSound_Init(HWND hwnd);
void DirectSound_Shutdown();
CSound* LoadSound(string filename);
void PlaySound(CSound* sound);
void LoopSound(CSound* sound);
void StopSound(CSound* sound);


//sprite structure



//font functions
LPD3DXFONT MakeFont(string name, int size);
void FontPrint(LPD3DXFONT font, int x, int y, string text, D3DCOLOR color = D3DCOLOR_XRGB(255, 255, 255));






