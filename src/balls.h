#ifndef __BALLS_H__
#define __BALLS_H__

#include "defs.h"
#include "draw.h"
#include "physics.h"
#include <SDL/SDL_mixer.h>

void InitBalls(Ball *balls,int mode, int difficulty, defines def);
void ResetBallsKickups(Ball *balls, int difficulty, defines def);
void ResetBallsPong(Ball *balls, int difficulty, defines def);
void ResetBalls(Ball *balls,int mode,int difficulty, defines def);
void UpdateBallsPong(Game * game, Ball *balls,Paddle *paddles, defines def,Mix_Chunk *puk,Mix_Chunk *pluck);
void UpdateBallsKickups(Game * game, Ball *balls,Paddle *paddles, defines def,Mix_Chunk *puk,Mix_Chunk *pluck);
void UpdateBalls(Game * game, Ball *balls,Paddle *paddles, int mode,defines def,Mix_Chunk *puk,Mix_Chunk *pluck);

#endif
