#include "balls.h"

void InitBalls(Ball *balls,int mode, int difficulty, defines def)
{
	// Init two balls as real
	for (int i = 1; i < def.GetMaxBalls(); i++)
		balls[i].SetReal(1);

	// One fake ball. Arbitrary init values
	balls[0].SetReal(0);
	balls[0].SetVelocity(0,0);
	balls[0].set(100,100);

	ResetBalls(balls,mode,difficulty, def);
}

void ResetBalls(Ball *balls, int mode, int difficulty, defines def) {
	if (mode == 1) ResetBallsPong(balls,difficulty,def);
	else if (mode == 2) ResetBallsKickups(balls,difficulty,def);
	else return;
}

void ResetBallsKickups(Ball *balls, int difficulty, defines def) {
	// Our max height (for now)
	int height = 60;
	
	float vel = sqrt(60);
	float gravity = 0.2;
	float velx,vely;
	int distx, disty;

	for (int i = 0; i < def.GetMaxBalls(); i++)
	{
		if (balls[i].GetReal())
		{
			distx = rand()%200+100;
			disty = rand()%150+50;
			balls[i].yset(height + disty);
			if (rand()%2) balls[i].xset(distx);
			else balls[i].xset(def.GetWindowWidth() - distx);
				
			vely = sqrt(2*gravity*disty);
			if (i%2) vely=-vely;
			
			velx = sqrt(sqr(vel) - sqr(vely));
			velx=4;
			if (rand()%2) velx = -velx;
			
			balls[i].SetAccel(0,gravity);
			balls[i].SetVelocity(velx,vely);
			balls[i].SetShow(1);
			balls[i].setSpeed(1);			
		}
		else
			balls[i].SetShow(0);
	}
}

void ResetBallsPong(Ball *balls, int difficulty, defines def) {
	// Our max velocity (in the beginning, anyway)
	float vel = sqrt(60);

	float velx,vely;
	int distx,disty;

	for (int i = 0; i < def.GetMaxBalls(); i++)
	{
			// We don't care about the fake ball here
			if (balls[i].GetReal())
			{
				
				velx = (rand()%150+150)/(rand()%50+1);

				// Don't want it to go too fast on the x-axis
				if (velx < 3) velx=3;
				else if (velx > 7) velx=7;
				
				// Keep our velocity vector
				vely = sqrt(sqr(vel) - sqr(velx));	

				// Random distances from the ends of the screen
				distx = rand()%100+50;
				disty = rand()%300+100;

				// We want the balls to start in opposite directions
				if (i % 2) {
					balls[i].set(distx,disty);
				}
				else {
					balls[i].set(def.GetWindowWidth() - distx,disty);
					velx = -velx;
				}

				// Determine the y velocity direction randomly
				if (rand()%2) vely = -vely;

				// Set everything
				balls[i].SetVelocity(velx,vely);
				balls[i].SetAccel((float)0.1*difficulty,0);
				balls[i].SetShow(1);
				balls[i].setSpeed(4);
			}
			else
				// If it's our fake ball, no need to show it now
				balls[i].SetShow(0);
	}
}
void UpdateBalls(Game * game, Ball *balls,Paddle *paddles, int mode ,defines def,Mix_Chunk *puk,Mix_Chunk *pluck) {
	if (mode == 1) UpdateBallsPong(game,balls,paddles,def,puk,pluck);
	else if (mode == 2) UpdateBallsKickups(game,balls,paddles,def,puk,pluck);
}

void UpdateBallsKickups(Game * game, Ball *balls,Paddle *paddles, defines def,Mix_Chunk *puk,Mix_Chunk *pluck) {
	for (int i = 0; i < def.GetMaxBalls(); i++)
	{
		if (balls[i].GetShow()) {
			AddRectList(game, balls[i].GetSDLRect());
			
			balls[i].xadd((int)balls[i].GetVelocity().x);
			balls[i].yadd((int)balls[i].GetVelocity().y);
			balls[i].SetVelocity(balls[i].GetVelocity().x+balls[i].GetAccel().x,
					balls[i].GetVelocity().y+balls[i].GetAccel().y);
			if (CheckBounds(&balls[i],def.UpperBound())) {
				balls[i].SetVelocity(balls[i].GetVelocity().x,-balls[i].GetVelocity().y);
			}
			else if (CheckBounds(&balls[i],def.LowerBound())) {
				balls[i].SetVelocity(balls[i].GetVelocity().x,-balls[i].GetVelocity().y);
			}
			else if (RectIntersect(balls[i].GetRect(),def.RightBound())) {
				balls[i].xset(def.GetWindowWidth()-5 - 15);
				balls[i].SetVelocity(-balls[i].GetVelocity().x,balls[i].GetVelocity().y);
			}
			else if (RectIntersect(balls[i].GetRect(),def.LeftBound())) {
				balls[i].xset(5+5);
				balls[i].SetVelocity(-balls[i].GetVelocity().x,balls[i].GetVelocity().y);
			}
			if (CheckCollision(paddles[1],balls[i]) && balls[i].GetReal()) {
/*				balls[i].Rotate();
				paddles[1].Rotate();
				balls[i].SetVelocity(ReflectionVelocity(paddles[i],balls[i]).y,
						ReflectionVelocity(paddles[i],balls[i]).x);
				balls[i].Rotate();
				paddles[1].Rotate();*/
				balls[i].SetVelocity(balls[i].GetVelocity().x,-balls[i].GetVelocity().y);
				balls[i].yset(paddles[1].GetY()-20);
			}
			AddRectList(game, balls[i].GetSDLRect());
		}
	}
}

void UpdateBallsPong(Game * game, Ball *balls,Paddle *paddles, defines def,Mix_Chunk *puk,Mix_Chunk *pluck)
{
	// No need to update the AI everytime
	paddles[0].SetUpdate(0);

	for (int i = 0; i < def.GetMaxBalls(); i++)
	{
		// We don't care about the fake ball(s)
		if (balls[i].GetShow()) {
			// The old rectangle of our object
			AddRectList(game, balls[i].GetSDLRect());
			
			// Arbitrary bound checking
			// Don't want to ball to be too slow on the x-axis
			if (balls[i].GetVelocity().x < 2 && balls[i].GetVelocity().x > 0)
				balls[i].SetVelocity(2,balls[i].GetVelocity().y);
			if (balls[i].GetVelocity().x > -2 && balls[i].GetVelocity().x < 0)
				balls[i].SetVelocity(-2,balls[i].GetVelocity().y);

			balls[i].xadd((int)balls[i].GetVelocity().x);
			balls[i].yadd((int)balls[i].GetVelocity().y);

			// Upper and lower bounds
			if (CheckBounds(&balls[i],def.UpperBound()) || CheckBounds(&balls[i],def.LowerBound())) {
				balls[i].SetVelocity(balls[i].GetVelocity().x,-balls[i].GetVelocity().y);
				paddles[0].SetUpdate(1);
				//sound
			}

			// Collision with paddles
			for (int j = 0; j < def.GetMaxPaddles(); j++) {
				if (CheckCollision(paddles[j],balls[i]) && balls[i].GetReal())
				{
					//sound
					Mix_PlayChannel(0,puk,0);
					balls[i].SetVelocity(ReflectionVelocity(paddles[j],balls[i]).x,
								ReflectionVelocity(paddles[j],balls[i]).y);
					paddles[0].SetUpdate(1);

					// Get the ball out of the paddle's range
					if (j)
						balls[i].xset(paddles[1].GetX() - balls[i].GetSize().x - 2);
					else
						balls[i].xset(paddles[0].GetX() + paddles[0].GetSize().x + 2);
				}
			}

			// The new rectangle
			AddRectList(game, balls[i].GetSDLRect());
		}
	}
	
	// Collision between the balls
	if (CheckBalls(balls,def)) {
		Mix_PlayChannel(0,pluck,0);
		paddles[0].SetUpdate(1);
	}
}
