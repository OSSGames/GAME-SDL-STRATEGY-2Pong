#include "ai.h"

float GetTime(Paddle paddle, Ball ball) {
	// Get the time it will take the ball to reach the paddle

	float dist = paddle.GetX() + paddle.GetSize().x - ball.GetX();
	float time = dist / ball.GetVelocity().x;
	return time;
}

int CloseBall(Paddle paddle, Ball *balls, defines def, int difficulty) {
	// Find the ball closest to the paddle

	float mintime = -1;
	int i,mini = -1;
	float time;
	for (i=0;i<def.GetMaxBalls();i++) {
	time = GetTime(paddle,balls[i]);
	if ((time > 0) && (time < mintime || mintime == -1) && balls[i].GetReal()) {
			mini = i;
			mintime = time;
		}
	}
	return mini;
}

void UpdateAI(Paddle *paddle, Ball *balls,int difficulty, defines def) {
	// Check if we even need to update the ai
	if (!paddle->GetUpdate()) return;

	// Limit the ai speed according to the difficulty level
	float limit = difficulty*1.5 + 7.5;
	
	// Get the closest ball
	int j=CloseBall(*paddle,balls,def,difficulty);
	int i=j;
	// If none of the balls is in our direction 
	// Choose an arbitrary value
	if (i<0) i=1;

	// Get the eta of the closest ball
	float time = GetTime(*paddle,balls[i]);

	// Random errors - doesn't work all that well
	// So we'll leave it out for now
/*	float error = float(rand()%3 + 1) / sqr(difficulty) / float(rand()%4+1) / 100;
	if (!rand()%2) error = -error;
	time = (1-error) * time;*/

	// Find the final position of the ball
	float end = balls[i].GetCenter().y + balls[i].GetVelocity().y * time;

	// Add in wall collisions for better accuracy
	// (according to the difficulty level)
	for (int k=1; k<difficulty; k++) {
		if (end > def.GetWindowHeight())
			end = 2*def.GetWindowHeight() - end;
		else if (end < 0)
			end = -end;
	}

	// Get the distance
	float dist = paddle->GetCenter().y - end;
	float vel = dist/time;
	
	// Add a little acceleration
	float accel = vel/(limit/0.2);

	// Limit the velocity
	if (dist > limit*time) {
		dist=limit*time;
		vel = limit;
		accel = -accel;
	} else if (dist < -limit*time) {
		dist=-limit*time;
		vel = -limit;
		accel = -accel;
	}

	// Calculate the velocity according to our acceleration
	vel = (dist + accel*sqr(time)/2) / time;

	// If none of the balls is in our direction
	// Send the paddle to the center of the screen at some velociry
	if (j==-1) {
		vel = (paddle->GetCenter().y - def.GetWindowHeight()/2) / (rand()%50+50);
		paddle->SetAccel(0,0);
	}
	else
		paddle->SetAccel(0,accel);

	paddle->SetVelocity(0,-vel);
}

