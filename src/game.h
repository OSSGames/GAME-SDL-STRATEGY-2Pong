#ifndef __GAME_H__
#define __GAME_H__

#include "net.h"
#include <SDL/SDL.h>
#include "SDL_mixer.h"
#include "defs.h"
#include "draw.h"
#include "physics.h"
#include "ai.h"
#include "powerups.h"
#include "balls.h"
#include "paddles.h"
int GameMain(Game *game,int init,Ball *balls, Paddle *paddles, Powerup *powerups, defines def,CSpriteBase * powerup_sprite,Mix_Chunk *puk,Mix_Chunk *pluck, IPaddress address, UDPsocket sock, int channel, int net);
void InitGame(Game * game, Ball *balls, Paddle *paddles, Powerup *powerups,int mode,int difficulty,defines def);
int ReachLimit(Game *game, Paddle *paddles);
void ResetGame(Game * game, Ball *balls, Paddle *paddles, Powerup *powerups, int mode, int difficulty, defines def);
int CheckState(Game *game, Ball *balls, Paddle *paddles, defines def);
void UpdateScene(Game * game, Ball *balls, Paddle *paddles, Powerup *powerups, int difficulty, int mode, defines def,CSpriteBase * powerup_sprite,Mix_Chunk *puk,Mix_Chunk *pluck);
void CheckWinLose(Game *game,Ball *ball,Paddle *paddles, defines def);

#endif
