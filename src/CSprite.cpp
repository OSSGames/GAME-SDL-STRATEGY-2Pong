/*
  CSprite.h
  Simple SDL sprite class.
  Made by Marius Andra 2001
  http://cone3d.gamedev.net

  You can use the code for anything you like.
  Even in a commercial project.
  But please let me know where it ends up.
  I'm just curious. That's all.
*/

#include "CSprite.h"

void AddToList(SDL_Rect list[], SDL_Rect rect, int *rectlistnum) {
	int temp = *rectlistnum;
	list[temp]=rect;
	*rectlistnum+=1;
}

void CSprite::destroy() {
//	SDL_FreeSurface(mBackreplacement);
//	mSpriteBase->destroy();
}

int CSprite::init(CSpriteBase *base, SDL_Surface *screen)
{
  mFrame=0;
  mSpriteBase = base;
  if(mSpriteBase->mBuilt)
  {
    if(mSpriteBase->mNumframes>1) mAnimating=1;
    mBackreplacement = SDL_DisplayFormat(mSpriteBase->mAnim[0].image);
  }
  mScreen = screen;
  return 0;
}

void CSprite::clearBG()
{
  SDL_Rect dest={1,1,1,1};
  if(mDrawn==1)
  {
    dest.x = mOldX;
    dest.y = mOldY;
    dest.w = mSpriteBase->mW;
    dest.h = mSpriteBase->mH;
    SDL_BlitSurface(mBackreplacement, NULL, mScreen, &dest);
  }
}

void CSprite::updateBG()
{
  SDL_Rect srcrect;
  srcrect.w = mSpriteBase->mW;
  srcrect.h = mSpriteBase->mH+1;
  srcrect.x = mX;
  srcrect.y = mY;
  mOldX=mX;mOldY=mY;
  SDL_BlitSurface(mScreen, &srcrect, mBackreplacement, NULL);
}

void CSprite::draw()
{
  if(mAnimating == 1)
  {
    if(mLastupdate+mSpriteBase->mAnim[mFrame].pause*mSpeed<SDL_GetTicks())
    {
      mFrame++;
      if(mFrame>mSpriteBase->mNumframes-1) mFrame=0;
      mLastupdate = SDL_GetTicks();
    }
  }
  if(mDrawn==0) mDrawn=1;
  SDL_Rect dest={1,1,1,1};
  dest.x = mX; dest.y = mY;
  dest.w = mSpriteBase->mW;
  dest.h = mSpriteBase->mH;
  SDL_BlitSurface(mSpriteBase->mAnim[mFrame].image, NULL, mScreen, &dest);
}
