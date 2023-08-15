#ifndef __POWERUPS_H__
#define __POWERUPS_H__

#include "defs.h"
#include "draw.h"
#include "physics.h"

void InitPowerups(Powerup *powerups, int mode, defines def);
void ResetPowerups(Powerup *powerups, int mode, defines def);
void UpdatePowerups(Game * game, Powerup *powerups,Ball *balls, Paddle *paddles ,int mode, defines def,CSpriteBase * powerup_sprite);
void ActivatePowerUp(Ball *ball, Ball *fakeball, Powerup *powerup,defines def, SDL_Surface * screen);
void WakePowerup(Powerup *powerup, defines def, SDL_Surface * screen,CSpriteBase * powerup_sprite);
void KillPowerup(Powerup *powerup, SDL_Surface * screen);
void SplitBall(Ball *ball, Ball *fakeball);
void ChangePosBall(Ball *ball, defines def);

#endif
