#include "menu.h"

char* GetNodeValue(xmlNode* a_node, char *name)
{
	xmlNode *cur_node = NULL;
	for (cur_node = a_node->children; cur_node; cur_node = cur_node->next) {
		if (cur_node->type == XML_ELEMENT_NODE) {
			if (!strcmp(name,(char*) cur_node->name)) {
				return (char*) cur_node->children->content;
			}
		}
	}
	return "aa";
}

xmlNode* GetPlayNode(xmlNode * a_node,char* mode,char* difficulty)
{
	xmlNode *cur_node = NULL,*node=NULL;
	for (node=a_node->children; node; node=node->next) {
		for (cur_node = node->children; cur_node; cur_node = cur_node->next) {
			if (cur_node->type == XML_ELEMENT_NODE) {
				if (!strcmp("option", (const char*) cur_node->name)) {
					if (!strcmp("action", GetNodeValue(cur_node,"type"))) {
						if (!strcmp("play", GetNodeValue(cur_node,"action"))) {
							if (!strcmp(mode, GetNodeValue(cur_node,"mode"))) {
								if (!strcmp(difficulty, GetNodeValue(cur_node,"difficulty"))) {
									return cur_node;
								}
							}
						}
					}
				}
			}
		}
	}
	return NULL;
}

xmlNode* GetNode(xmlNode * a_node,char* name)
{
	xmlNode *cur_node = NULL;

	for (cur_node = a_node; cur_node; cur_node = cur_node->next) {
		if (cur_node->type == XML_ELEMENT_NODE) {
			if (!strcmp("menu", (const char*) cur_node->name))
				if (!strcmp(name,(char*) cur_node->properties->children->content)) {
					return cur_node;
				}
		}
		GetNode(cur_node->children,name);
	}
	//   return NULL;
}

void SubMenu(xmlNode *elem, char *name) {
	xmlNode *root_element = NULL, *wanted_node = NULL;
	root_element = xmlDocGetRootElement(elem->doc);
	wanted_node = GetNode(root_element,name);

}

void DrawMenu(xmlNode *Nodes[10],int hover, int n, SDL_Surface *screen, SDLFont * font) {
	char * str;
	SDL_FillRect(screen,NULL,0x000000);
	drawString(screen,font,60,10,GetNodeValue(Nodes[0],"title"));
	for (int i=1;i<n;i++) {
		str=GetNodeValue(Nodes[i],"title");
		if (hover == i) {
			if (!strcmp("setting",GetNodeValue(Nodes[i],"type"))) {
				drawString(screen,font,90,20+(i+1)*40,str);
				str=GetNodeValue(Nodes[i],"selected");
				drawString(screen,font,305,20+(i+1)*40,str);
			}
			else
				drawString(screen,font,95,20+(i+1)*40,str);
		}
		else {
			drawString(screen,font,90,20+(i+1)*40,str);
			if (!strcmp("setting",GetNodeValue(Nodes[i],"type"))) {
				str=GetNodeValue(Nodes[i],"selected");
				drawString(screen,font,300,20+(i+1)*40,str);
			}
		}
	}
	SDL_Flip(screen);
}

xmlNode * ShowNode(xmlNode * a_node, char * name,SDL_Surface *screen, SDLFont *font) {
	xmlNode *cur_node = NULL;
	xmlNode *Nodes[10];
	int x=0,y=0,n=1,done=0,i=0,mousehover=0;
	char * str;
	InitBackground(screen);
	Nodes[0]=a_node;
	str=GetNodeValue(a_node,"title");
	drawString(screen,font,60,10,str);
	for (cur_node = a_node->children; cur_node; cur_node = cur_node->next) {
		if (cur_node->type == XML_ELEMENT_NODE) {
			if (!strcmp("option",(char*) cur_node->name)) {
				str=GetNodeValue(cur_node,"title");
				drawString(screen,font,90,20+(n+1)*40,str);
				Nodes[n]=cur_node;
				n++;
			}
		}
	}
	SDL_Flip(screen);
	while (!done) {
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)  { return NULL; }

			if (event.type == SDL_KEYDOWN)
				if (event.key.keysym.sym == SDLK_ESCAPE) {
					if (!strcmp(name,"main")) {
						return NULL;
					}
					else {
						done = 1;
					}
				}
			if (event.type == SDL_MOUSEBUTTONUP) {
				if (mousehover)
					return Nodes[mousehover];
			}
		}
		SDL_GetMouseState(&x, &y);
		if (x > 90 && x < 300 && y > 60 && y < 20+(n+1)*40) {
			i=(int)(y-20)/40 - 1;
			if (i != mousehover) {
				DrawMenu(Nodes,i,n,screen,font);
			}
			mousehover=i;
		}
		else {
			if (mousehover)
				DrawMenu(Nodes,0,n,screen,font);
			mousehover=0;
		}
		SDL_Delay(25);
	}
	return (a_node->parent);
}

optionsPtr ParseNode(xmlNode* cur) {
	optionsPtr ret = NULL;
	ret = (optionsPtr) malloc(sizeof(options));
	if (ret == NULL) {\
		fprintf(stderr,"out of memory\n");
		return(NULL);
	}
	memset(ret, 0, sizeof(options));
	cur=cur->children;
	ret->type = "";
	ret->action = "";
	ret->submenu = "NUUUUULL";
	ret->mode = "";
	ret->difficulty = "";
	while (cur != NULL) {
		if ((!strcmp((char *)cur->name, "type")))
			ret->type = (char *) cur->children->content;
		if ((!strcmp((char *)cur->name, "submenu")))
			ret->submenu = (char *) cur->children->content;
		if ((!strcmp((char *)cur->name, "action")))
			ret->action = (char *) cur->children->content;
		if ((!strcmp((char *)cur->name, "mode")))
			ret->mode = (char *) cur->children->content;
		if ((!strcmp((char *)cur->name, "difficulty")))
			ret->difficulty = (char *) cur->children->content;
		cur = cur->next;
	}
	return ret;
}


int InitMenu(char *file, Game *game,Ball *balls, Paddle *paddles, Powerup *powerups, defines  def,CSpriteBase * powerup_sprite,Mix_Chunk *puk,Mix_Chunk *pluck) {
	UDPsocket sock;
	IPaddress address,aout;
	int channel;
	int port;
	bool selected;
	optionsPtr curoptions;
	char * name;
	xmlDoc *doc = NULL;
	int init=1;
	xmlNode *root_element = NULL, *wanted_node = NULL,*Selected_node = NULL;
	doc = xmlReadFile(file, NULL, 0);
	if (doc == NULL) {
		printf("error: could not parse file %s\n", file);
	}
	root_element = xmlDocGetRootElement(doc);
	wanted_node = GetNode(root_element,"main");
	while (wanted_node) {
		if (!strcmp((char*)wanted_node->name,"menu")) {
			selected=1;
			Selected_node=ShowNode(wanted_node,name,game->GetScreen(),game->GetFont());
		}
		else {
			selected=0;
			Selected_node=wanted_node;
		}
		if (!Selected_node) return 0;
		curoptions=ParseNode(Selected_node);
		if (!strcmp(curoptions->type,"action")) {
			if (!strcmp(curoptions->action,"quit")) {
				return 0;
			}
			else if (!strcmp(curoptions->mode,"server")) {
				if(SDLNet_Init()==-1) {
					printf("SDLNet_Init: %s\n", SDLNet_GetError());
					exit(2);
				}
				game->SetBegin(0);
				game->SetEnd(0);
                                game->SetDifficulty(1);
                                game->SetMode(1);				
				paddles[0].SetType(Net);
				paddles[1].SetType(Human);
				paddles[1].SetControl(Keyboard);
				port = GetPort(game->GetScreen(),game->GetFont(),1);
				if (port>0) {
					InitBackground(game->GetScreen());
					drawString(game->GetScreen(),game->GetFont(),60,10,"Waiting For Connection ...");
					SDL_Flip(game->GetScreen());
					if (!StartServer(&sock,&address,&channel,port)) {
						GameMain(game,init,balls,paddles,powerups,def,powerup_sprite,puk,pluck,address,sock,channel,1);
					}
					SDLNet_UDP_Unbind(sock, channel);
					SDLNet_UDP_Close(sock);
				}
				SDLNet_Quit();
			}
			else if (!strcmp(curoptions->mode,"client")) {
				if(SDLNet_Init()==-1) {
					printf("SDLNet_Init: %s\n", SDLNet_GetError());
				}	
				game->SetBegin(0);
				game->SetEnd(0);
				game->SetDifficulty(1);
				game->SetMode(1);
				paddles[0].SetType(Human);
				paddles[1].SetType(Net);
				paddles[0].SetControl(Keyboard);
				port = GetPort(game->GetScreen(),game->GetFont(),0);
				if (port >= 0) { 
					address = GetAddress(game->GetScreen(),game->GetFont());
					InitBackground(game->GetScreen());
					drawString(game->GetScreen(),game->GetFont(),60,10,"Connecting ...");
					SDL_Flip(game->GetScreen());
					StartClient(&sock,address,&channel,port);
					GameMain(game,init,balls,paddles,powerups,def,powerup_sprite,puk,pluck,address,sock,channel,2);
					SDLNet_UDP_Unbind(sock, channel);
					SDLNet_UDP_Close(sock);
				}
				SDLNet_Quit();
			}
			else if (!strcmp(curoptions->action,"play")) {
				game->SetBegin(0);
				game->SetEnd(0);
				game->SetDifficulty(1);
				paddles[0].SetType(AI);
				paddles[1].SetType(Human);
				def.SetPowerups(1);
				if (!strcmp(curoptions->difficulty,"easy"))
					
					game->SetDifficulty(1);
				if (!strcmp(curoptions->difficulty,"medium"))
					game->SetDifficulty(2);
				if (!strcmp(curoptions->difficulty,"hard"))
				{
					game->SetDifficulty(3);
				}
				if (!strcmp(curoptions->mode,"regular")) {
					game->SetMode(1);
					def.SetMaxBalls(2);
				}
				else if (!strcmp(curoptions->mode,"classic")) {
					game->SetMode(1);
					def.SetPowerups(0);
					def.SetMaxBalls(1);
				}
				else if (!strcmp(curoptions->mode,"kickups")) {
					def.SetPowerups(0);
					game->SetMode(2);
					def.SetMaxBalls(2);
				}
				else if (!strcmp(curoptions->mode,"quickplay")) {
					game->SetDifficulty(rand()%3+1);
					game->SetMode(1);
				}
				else if (!strcmp(curoptions->mode,"network")) {
					game->SetMode(1);
					paddles[0].SetType(Net);
				}
				if (selected) init=1;
				else init=0;
				if (!GameMain(game,init,balls,paddles,powerups,def,powerup_sprite,puk,pluck,address,sock,-1,0)) {
					name="pause-menu";
					wanted_node=GetNode(root_element,name);
				}
				else {
					name="main";
					wanted_node=GetNode(root_element,name);
				}
			}
			else if (!strcmp(curoptions->action,"resume")) {
				InitBackground(game->GetScreen());
				DrawScene(game,balls,paddles,powerups,def);
				drawString(game->GetScreen(),game->GetFont(),450-
					stringWidth(game->GetFont(),"Press space to continue"),200-30,"Press space to continue");
				SDL_Flip(game->GetScreen());
				wanted_node=GetPlayNode(root_element,"regular","easy");
			}
		}
		else if (!strcmp(curoptions->type,"submenu")) {
			name=curoptions->submenu;
			wanted_node=GetNode(root_element,name);
		}
		else wanted_node=GetNode(root_element,"main");
	}
	xmlFreeDoc(doc);
	xmlCleanupParser();
	return 1;
}

