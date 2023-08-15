#include "powerups.h"

void InitPowerups(Powerup *powerups, int mode, defines def) {
	if (!def.GetPowerups()) return;
	ResetPowerups(powerups, mode, def);
}

void ResetPowerups(Powerup *powerups, int mode, defines def) {
	if (!def.GetPowerups()) return;
	int time = SDL_GetTicks();
	//printf ("Tick Tock... %d\n",time);
	time = time + rand()%5000+2500;
	for (int i = 0; i < def.GetMaxPowerups(); i++) {
		powerups[i].SetState(0);
		powerups[i].SetAlive(0);
		//reset somethings
	}
	powerups[0].SetAlive(1);
	powerups[0].SetStartTime(time);
	powerups[0].SetTime(5000);
}

void UpdatePowerups(Game * game, Powerup *powerups,Ball *balls, Paddle *paddles ,int mode,defines def, CSpriteBase * powerup_sprite)
{
	if (!def.GetPowerups()) return;
	int time=SDL_GetTicks();
	//gettime
	for (int i = 0; i < def.GetMaxPowerups(); i++) {
		if (powerups[i].GetState()) {
			if ( (time - powerups[i].GetStartTime()) > powerups[i].GetTime()) {
				KillPowerup(&powerups[i], game->GetScreen());
			}
			else {
				AddRectList(game, powerups[i].GetSDLRect());
				for (int j = 0; j < def.GetMaxBalls(); j++) {
					if ( RectIntersect(balls[j].GetRect(),powerups[i].GetRect()) && balls[j].GetReal()) {
						ActivatePowerUp(&balls[j],&balls[0],&powerups[i],def, game->GetScreen());
						paddles[0].SetUpdate(1);
						ResetPowerups(powerups, 1, def);
					}
				}
			}
		}
		else if (time > powerups[i].GetStartTime() && powerups[i].GetAlive()) {
			WakePowerup(&powerups[i],def, game->GetScreen(),powerup_sprite);
		}
	}

}

void ActivatePowerUp(Ball *ball, Ball *fakeball, Powerup *powerup,defines def, SDL_Surface * screen) {
	switch(powerup->GetState())
	{
		case DoubleSpeed:
			ball->ScaleVelocity(2.0);
			break;
		case HalfSpeed:
			ball->ScaleVelocity(0.5);
			break;
		case IncSpeed:
			ball->ScaleVelocity(1.25);
			break;
		case DecSpeed:
			ball->ScaleVelocity(1/1.25);
			break;
		case ChangePos:
			ChangePosBall(ball,def);
			break;
		case Split:
			SplitBall(ball,fakeball);
			break;
		default: printf("Vini Vidi Vici!\n");
	}
	KillPowerup(powerup, screen);
}

void WakePowerup(Powerup *powerup, defines def, SDL_Surface * screen, CSpriteBase * powerup_sprite)
{
	int state=rand()%def.GetMaxPowerups();
	powerup->init(&powerup_sprite[state],screen);
	powerup->SetState(state);
	powerup->SetSize(50,50);
	powerup->set(rand()%240*2+60,rand()%150*2+60);
}

void KillPowerup(Powerup *powerup, SDL_Surface * screen)
{
	SDL_Rect rect = powerup->GetSDLRect();
	SDL_FillRect(screen,&rect,0x000000);
	SDL_UpdateRects(screen, 1, &rect);
	powerup->SetState(0);
	powerup->SetAlive(0);
}

void SplitBall(Ball *ball, Ball *fakeball) {
	//split the ball using fakeball. graphics etc - mor.
	fakeball->SetVelocity(ball->GetVelocity());
	fakeball->set(ball->GetX(),ball->GetY());
	fakeball->SetShow(1);
	if (rand()%2)
		fakeball->SetVelocity(ball->GetVelocity().x,-ball->GetVelocity().y);
	else
		ball->SetVelocity(ball->GetVelocity().x,-ball->GetVelocity().y);
}

void ChangePosBall(Ball *ball, defines def) {
	int diff;
	diff = min (ball->GetY(), def.GetWindowHeight() - ball->GetY());
	diff = rand()%diff;
	if (rand()%2) diff = -diff;
	ball->set(ball->GetX(),ball->GetY() + diff);
}

