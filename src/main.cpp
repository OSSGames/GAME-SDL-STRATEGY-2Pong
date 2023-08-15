#include <stdio.h>
#include <stdlib.h>
#include <libxml/tree.h>
#include "draw.h"
#include "defs.h"
#include "ai.h"
#include "physics.h"
#include "game.h"
#include "menu.h"
//#include <SDL/SDL_net.h>
#include "math.h"

SDL_Surface *back1, *back2;
CSpriteBase balll,paddlee;


int InitImages()
{
//  back1 = ImageLoad("data/bg1.bmp");
// back2 = ImageLoad("data/bg2.bmp");

	return 0;
}

void FreeStuff(SDL_Surface * screen, Ball *balls, Paddle *paddles,  CSpriteBase * powerup_sprite, Powerup *powerups,defines def) {
	int i;
	for (i = 0; i < def.GetMaxPowerups(); i++)
		powerup_sprite[i].destroy();
	for (i = 0; i < def.GetMaxBalls(); i++)	
		balls[i].destroy();
	for (i = 0; i < def.GetMaxPaddles(); i++)
		paddles[i].destroy();
	SDL_FreeSurface(screen);
}

void FirstInitPowerups(Powerup *powerups, defines def, CSpriteBase * powerup_sprite) {
	/*for (int i = 0; i < def.GetMaxPowerups(); i++) {
		powerup_sprite[i].init("src/data/powerups");
	}*/
	powerup_sprite[0].init("src/data/powerups");
	powerup_sprite[1].init("src/data/powerups/DoubleSpeed");
	powerup_sprite[2].init("src/data/powerups/Split");
	powerup_sprite[3].init("src/data/powerups/HalfSpeed");
	powerup_sprite[4].init("src/data/powerups/IncSpeed");
	powerup_sprite[5].init("src/data/powerups/DecSpeed");
	powerup_sprite[6].init("src/data/powerups/");
}

void FirstInitBalls(Ball *balls, SDL_Surface * screen, defines def) {
	balll.init("src/data/ball");
	for (int i = 0; i < MAX; i++) {
		balls[i].init(&balll,screen);
		balls[i].SetSize(def.GetBallSize());
	}
}

void FirstInitPaddles(Paddle *paddles, SDL_Surface * screen, defines def) {
	paddlee.init("src/data/paddle");
	for (int j = 0; j < def.GetMaxPaddles(); j++) {
		paddles[j].init(&paddlee,screen);
		paddles[j].SetSize(13,80);
	}
	paddles[0].SetType(AI);
	paddles[1].SetType(Human);
	paddles[1].SetControl(Keyboard);
}


void FirstInit(Ball *balls, Paddle *paddles, Powerup *powerups, SDL_Surface * screen, defines def, CSpriteBase * powerup_sprite) {
	InitImages();
	FirstInitBalls(balls,screen, def);
	FirstInitPaddles(paddles,screen, def);
	FirstInitPowerups(powerups,def,powerup_sprite);
}

int main(int argc, char *argv[])
{
	CSpriteBase powerup_sprite[MAX];
	Mix_Chunk *puk, *pluck;
	defines def;
	srand(time(NULL));
	if ( SDL_Init(SDL_INIT_AUDIO|SDL_INIT_VIDEO) < 0 )
	{
		printf("Unable to init SDL: %s\n", SDL_GetError());
	exit(1);
	}
	atexit(SDL_Quit);

	if(Mix_OpenAudio(44100, AUDIO_S16SYS, 2, 2048) < 0)
{
printf("Warning: Couldn't set 44100 Hz 16-bit audio\n- Reason: %s\n",
							SDL_GetError());
}
	puk = Mix_LoadWAV("src/data/sound/puk.wav");
	pluck = Mix_LoadWAV("src/data/sound/pluck.wav");

	Game game;
	game.SetLimit(5);
	SDL_WM_SetIcon(SDL_LoadBMP("icon.bmp"),NULL);
	SDL_WM_SetCaption("2Pong [1.0a]","2Pong [1.0a]");
	SDL_Surface * screen;
//For Linux
//	screen = SDL_SetVideoMode(def.GetWindowWidth(),def.GetWindowHeight(),32,SDL_SWSURFACE);
//For Windows
	screen = SDL_SetVideoMode(def.GetWindowWidth(),def.GetWindowHeight(),32,SDL_HWSURFACE|SDL_DOUBLEBUF);
	if ( screen == NULL )
	{
		printf("Unable to set %dx%d video: %s\n", def.GetWindowWidth(),def.GetWindowHeight(),SDL_GetError());
		exit(1);
	}
	Ball balls[MAX];
	Paddle paddles[MAX];
	Powerup powerups[MAX];
	SDL_ShowCursor(1);
	//InitFonts()
	SDLFont * font1;
	font1 = initFont("src/data/font1");
	game.SetFont(font1);
	game.SetScreen(screen);
	//In the future, dynamic limit.
	FirstInit(balls,paddles,powerups,game.GetScreen(), def,powerup_sprite);
	InitMenu("conf.xml",&game,balls,paddles,powerups,def,powerup_sprite,puk,pluck);

	freeFont(font1);
	Mix_FreeChunk(puk);
	Mix_FreeChunk(pluck);
	FreeStuff(screen,balls,paddles, powerup_sprite,powerups, def);
	return(0);
}
