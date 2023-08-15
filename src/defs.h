#ifndef __DEFS_H__
#define __DEFS_H__

#include <SDL/SDL.h>
#include <time.h>
#include "CSprite.h"
#include <stdlib.h>
#include "font.h"

enum player_type { Human, AI, Net };
enum control_type { Mouse, Keyboard };
enum powerups_type { NotActive, DoubleSpeed, Split, HalfSpeed, IncSpeed, DecSpeed, ChangePos };
#define MAX 10

typedef struct options 
{
	char *title;
	char *mode;
	char *difficulty;
	char *submenu;
	char *type;
	char *action;
} options, *optionsPtr;

float sqr(float x);

int sign (float i);

SDL_Rect FixRect (SDL_Rect rect);

int min(int a, int b);

int max(int a, int b);

#define DOT_PRODUCT(ux,uy,vx,vy) ((ux)*(vx) + (uy)*(vy))

struct point
{
	float x;
	float y;
};

struct rectangle
{
	point ul;
	point lr;
};

class Powerup: public CSprite
{
public:
	int GetState() { return state; }
	void SetState(int x) { state = x; }
        int GetStartTime() { return starttime; }
        void SetStartTime(int x) { starttime = x; }
        int GetTime() { return time; }
        void SetTime(int x) { time = x; }
	rectangle GetRect();
	void SetSize(float x,float y);	
	point GetSize() { return size; }
	void SetAlive(bool a) { alive = a; }
	bool GetAlive() { return alive; }
	SDL_Rect GetSDLRect();
private:
	bool alive;
	int state;
	int starttime;
	int time;
	point size;
};

class object: public CSprite
{
public:

	object();

	void SetVelocity(point Velocity);
	void SetVelocity(float x, float y);
	point GetVelocity() { return velocity; }

	void SetSize(float x, float y);
	void SetSize(float x);
	point GetSize() { return size; }
	point GetAccel() { return accel; }
	void SetAccel(float x, float y);
	void SetMass(int Mass) { mass = Mass;}
	int GetMass() { return mass;}
	void ScaleVelocity(float s);
	point GetCenter();

	rectangle GetRect();
        int GetReal() { return real; }
        void SetReal(int x) { real=x; }
        int GetShow() { return show; }
        void SetShow(int x) { show=x; }
	void Rotate();
	SDL_Rect GetSDLRect();
private:
	int real;
	int show;
	point size;
	point velocity;
	point accel;
	int mass;
};

class Game
{
public:
	Game();
	bool GetFirstInit() { return firstinit;}
	void SetFirstInit(bool FirstInit) { firstinit = FirstInit; }

	bool GetLose() { return lose;}
	void SetLose(bool Lose) { lose = Lose; }

	int GetLimit() { return limit; }
	void SetLimit(int Limit) { limit = Limit; }

	bool GetBegin() { return begin;}
	void SetBegin(bool Begin) { begin = Begin; }

    bool GetNet() { return net;}
    void SetNet(bool Net) { net = Net; }

	bool GetEnd() { return end;}
	void SetEnd(bool End) { end = End; }

	int GetMode() { return mode; }
	void SetMode(int x) { mode = x; }

	int GetDifficulty() { return difficulty; }
	void SetDifficulty(int x) { difficulty = x; }

	SDL_Surface * GetScreen() { return screen; }
	void SetScreen(SDL_Surface * Screen) { screen = Screen; }

	SDLFont * GetFont() { return font; }
	void SetFont(SDLFont * Font) { font = Font; }

	SDL_Rect * GetRectList() { return rectlist; }
	int GetRectNum() { return numrects; }
	void SetRectNum(int x) { numrects = x; }

  private:

	bool lose;
	bool firstinit;
	int limit;
	bool begin;
	bool end;
	int mode;
	int difficulty;
	bool net;
	SDL_Surface * screen;
	SDLFont * font;
	SDL_Rect rectlist[30];
	int numrects;
};


class Ball: public object 
{
public:
private:
};

class Paddle: public object
{
public:
	Paddle() { points = 0;}
	char GetPoints() {return points;}
	void SetPoints(int Points) { points = Points; }
	void IncPoints() { points++;}
	int GetType() {return type;}
	void SetType(int x) { type=x; }
	int GetControl() {return control;}
	void SetControl(int x) { control=x; }
	void SetBound(rectangle r) { bound = r; }
	rectangle GetBound() { return bound; }
	bool GetUpdate() { return update; }
	void SetUpdate(bool x) { update = x; }
private:
	int type;
	int points;
	int control;
	rectangle bound;
	bool update;
		
};

/*
char * paddle::GetPoints()
{
	char buffer1 [10]; 
	return itoa(points,buffer1,16);
}*/

class defines
{
public:
	defines();

	int GetMaxBalls() { return maxballs;}
	void SetMaxBalls(int MaxBalls) { maxballs = MaxBalls+1;}

	int GetMaxPaddles() { return maxpaddles; }
	void SetMaxPaddles(int MaxPaddles) { maxpaddles = MaxPaddles; }

	int GetMaxPowerups() { return maxpowerups;}
	void SetMaxPowerups(int MaxPowerups) { maxpowerups = MaxPowerups;}

	int GetMaxSprites() { return maxsprites;}
	void SetMaxSprites(int MaxSprites) { maxsprites = MaxSprites;}

	int GetBallSize() {return ballsize;}
	void SetBallSize(int BallSize) { ballsize = BallSize;}

	int GetWindowHeight() {return windowheight;}
	void SetWindowHeight(int WindowHeight) { windowheight = WindowHeight;}

	int GetWindowWidth() {return windowwidth;}
	void SetWindowWidth(int WindowWidth) { windowwidth = WindowWidth;}

	bool GetPowerups() { return powerups; }
	void SetPowerups(bool x) { powerups=x; }
	rectangle defines::UpperBound();
	rectangle defines::LowerBound();
	rectangle defines::RightBound();
	rectangle defines::LeftBound();

private:
	int maxballs;
	int maxpaddles;
	int maxpowerups;
	int maxsprites;
	int ballsize;
	int windowheight;
	int windowwidth;
	bool powerups;
};

#endif
