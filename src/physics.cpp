#include "defs.h"
#include "physics.h"

int RectIntersect(rectangle rect1 , rectangle rect2)
{
	int ulx = max((int)rect1.ul.x, (int)rect2.ul.x);
	int uly = max((int)rect1.ul.y, (int)rect2.ul.y);
	int lrx = min((int)rect1.lr.x, (int)rect2.lr.x);
	int lry = min((int)rect1.lr.y, (int)rect2.lr.y);
	if (ulx > lrx || uly > lry)
		return 0;
	else
		return 1;
}

int CheckCollision(object a, object b)
{
	return RectIntersect(a.GetRect(), b.GetRect());
}

int CheckBounds(object *obj, rectangle bound)
{
	if (RectIntersect(obj->GetRect(),bound))
	{
		if ((obj->GetY() < bound.lr.y) && (obj->GetVelocity().y < 0))
			obj->yset(bound.lr.y + 1);
		else if ((obj->GetY() + obj->GetSize().y > bound.ul.y) && (obj->GetVelocity().y > 0))
			obj->yset(bound.ul.y  - obj->GetSize().y - 1);
		//Add checks to keep things inside the window
		return 1;
	}
	return 0;
}

int CheckBalls(Ball *balls,defines def)
{
	for (int i = 1; i < def.GetMaxBalls(); i++)
	{
		for (int j = i + 1; j < def.GetMaxBalls(); j++)
		{
			float nabx = (float)(balls[i].GetCenter().x - balls[j].GetCenter().x);
			float naby = (float)(balls[i].GetCenter().y - balls[j].GetCenter().y);
			float length = (float)sqrt(nabx*nabx + naby*naby);
			float ballradius = def.GetBallSize()/2;

			if (length <= 2.0*(ballradius*0.75))
			{

				nabx/=length;
				naby/=length;

				float tabx =  -naby;
				float taby =  nabx;

				float vait = DOT_PRODUCT(balls[i].GetVelocity().x,
										balls[i].GetVelocity().y,
										tabx, taby);

				float vain = DOT_PRODUCT(balls[i].GetVelocity().x,
										balls[i].GetVelocity().y,
										nabx, naby);

				float vbit = DOT_PRODUCT(balls[j].GetVelocity().x,
										balls[j].GetVelocity().y,
										tabx, taby);

				float vbin = DOT_PRODUCT(balls[j].GetVelocity().x,
										balls[j].GetVelocity().y,
										nabx, naby);

				float ma = 1;//balls[i].GetMass();
				float mb = 1;//balls[j].GetMass();

				float vafn = (mb*vbin*(1+1) + vain*(ma - 1*mb)) / (ma + mb);
				float vbfn = (ma*vain*(1+1) - vbin*(ma - 1*mb)) / (ma + mb);

				float vaft = vait;
				float vbft = vbit;

				float xfa = vafn*nabx + vaft*tabx;
				float yfa = vafn*naby + vaft*taby;

				float xfb = vbfn*nabx + vbft*tabx;
				float yfb = vbfn*naby + vbft*taby;

				balls[i].SetVelocity(xfa,yfa);
				balls[j].SetVelocity(xfb,yfb);
				return 1;
			}
		}
	}
	return(0);
}

point ReflectionVelocity(object paddle, object ball) {
	point ret={-ball.GetVelocity().x,ball.GetVelocity().y};
	float vel=sqrt(sqr(ball.GetVelocity().x) + sqr(ball.GetVelocity().y));
	vel+=ball.GetAccel().x;
	if (vel > 12) vel = 12;

	float speedtilt = 7.5*sqr(paddle.GetVelocity().y)/sqr(12);
	if (paddle.GetVelocity().y > 0)
		speedtilt = -speedtilt;
	if (speedtilt > 7.5) speedtilt = 7.5;
	if (speedtilt < -7.5) speedtilt = -7.5;
	float tilt = 7.5/57.3;
	float minangle=40/57.3;
	float diff = (paddle.GetCenter().y - ball.GetCenter().y);
	tilt = (diff / (paddle.GetSize().y / 2)) * tilt ;
	tilt+=speedtilt / 57.3;
	float angle=(float)(atan(ret.x/ret.y));
	if (angle*57.3 > 90) angle=89/57.3;
	if (angle*57.3 < -90) angle=-89/57.3;
	angle = angle - 2*tilt;
	if (angle >= 0 && angle <= minangle) angle = minangle;
	if (angle <= 0 && angle >= -minangle) angle = -minangle;
	if (ret.y>0)
		ret.y= sqrt( sqr(vel) / (sqr(tan(angle)) + 1) );
	else
		ret.y = -sqrt( sqr(vel) / (sqr(tan(angle)) + 1) );
	if (ret.x > 0)
		ret.x = sqrt( sqr(vel) - sqr(ret.y) );
	else
		ret.x = -sqrt( sqr(vel) - sqr(ret.y) );
	return ret;
}

