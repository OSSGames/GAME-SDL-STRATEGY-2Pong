#ifndef __CSPRITEBASE_H__
#define __CSPRITEBASE_H__

#include <SDL/SDL.h>

struct CSpriteFrame
{
  SDL_Surface *image;
  int pause;
};

class CSpriteBase
{
  public:
  int init(char *dir);
  void destroy();
  CSpriteFrame *mAnim;
  int mBuilt, mNumframes, mW, mH;
};

#endif

