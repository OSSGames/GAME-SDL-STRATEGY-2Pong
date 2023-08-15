#include "defs.h"

#define WINDOW_HEIGHT 480;
#define WINDOW_WIDTH 640;

float sqr(float x)
{
	return x*x;
}

int sign (float i)
{
	if (i <= 0) return 0;
	else return 1;
}
SDL_Rect FixRect (SDL_Rect rect) {
	if (rect.x <= 0) rect.x = 0;
	if (rect.x >= 640) rect.x = 640;
	if (rect.y <= 0) rect.y = 0;
	if (rect.y >= 480) rect.y = 480;
	if (rect.x + rect.w >= 640) rect.w = 640 - rect.x ;
	if (rect.y + rect.h >= 480) rect.h = 480 - rect.y ;
	return rect;
}
int min(int a, int b)
{
	return a < b ? a : b;
}

int max(int a, int b)
{
	return a > b ? a : b;
}
void Powerup::SetSize(float x, float y)
{
	size.y = y;
	size.x = x;
}

SDL_Rect Powerup::GetSDLRect() {
	SDL_Rect rect;
	rect.x = GetX();
	rect.y = GetY();
	rect.h = (int)size.y;
	rect.w = (int)size.x;
	return FixRect(rect);
}

rectangle Powerup::GetRect()
{
	rectangle rect;
	rect.ul.x = (float)GetX();
	rect.ul.y = (float)GetY();
	rect.lr.x = GetX()+size.x;
	rect.lr.y = GetY()+size.y;
	return rect;
}
object::object()
{
	srand (time(NULL));
	mass = 1;
}

SDL_Rect object::GetSDLRect() {
	SDL_Rect rect;
	rect.x = GetX();
	rect.y = GetY();
	rect.h = (int)size.y;
	rect.w = (int)size.x;
	return FixRect(rect);
}
void object::SetVelocity(point Velocity)
{
	velocity.x = Velocity.x;
	velocity.y = Velocity.y;
}

void object::Rotate() {
        float temp;
        temp = size.x;
        size.x = size.y;
        size.y = temp;
        temp = velocity.x;
        velocity.x = velocity.y;
        velocity.y = temp;
        temp = GetX();
        xset(GetY());
        yset(temp);
}

void object::SetVelocity(float x=NULL,float y=NULL)
{
	//if (x == NULL)
			velocity.y = y;
	//if (y == NULL)
			velocity.x = x;

}
void object::SetAccel(float x,float y)
{
	accel.y = y;
	accel.x = x;

}

void object::SetSize(float x,float y)
{
	size.y = y;
	size.x = x;

}

void object::SetSize(float x)
{
	size.y = x;
	size.x = x;
}

point object::GetCenter() {
	point pt;
	pt.x = GetX() + size.x / 2;
	pt.y = GetY() + size.y / 2;
	return pt;
}

void object::ScaleVelocity(float s)
{
	velocity.x = velocity.x * s;
	velocity.y = velocity.y * s;
}

rectangle object::GetRect()
{
	rectangle rect;
	rect.ul.x = (float)GetX();
	rect.ul.y = (float)GetY();
	rect.lr.x = GetX()+size.x;
	rect.lr.y = GetY()+size.y;
	return rect;
}

rectangle defines::UpperBound() {
	rectangle r1 = {{0,0},{windowwidth,5}};
	return r1;
}

rectangle defines::LowerBound() {
	rectangle r2 = {{0,(windowheight - 5)},{windowwidth,windowheight}};
	return r2;
}

rectangle defines::RightBound() {
	rectangle r3 = {{(windowwidth - 5),0},{windowwidth,windowheight}};
	return r3;
}

rectangle defines::LeftBound() {
	rectangle r4 = {{0,0},{5,windowheight}};
	return r4;
}

Game::Game():
	lose(false),
	firstinit(true),
	begin(false),
	end(false)
{}
defines::defines():
	maxballs(3),
	maxpaddles(2),
	maxpowerups(6),
	maxsprites((maxballs+maxpaddles+1)*2+10),
	ballsize(15),
	powerups(1),
	windowheight(480),
	windowwidth(640)
{}

