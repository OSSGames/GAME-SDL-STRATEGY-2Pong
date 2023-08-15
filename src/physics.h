#ifndef __PHYSICS_H__
#define __PHYSICS_H__
#include "defs.h"
#include "math.h"

int RectIntersect(rectangle rect1 , rectangle rect2);
int CheckCollision(object a, object b);
int CheckBounds(object *obj, rectangle bound);
int CheckBalls(Ball *balls, defines def);
point ReflectionVelocity(object paddle, object ball);


#endif
