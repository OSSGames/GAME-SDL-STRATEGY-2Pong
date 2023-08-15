/*
  CSprite.h
  Header for the simple SDL sprite class.
  Made by Marius Andra 2001
  http://cone3d.gamedev.net

  You can use the code for anything you like.
  Even in a commercial project.
  But please let me know where it ends up.
  I'm just curious. That's all.
*/

#ifndef __CSPRITE_H__
#define __CSPRITE_H__

#include <SDL/SDL.h>

#include "CSpriteBase.h"

class CSprite
{
  public:
  CSprite() {}
  ~CSprite() {};
  int init(CSpriteBase *base, SDL_Surface *screen);
  void destroy();
  void draw();
  void clearBG();
  void updateBG();

  void setFrame(int nr) { mFrame = nr; }
  int getFrame() { return mFrame; }

  void setSpeed(float nr) { mSpeed = nr; }
  float getSpeed() { return mSpeed; }

  void toggleAnim() { mAnimating = !mAnimating; }
  void startAnim() { mAnimating = 1; }
  void stopAnim() { mAnimating = 0; }
  void rewind() { mFrame = 0; }

  void xadd(int nr) { mX+=nr; }
  void yadd(int nr) { mY+=nr; }
  void xset(int nr) { mX=nr; }
  void yset(int nr) { mY=nr; }
  void set(int xx, int yy) { mX=xx; mY=yy; }

  int GetX() { return mX;}
  int GetY() { return mY;}
  int GetXold() { return mOldX;}
  int GetYold() { return mOldY;}

  private:
  int mFrame;
  int mX, mY, mOldX, mOldY;
  int mAnimating;
  int mDrawn;
  float mSpeed;
  long mLastupdate;
  CSpriteBase *mSpriteBase;
  SDL_Surface *mBackreplacement;
  SDL_Surface *mScreen;
};

#endif
