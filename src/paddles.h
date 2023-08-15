#ifndef __PADDLES_H__
#define __PADDLES_H__

#include "defs.h"
#include "draw.h"
#include "physics.h"
#include "ai.h"

void InitPaddles(Paddle *paddles, Ball *balls,int mode, defines def);
void ResetPaddles(Paddle *paddles, Ball *balls, int mode, defines def);
void UpdatePaddles(Game * game, Paddle *paddles, Ball *balls, int difficulty, int mode,defines def);
void UpdatePaddlesPong(Game * game, Paddle *paddles, Ball *balls, int difficulty, defines def);
void UpdatePaddlesKickups(Game * game, Paddle *paddles, Ball *balls, int difficulty, defines def);

#endif
