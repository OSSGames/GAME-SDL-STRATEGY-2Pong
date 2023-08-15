#include "paddles.h"

void InitPaddles(Paddle *paddles, Ball *balls,int mode, defines def)
{
	for (int j = 0; j < def.GetMaxPaddles(); j++)
	{
		paddles[j].SetPoints(0);
	}
	ResetPaddles(paddles,balls,mode,def);
}

void ResetPaddles(Paddle *paddles, Ball *balls, int mode, defines def) {
	rectangle r1={{(def.GetWindowWidth() - 20)+1,0},{def.GetWindowWidth(),def.GetWindowHeight()}};
	rectangle r2={{0,0},{20-1,def.GetWindowHeight()}};
	paddles[0].set(20,200);
	paddles[1].SetSize(13,80);
	paddles[0].SetSize(13,80);
	paddles[1].set((def.GetWindowWidth()-(int)paddles[1].GetSize().x) - 20,200);
	paddles[0].SetVelocity(0,2);
	paddles[0].SetAccel(0,0);
	paddles[1].SetAccel(0,0);
	paddles[0].SetBound(r1);
	paddles[1].SetBound(r2);
	paddles[0].SetUpdate(1);
	UpdateAI(&paddles[0],balls,1,def);
	if (mode==2) {
		paddles[1].Rotate();
		paddles[1].set(220,440);
	}
}

void UpdatePaddles(Game * game, Paddle *paddles, Ball *balls, int difficulty, int mode,defines def) {
	if (mode == 1) UpdatePaddlesPong(game,paddles,balls,difficulty,def);
	else if (mode == 2) UpdatePaddlesKickups(game,paddles,balls,difficulty,def);
}

void UpdatePaddlesPong(Game * game, Paddle *paddles, Ball *balls, int difficulty, defines def) {
	for (int i = 0; i < def.GetMaxPaddles(); i++) {
		AddRectList(game, paddles[i].GetSDLRect());
		if (paddles[i].GetType() != Net) {
			paddles[i].SetVelocity(0,paddles[i].GetVelocity().y + paddles[i].GetAccel().y);
		paddles[i].yadd((int)paddles[i].GetVelocity().y);
		paddles[i].xadd((int)paddles[i].GetVelocity().x); }
		CheckBounds(&paddles[i],def.UpperBound());
		CheckBounds(&paddles[i],def.LowerBound());
		if (paddles[i].GetType() == AI) UpdateAI(&paddles[i],balls, difficulty,def);
		AddRectList(game, paddles[i].GetSDLRect());
	}
}

void UpdatePaddlesKickups(Game * game, Paddle *paddles, Ball *balls, int difficulty, defines def) {
	int i=1;
	AddRectList(game, paddles[i].GetSDLRect());
	paddles[i].yadd((int)paddles[i].GetVelocity().y);
	paddles[i].xadd((int)paddles[i].GetVelocity().x);
	CheckBounds(&paddles[i],def.LeftBound());
	CheckBounds(&paddles[i],def.RightBound());
	AddRectList(game, paddles[i].GetSDLRect());
}
