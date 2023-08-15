#ifndef __DRAW_H__
#define __DRAW_H__

#include "defs.h"

void DrawScene(Game * game, Ball *balls, Paddle *paddles, Powerup *powerups, defines def);
void DrawResults(Game * game, Paddle *paddles, int i, int j);
SDL_Surface * ImageLoad(char *file);
void DrawIMG(SDL_Surface *screen, SDL_Surface *img, int x, int y);
void DrawBG(SDL_Surface *screen, SDL_Surface *back);
void ResetRectList(Game * game);
void AddRectList(Game * game, SDL_Rect rect);
void InitBackground(SDL_Surface * screen);

#endif
