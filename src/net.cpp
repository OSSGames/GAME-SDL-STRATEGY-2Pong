#include "net.h"

void ParseNetData(Game *game,Ball * balls, Paddle * paddles, Uint8 * data) {
	AddRectList(game, balls[1].GetSDLRect());
	AddRectList(game, balls[2].GetSDLRect());
	AddRectList(game, paddles[1].GetSDLRect());
	balls[1].set(data[0]*3,data[1]*3);
	balls[2].set(data[4]*3,data[5]*3);
	balls[1].SetVelocity(data[2]/25,data[3]/25);
	if (!data[13])
		balls[1].SetVelocity(-balls[1].GetVelocity().x,balls[1].GetVelocity().y);
	if (!data[14])
		balls[1].SetVelocity(balls[1].GetVelocity().x,-balls[1].GetVelocity().y);
	balls[2].SetVelocity(data[6]/25,data[7]/25);
	if (!data[15])
		balls[2].SetVelocity(-balls[2].GetVelocity().x,balls[2].GetVelocity().y);
	if (!data[16])
		balls[2].SetVelocity(balls[2].GetVelocity().x,-balls[2].GetVelocity().y);
	paddles[1].SetVelocity(0,data[17]);
	paddles[1].SetVelocity(0,0);
	if (!data[18])
		paddles[1].SetVelocity(0,-data[17]);
	paddles[1].yset(data[8]*3);
	paddles[0].SetPoints(data[9]);
	paddles[1].SetPoints(data[10]);
	if (!game->GetBegin() && data[11]) {
		InitBackground(game->GetScreen());
		SDL_Flip(game->GetScreen());
	}
	game->SetBegin(data[11]);
	game->SetLose(data[12]);
	AddRectList(game, balls[1].GetSDLRect());
	AddRectList(game, balls[2].GetSDLRect());
	AddRectList(game, paddles[0].GetSDLRect());
	AddRectList(game, paddles[1].GetSDLRect());
}

void UpdateClient(Game *game,Ball * balls, Paddle * paddles, UDPsocket sock, int channel) {
	int num;
	UDPpacket *out;
	out=SDLNet_AllocPacket(20);
	out->len=17;
	out->data[0]=balls[1].GetX()/3;
	out->data[1]=balls[1].GetY()/3;
	out->data[2]=abs(balls[1].GetVelocity().x*25);
	out->data[3]=abs(balls[1].GetVelocity().y*25);
	out->data[4]=balls[2].GetX()/3;
	out->data[5]=balls[2].GetY()/3;
	out->data[6]=abs(balls[2].GetVelocity().x*25);
	out->data[7]=abs(balls[2].GetVelocity().y*25);
	out->data[8]=paddles[1].GetY()/3;
	out->data[9]=paddles[0].GetPoints();
	out->data[10]=paddles[1].GetPoints();
	out->data[11]=game->GetBegin();
	out->data[12]=game->GetLose();
	out->data[13]=sign(balls[1].GetVelocity().x);
	out->data[14]=sign(balls[1].GetVelocity().y);
	out->data[15]=sign(balls[2].GetVelocity().x);
	out->data[16]=sign(balls[2].GetVelocity().y);
	out->data[17]=abs(paddles[1].GetVelocity().y);
	out->data[18]=sign(paddles[1].GetVelocity().y);
	SDLNet_UDP_Send(sock, channel, out);
	SDLNet_FreePacket(out);
}

int StartServer(UDPsocket *sock, IPaddress *address, int *channel, int port) {
	*sock=SDLNet_UDP_Open(port);
	SDLNet_ResolveHost(address, NULL, port);
	*channel=SDLNet_UDP_Bind(*sock, -1, address);
	if(*channel==-1) {
		printf("SDLNet_UDP_Bind: %s\n", SDLNet_GetError());
		// do something because we failed to bind
	}
	UDPpacket *in,*out;
	in=SDLNet_AllocPacket(10);
	out=SDLNet_AllocPacket(10);
	if(!in|!out) {
		printf("SDLNet_AllocPacket: %s\n", SDLNet_GetError());
		// perhaps do something else since you can't make this packet
	}
	else {
		out->data[1]=rand()%50;
		out->len=3;
		out->channel=*channel;
	}
	SDL_Event event;
	while (!SDLNet_UDP_Recv(*sock, in)) {
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT)  { 
				SDLNet_UDP_Unbind(*sock, *channel);
				SDLNet_FreePacket(in);
				return 1;
			}
			if (event.type == SDL_KEYDOWN)
				if (event.key.keysym.sym == SDLK_ESCAPE) {
					SDLNet_UDP_Unbind(*sock, *channel);
					SDLNet_FreePacket(in);
					return 1;
				}
		}
	}
	printf("data: %d\n",in->data[1]);
	SDLNet_ResolveHost(address, SDLNet_ResolveIP(&in->address), ntohs(in->address.port));
	SDLNet_UDP_Unbind(*sock, *channel);
	SDLNet_UDP_Bind(*sock, *channel, address);
	while (!SDLNet_UDP_Send(*sock, out->channel, out)) {}
	SDLNet_FreePacket(out);
	SDLNet_FreePacket(in);
	return 0;
}
int StartClient(UDPsocket *sock, IPaddress address, int *channel, int myport) {
	*sock=SDLNet_UDP_Open(myport);
	// or whatever port you want
//	SDLNet_ResolveHost(address, "127.0.0.1", hostport);
	*channel=SDLNet_UDP_Bind(*sock, -1, &address);
	if(*channel==-1) {
		printf("SDLNet_UDP_Bind: %s\n", SDLNet_GetError());
		// do something because we failed to bind
	}
	UDPpacket *in,*out;
	in=SDLNet_AllocPacket(10);
	out=SDLNet_AllocPacket(10);
	if(!in || !out) {
		printf("SDLNet_AllocPacket: %s\n", SDLNet_GetError());
	// perhaps do something else since you can't make this packet
	}
	else {
		out->data[1]=rand()%50;
		out->len=3;
		out->channel=*channel;
	}
	while (!SDLNet_UDP_Send(*sock, out->channel, out)) {}
	SDL_Event event;
	while (!SDLNet_UDP_Recv(*sock, in)) {
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT)  {
				SDLNet_UDP_Unbind(*sock, *channel);
				SDLNet_FreePacket(in);
				SDLNet_FreePacket(out);
				return -1;
			}
			if (event.type == SDL_KEYDOWN)
				if (event.key.keysym.sym == SDLK_ESCAPE) {
					SDLNet_UDP_Unbind(*sock, *channel);
					SDLNet_FreePacket(in);
					SDLNet_FreePacket(out);
					return -1;
				}
		}
	}
	SDLNet_FreePacket(out);
	SDLNet_FreePacket(in);
	return 0;
}

IPaddress GetAddress(SDL_Surface * screen, SDLFont * font) {
	IPaddress ip; 
	SDLNet_ResolveHost(&ip, NULL, 1111);
	int done=0,i;
	int len=0,old;
	char str[101];
	char * host;
	long port=0;
	InitBackground(screen);
	drawString(screen,font,60,10,"Enter Host Address:Port");
	SDL_Flip(screen);
	for (i=0;i<101;i++)
		str[i]=0;
	while (!done) {
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT)  { return ip; }
			if (event.type == SDL_KEYDOWN) {
				old = len;
				if (event.key.keysym.sym == SDLK_ESCAPE) { return ip; }
				else if (event.key.keysym.sym == SDLK_RETURN) { done=1; }
				else if ((event.key.keysym.sym <= 57 && event.key.keysym.sym >= 48) || 
						event.key.keysym.sym == 46 ||
						(event.key.keysym.sym <= 90 && event.key.keysym.sym >= 65) ||
						(event.key.keysym.sym <= 122 && event.key.keysym.sym >= 97)) {
					str[len]=event.key.keysym.sym;
					len++;
				}
				else if (event.key.keysym.sym == SDLK_SEMICOLON && (KMOD_RSHIFT || KMOD_LSHIFT)) {
					str[len]=58;
					len++;
				}
				else if (event.key.keysym.sym == SDLK_BACKSPACE) { 
					len--; 
					str[len]=0;
				}
				if (len>=100) len=100;
				if (old!=len) {
					InitBackground(screen);
					drawString(screen,font,60,10,"Enter Host Address:Port");
					drawString(screen,font,90,60,str);
					SDL_Flip(screen);
				}
			}
		}
	}
	i=0;
	while (str[i] != 58)
		i++;
	len=i;
	host = (char *) calloc(i,sizeof(char));
	for (i=0;i<len;i++)
		host[i]=str[i];
	i++;
	while (str[i] <= 57 && str[i] >= 48) {
		port = 10*port 	+ str[i]-48;
		i++;
	} 
	if (port > 65535) port = 65535;
	if (!SDLNet_ResolveHost(&ip, host, port)) return ip;
	else {
		SDLNet_ResolveHost(&ip, NULL, 1111);
		return ip;		
	}
}

int GetPort(SDL_Surface * screen, SDLFont * font, int server) {
	int done=0;
	long old=0,port=0;
	char str[5];
	InitBackground(screen);
	if (!server) drawString(screen,font,60,10,"Enter Your Port (0 for random):");
	else drawString(screen,font,60,10,"Enter Your Port:");
	SDL_Flip(screen);
	while (!done) {
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT)  { return -1; }
			if (event.type == SDL_KEYDOWN) {
				old=port;
				if (event.key.keysym.sym == SDLK_ESCAPE) { return -1; }
				else if (event.key.keysym.sym == SDLK_RETURN) {
					if (!server) done=1; 
					else if (port) done=1;
					else {
						InitBackground(screen);
						drawString(screen,font,60,10,"Not 0 dumbass!");
						SDL_Flip(screen);
						SDL_Delay(200);
					}
				}
				else if (event.key.keysym.sym <= 57 && event.key.keysym.sym >= 48)
					port = 10*port + event.key.keysym.sym - 48;
				else if (event.key.keysym.sym == SDLK_BACKSPACE) { port = port / 10; }
				if (port > 65535) port = 65535;
				if (old!=port) {
					InitBackground(screen);
					(void) sprintf(str,"%ld",port);
					if (!server) drawString(screen,font,60,10,"Enter Your Port (0 for random):");
					else drawString(screen,font,60,10,"Enter Your Port:");
					drawString(screen,font,90,60,str);
					SDL_Flip(screen);
				}
			}
		}
	}
	return port;
}
