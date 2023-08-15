#ifndef __AI_H__
#define __AI_H__

#include "defs.h"
#include <SDL/SDL.h>
float GetTime(Paddle paddle, Ball ball);
int CloseBall(Paddle paddle, Ball *balls);
void UpdateAI(Paddle *paddle, Ball *balls,int difficulty,defines def);

#endif
