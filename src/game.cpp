#include "game.h"

int GameMain(Game *game,int init,Ball *balls, Paddle *paddles, Powerup *powerups, defines def,CSpriteBase * powerup_sprite,Mix_Chunk *puk,Mix_Chunk *pluck, IPaddress address, UDPsocket sock, int channel, int net)
{
	SDL_Surface * screen = game->GetScreen();
	//Game game; // In order to be able to play time after time, game has to
					// be initialzed every time...
	if (init) InitGame(game, balls,paddles,powerups,game->GetMode(),game->GetDifficulty(),def);
	Uint8* keys;
	//DrawBG(screen,back);
	//UpdateAI();
	int gone=0;
	int done=0,state=0;
	int mx,my,myold=0,mspeed;
	int update=0;
	UDPpacket *in,*out;
	int numrecv=0;
	if (net) {
		if (net==1) {
			in=SDLNet_AllocPacket(3);
			out=SDLNet_AllocPacket(20);
		}
		else {
			in=SDLNet_AllocPacket(20);
			out=SDLNet_AllocPacket(3);
		}
		if(!in || !out) {
			printf("SDLNet_AllocPacket: %s\n", SDLNet_GetError());
		}
	}
	SDL_GetMouseState(&mx, &my);
	while (!done)
	{
		SDL_Event event;

		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)  { done = 1; }

			if (event.type == SDL_KEYDOWN)
				if (event.key.keysym.sym == SDLK_ESCAPE) {
					if (net) {
						SDLNet_FreePacket(out);
						SDLNet_FreePacket(in);
					}
					return 1;
				}
				if (event.key.keysym.sym == SDLK_p) {
					game->SetBegin(0);
					done=1;
				}
		}

		if (!game->GetEnd())
		{
			ResetRectList(game);
			keys = SDL_GetKeyState(NULL);
			SDL_GetMouseState(&mx, &my);
			mspeed = my - myold;
			myold = my;
			for (int i = 0; i < def.GetMaxPaddles(); i++)
			{
				if (paddles[i].GetType() == Human)
				{
					if (paddles[i].GetControl() == Mouse)
					{
						if (mspeed > 10)
							mspeed = 10;
						else if (mspeed < -10)
							mspeed = -10;
						paddles[i].SetVelocity(0,(float)mspeed);
					}
					else if (paddles[i].GetControl() == Keyboard)
					{
						if (game->GetMode() == 1) {
							if (keys[SDLK_UP])
								paddles[i].SetVelocity(0,paddles[i].GetVelocity().y-2);
							else if (keys[SDLK_DOWN])
								paddles[i].SetVelocity(0,paddles[i].GetVelocity().y+2);
							else paddles[i].SetVelocity(0,0);
							if (paddles[i].GetVelocity().y > 12)
								paddles[i].SetVelocity(0,12);
							else if (paddles[i].GetVelocity().y < -12)
								paddles[i].SetVelocity(0,-12);
						}
						else if (game->GetMode() == 2) {
							if (keys[SDLK_RIGHT])
								paddles[i].SetVelocity(paddles[i].GetVelocity().y+5,0);
							else if (keys[SDLK_LEFT])
								paddles[i].SetVelocity(paddles[i].GetVelocity().y-5,0);
							else paddles[i].SetVelocity(0,0);
							if (paddles[i].GetVelocity().x > 25)
								paddles[i].SetVelocity(25,0);
							else if (paddles[i].GetVelocity().x < -25)
								paddles[i].SetVelocity(-25,0);
						}
						if (net == 2) {
							AddRectList(game, paddles[i].GetSDLRect());
							paddles[i].yadd(paddles[i].GetVelocity().y);
							CheckBounds(&paddles[i],def.UpperBound());
							CheckBounds(&paddles[i],def.LowerBound());
							out->len=4;
							out->data[0]=paddles[i].GetY()/3;
							out->data[1]=abs(paddles[i].GetVelocity().y);
							out->data[2]=sign(paddles[i].GetVelocity().y);
							if (!paddles[i].GetVelocity().y) update=1;
						}
					}
				}
			}
			if (net) numrecv=SDLNet_UDP_Recv(sock, in);
			if (numrecv) {
				gone=0;
				if (net == 1) {
					AddRectList(game, paddles[0].GetSDLRect());
					paddles[0].yset(in->data[0]*3);
					AddRectList(game, paddles[0].GetSDLRect());
				}
				else if (!game->GetBegin()) {
					game->SetBegin(1);
/*					InitBackground(screen);
					SDL_Flip(screen);*/
				}
			}
			else if (net == 1) {
				paddles[0].SetVelocity(0,in->data[1]);
				if (!in->data[2]) paddles[0].SetVelocity(0,-in->data[1]);
			}
			if (keys[SDLK_SPACE] && !game->GetBegin() && net < 2) {
				game->SetBegin(true);
				InitBackground(game->GetScreen());
				SDL_Flip(screen);
			}

			else if (game->GetBegin())
			{
				if (net < 2) {
					state=CheckState(game,balls,paddles,def);
					if (state==2) return 1;
					else if (state==1) ResetGame(game, balls,paddles,powerups,game->GetMode(),game->GetDifficulty(),def);
					UpdateScene(game, balls,paddles,powerups, game->GetDifficulty(),game->GetMode(),def,powerup_sprite,puk,pluck);
				}
				else if (numrecv)
					ParseNetData(game,balls,paddles,in->data);
				else
					UpdateScene(game, balls,paddles,powerups, game->GetDifficulty(),game->GetMode(),def,powerup_sprite,puk,pluck);

				DrawScene(game, balls,paddles,powerups, def);
			}
			if (!update) {
				if (net == 1)
					UpdateClient(game,balls,paddles,sock,channel);
				else if (net == 2)
					SDLNet_UDP_Send(sock, channel, out);
			}
			update++;
			update = update % 2;
			SDL_Delay(20);
		}
		else
			done = 1;
	}
	if (net) {
		SDLNet_FreePacket(out);
		SDLNet_FreePacket(in);
	}
	SDL_ShowCursor(1);
	return 0;
}

void InitGame(Game * game, Ball *balls, Paddle *paddles, Powerup *powerups,int mode,int difficulty,defines def)
{
	SDLFont * font = game->GetFont();
	InitBalls(balls,mode,difficulty,def);
	InitPaddles(paddles,balls,mode,def);
	InitPowerups(powerups,mode,def);
	InitBackground(game->GetScreen());
	drawString(game->GetScreen(),font,450-stringWidth(font,"Press space to begin"),200-30,"Press space to begin");
	SDL_Flip(game->GetScreen());
}

int ReachLimit(Game *game, Paddle *paddles)
{
	if (paddles[1].GetPoints() >= game->GetLimit())
		return 2;
	else if (paddles[0].GetPoints() >= game->GetLimit())
		return 1;
	else
		return false;
}

void ResetGame(Game * game, Ball *balls, Paddle *paddles, Powerup *powerups, int mode, int difficulty, defines def)
{
	SDL_Surface * screen = game->GetScreen();
	SDLFont * font = game->GetFont();
	srand(time(NULL));
	ResetBalls(balls,mode,difficulty, def);
	ResetPaddles(paddles,balls,mode,def);
	ResetPowerups(powerups,mode,def);
	InitBackground(game->GetScreen());
	drawString(screen,font,450-stringWidth(font,"Press space to begin"),200-30,"Press space to begin");
	SDL_Flip(screen);
}

int CheckState(Game *game, Ball *balls, Paddle *paddles, defines def)
{
	int pos;
	SDLFont * font = game->GetFont();
	SDL_Surface * screen = game->GetScreen();
	if (game->GetMode() == 1)
	{
		for (int i = 0; i < def.GetMaxBalls(); i++)
			if (balls[i].GetReal())
				CheckWinLose (game, &balls[i], paddles, def);

		if (game->GetLose())
		{
			int reach = ReachLimit(game,paddles);
			if (reach)
			{
				DrawResults(game, paddles,290,240);
				if (reach==1) {
					pos = stringWidth(font,"Moshe says: YOU SUCK");
					pos = (def.GetWindowWidth() - pos)/2;
					drawString(screen,font,pos,def.GetWindowHeight()/2-60,"Moshe says: YOU SUCK");
				}
				else {
					pos = stringWidth(font,"Moshe shuts the fuck up");
					pos = (def.GetWindowWidth() - pos)/2;
					drawString(screen,font,pos,def.GetWindowHeight()/2-60,"Moshe shuts the fuck up");
				}
				SDL_Flip(screen);

				SDL_Delay(1000);
				game->SetEnd(true);
				return 2;
			}
			else
			{
				game->SetLose(false);
				game->SetBegin(false);
			}
			return 1;
		}
		return 0;
	}
	else if (game->GetMode() == 2)
	{
		return 0;
		for (int i = 0; i < def.GetMaxBalls(); i++)
			if (balls[i].GetReal())
				if (RectIntersect(balls[i].GetRect(),def.LowerBound()))
					return 2;
		return 0;
	}
}

void UpdateScene(Game * game, Ball *balls, Paddle *paddles, Powerup *powerups, int difficulty, int mode, defines def,CSpriteBase * powerup_sprite,Mix_Chunk *puk,Mix_Chunk *pluck) {
	UpdateBalls(game,balls,paddles,mode,def,puk,pluck);
	UpdatePaddles(game,paddles,balls, difficulty,mode,def);
	UpdatePowerups(game,powerups,balls,paddles,mode,def,powerup_sprite);
}

void CheckWinLose(Game *game,Ball *ball,Paddle *paddles, defines def) {
	if (!game->GetLose()) {
		for (int i = 0; i < def.GetMaxPaddles(); i++) {
			if (RectIntersect(ball->GetRect(),paddles[i].GetBound())) {
				if (ball->GetReal()) {
					game->SetLose(true);
					paddles[i].IncPoints();
				}
				else {
					ball->SetShow(0);
					ball->set(0,0);
					ball->SetVelocity(0,0);
				}
			}
		}
	}
}
