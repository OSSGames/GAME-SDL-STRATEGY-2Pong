#ifndef __MENU_H__
#define __MENU_H__

#include "net.h"
#include "defs.h"
#include "game.h"
#include "draw.h"
#include <libxml/tree.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL/SDL.h>

char* GetNodeValue(xmlNode* a_node, char *name);
xmlNode* GetPlayNode(xmlNode * a_node,char* mode,char* difficulty);
xmlNode* GetNode(xmlNode * a_node,char* name);
void SubMenu(xmlNode *elem, char *name);
void DrawMenu(xmlNode *Nodes[10],int hover, int n, SDL_Surface *screen, SDLFont * font);
xmlNode * ShowNode(xmlNode * a_node, char * name,SDL_Surface *screen, SDLFont *font);
optionsPtr ParseNode(xmlNode* cur);
int InitMenu(char *file, Game *game,Ball *balls, Paddle *paddles, Powerup *powerups, defines def,CSpriteBase * powerup_sprite,Mix_Chunk *puk,Mix_Chunk *pluck);

#endif
