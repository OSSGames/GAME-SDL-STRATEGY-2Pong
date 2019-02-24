#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>
#include <SDL/SDL.h>
#include <SDL/SDL_endian.h>
#include <SDL/SDL_mixer.h>
#include "BFont.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#define PATH "./"

SDL_Surface *back,*back2,*paddle,*paddle2,*paddle4,*ball,*ball2,*paddle3,*icon,*icon2,*back3,*back4,*back5,*ring,*paddle5,*midring;
Mix_Chunk *hit_wall,*hit_wall,*hit_paddle,*win_snd,*lose_snd,*menu_snd,*click_snd;
float x[2],y[2],xs[2],ys[2],es[3],ej[3],xa[1000][2],xb[1000][2];
int xac=0,xbc=0;
int scorep=0,scoree=0,mode=0,ppause,timer,timerp,timerp2=0,timerc=0,playing=0,count=0,j,j2=0,j3=0,timern=0,tim,timern2=0,sound=1,mj,mj2,state=0,spress=1,full=0;
time_t st,ait,now,stp,nowp,stc,nowc;
Uint32 ta,tb;
int sr=0,mi,bounced=0,df;
float grav=0.1,xinc,yinc,xinc2,yinc2;
int l,server,btime=0,btc=0,bav=0;
char VERSION[20]="2Pong v0.6";
char *PORT,*SRV_IP;
SDL_Surface *back,*back2,*image,*image2,*image3,*screen,*image4,*icon,*icon2;
BFont_Info *Font1=NULL,*Font2=NULL,*Font3=NULL,*Font4=NULL;
char msg[2]; int ms;
struct sockaddr_in si_me, si_other;
int s, r, slen=sizeof(si_other);
#define BUFLEN 512
#define NPACK 10
char buf[BUFLEN];


void diep(char *s)
{
  perror(s);
  exit(1);
}

static SDL_Surface *png_load( char *path )
{
    SDL_RWops *src;
    SDL_Surface *surf;
    if ( ( src = SDL_RWFromFile(path, "rb") ) == 0 ) {
        fprintf( stderr, "%s\n", SDL_GetError() );
        return 0;
    }
    else {
        surf = IMG_LoadPNG_RW( src );
        SDL_RWclose( src );
        return surf;
    }
}


void start_server() {
       if ((s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP))==-1)
          diep("socket");

        memset((char *) &si_me, sizeof(si_me), 0);
        si_me.sin_family = AF_INET;
        si_me.sin_port = htons(PORT);
        si_me.sin_addr.s_addr = htonl(INADDR_ANY);
        if (bind(s, &si_me, sizeof(si_me))==-1)
            diep("bind");
}


void connect_server() {

        if ((s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP))==-1)
          diep("socket");

        memset((char *) &si_other, sizeof(si_other), 0);
        si_other.sin_family = AF_INET;
        si_other.sin_port = htons(PORT);
        if (inet_aton(SRV_IP, &si_other.sin_addr)==0) {
          fprintf(stderr, "inet_aton() failed\n");
          exit(1);
        }

}


Uint32 callback(Uint32 a, Uint32 b);
Uint32 callback(Uint32 a, Uint32 b) {
  timern2=timern2+1;
  return a;
}

Uint32 callback2(Uint32 a, Uint32 b);
Uint32 callback2(Uint32 a, Uint32 b) {
  spress=1;
  return a;
}


void init();
float ai(float ej, int b);
void Bounce(int i, int j);
void Bounce2(int c);
void DrawIMG(SDL_Surface *img, int x, int y);
void DrawBall(SDL_Surface *screen, int i, int j);

void init(int j) {
  srand((long) st); timerp2=0;
  ej[0]=rand()%400+40;
  ys[0]=(rand()%66+4)/13;
  if ((rand()%3577+131)%2==0) ys[0]=ys[0]*-1;
  ys[1]=(rand()%36+4)/13;
  if ((rand()%3577+131)%2==0) ys[1]=ys[1]*-1;
  y[0]=ej[0]; x[0]=50; x[1]=590; y[1]=j;
  if (mode!=4)      xs[0]=5.3;
  xs[1]=-5.3; bounced=0; grav=0.1;
  ej[1]=200; ej[2]=200;
  if (mode==4) {
    xs[0]=(rand()%80+4)/13;
    if ((rand()%3577+131)%2==0) xs[0]=xs[0]*-1;
    if (ys[0]<4 && ys[0]>=0) ys[0]=4;
    if (ys[0]>-4 && ys[0]<=0) ys[0]=-4;
    if (xs[0]<4 && xs[0]>=0) xs[0]=4;
    if (xs[0]>-4 && xs[0]<=0) xs[0]=-4;
    x[0]=315;
    y[0]=236;
  }
  playing=1;
  if (mode == 10) {
    xs[0]=13;
    xs[1]=-13;
  }
  if (!mode) {
    y[0]=419; y[1]=30;
    ys[0]=-9; ys[1]=0;
    xs[0]=(rand()%70+4)/13;
    if ((rand()%3577+131)%2==0) xs[0]=xs[0]*-1;
    xs[1]=(rand()%70+4)/13;
    if ((rand()%3577+131)%2==0) xs[1]=xs[1]*-1;
    x[0]=(rand()%600+24);
    x[1]=(rand()%600+24);
  }
}

void win(int b);
void win(int b) {
df=6;
  if (!mode) {
    if (sound)  if (sound) Mix_PlayChannel(0,lose_snd,0);
    PutString (screen, 160, 196, "Don't Let It Fall!");
  }
  else if (b) {
    scorep+=1;
    if (sound) if (sound) Mix_PlayChannel(0,win_snd,0);
    PutString (screen,  195, 196, "Player Wins!"); btc=0; btime=0;
  }
  else {
    scoree+=1;
    if (sound)  if (sound) Mix_PlayChannel(0,lose_snd,0);
    PutString (screen, 235, 196, "AI Wins!"); btc+=1; btime=0;
  }
  if (btc>=2) { btc=0; if (bav<4) bav++; }
  SDL_UpdateRect(screen, 0, 0, 0, 0);
  init(j); xac=0;
  SDL_Delay(1000);
  (void) time(&st);
}

void win_check(int b);
void win_check(int b) {
  if (mode) {
    if (x[b]<10 && mode !=4) win(1);
    else if (x[b]<10) win(0);
    else if (x[b]>618) win(0);
    else if (mode==4) {
      if (y[b]<14) win(1);
      else if (y[b]>456) win(1);
    }
  }
  else if (y[b]>=469) win(b);
}

float ai(float ej, int b) {
  float aix=x[b],aiy=y[b],aixs=xs[b],aiys=ys[b];
  int a=0;
  while (aix>15){
    if (aiy<11 || aiy>459) aiys=-1*aiys;
    aix=aix+aixs;
    aiy=aiy+aiys;
    a+=1;
  }
  return (aiy-ej)/a; 
}

void BounceBalls();
void BounceBalls() {
  if ((x[0]>=x[1]-12 && x[0]<=x[1]+12) && (y[0]>=y[1]-12 && y[0]<=y[1]+12)) {
    df=6;
    xs[0]=xs[0]*-1;
    xs[1]=xs[1]*-1;
    ys[0]=ys[0]*-1;
    ys[1]=ys[1]*-1;
  }
}

void Bounce(int i, int j) {
  int b,a,lll=1;
if (mode==4) lll=0;
  for (b=0,a=1;b<=lll;b++,a--) {
    if ((y[b]>=j-10-25 && y[b]<=j+50-25) && ((x[b]>=600 && i==603) || (x[b]<=30 && i==20))) {
      xs[b]=-1*xs[b];
      if (mode)      if (xs[b]<8.8) xs[b]=xs[b]*1.03; 
      else if (xs[b]<8.8) xs[b]=xs[b]*1.03;
      if (y[b]<j+16-25 || y[b]>j+24-25) ys[b]=ys[b]+(y[b]-(j-25+20))*0.08;
      if (ys[b]>6) ys[b]=6;
      if (sound) Mix_PlayChannel(0,hit_paddle,0);
      if (i=603 && btime) {
	btime=0;
	xs[0]=xs[0]*4.4;
	xs[1]=xs[1]*4.4;
	ys[0]=ys[0]*4.4;
	ys[1]=ys[1]*4.4;xac=0;
	SDL_EnableKeyRepeat(1,1);
      }
      df=6;
      if (i==603) x[b]=598;
      else if (i==20) x[b]=33;
    }
  }
}

void Bounce2(int c) {
  int b=0;
  if ((c==1 && y[0]<28) || (c==2 && y[0]>444)) {
    if (x[b]>=ej[c]-10-16 && x[b]<=ej[c]+50-16) {
      ys[b]=-1*ys[b];
      if (ys[b]<8.8) ys[b]=ys[b]*1.02;
      if (x[b]<ej[c]+16-16 || x[b]>ej[c]+24-16) xs[b]=xs[b]+(x[b]-(ej[c]-16+20))*0.1;
      if (sound) Mix_PlayChannel(0,hit_paddle,0);
      df=6;
      if (c==1) y[0]=30;
      else if (c==2) y[0]=442;
    }
  }
}

void Bounce3(int j,int i6);
void Bounce3(int j,int i6) {
  int b,a;
  for (b=0,a=1;b<=1;b++,a--) {
    if ((x[b]>=j-10-25 && x[b]<=j+50-25) && (y[b]>=440)) {
      ys[b]=-1*ys[b];
      if (x[b]<j+16-25 || x[b]>j+24-25) xs[b]=xs[b]+(x[b]-(j-25+20))*0.08;
      if (xs[b]>6) xs[b]=6;
      if (sound) Mix_PlayChannel(0,hit_paddle,0);
      y[b]=439;
      bounced=bounced+1; grav=grav+0.002;
    }
  }
}


void MoveBall(int k);
void MoveBall(int k) {
  xinc=xs[k]/5;
  yinc=ys[k]/5;
  xinc2=xs[1]/5;
  yinc2=ys[1]/5;
  for (df=1; df<=5; df++) {
    x[k]=x[k]+xinc;
    y[k]=y[k]+yinc;
    x[1]=x[1]+xinc2;
    y[1]=y[1]+yinc2;
    if (mode != 10 || server==1) {
      Bounce(603,j);
      Bounce(20,ej[0]);
      if (mode != 4) BounceBalls();
    }
    if (mode==4) {
      Bounce2(2);
      Bounce2(1);
    }
    win_check(k);
    if (mode != 4) win_check(1);
  }
  DrawBall(screen,j-25,ej[0]-25);
}

void DrawBall(SDL_Surface *screen, int i, int j) {
  char str[10]; int s;
  SDL_FillRect(screen,NULL,0x000000);
  DrawIMG(back, 0, 0);
  DrawIMG(back3, 0,0);
  DrawIMG(back2, 0, 467);
  DrawIMG(back4, 625, 0);
  DrawIMG(midring,252,173);
  if (mode!=4)  DrawIMG(back5, 315, 0); 
  if (btime) {
    xa[xac][0]=x[0];
    xb[xac][0]=x[1];
    xa[xac][1]=y[0];
    xb[xac][1]=y[1];
    int ll;
    for (ll=0;ll<=xac;ll+=3) {
      DrawIMG(ring,xa[ll][0],xa[ll][1]-6);
      if (mode!=4)  DrawIMG(ring,xb[ll][0],xb[ll][1]-6);
    }
  }
  DrawIMG(ball,x[0],y[0]);
  if (mode!=4)  DrawIMG(ball2,x[1],y[1]);
  if (mode==4) { 
    DrawIMG(paddle4,ej[1]-16,20);
    DrawIMG(paddle3,ej[2]-16,450);
  }
  else if (mode == 0) DrawIMG(paddle5,mi-25,450);
  if ((mode != 10 || server ==1) && mode) {
    DrawIMG(paddle,609,i);
    DrawIMG(paddle2,20,j);
  }
  else if (server==2) {
    DrawIMG(paddle,610,j);
    DrawIMG(paddle2,20,i);
  }
  if (mode) {
    s = sprintf(str, "%d", scorep);
    PutStringFont            (screen,Font3,  338, 20, str);
    s = sprintf(str, "%d", scoree);
    if (scoree>=10)  PutStringFont            (screen,Font3,  265, 20, str);
    else PutStringFont            (screen,Font3,  278, 20, str);
    if (bav>0) {
      PutStringFont            (screen,Font3,  18, 20, "BT:");
      s = sprintf(str, "%d", bav);
      PutStringFont            (screen,Font3,  81, 20, str);
    }
  }
  else {
    s = sprintf(str, "%d", bounced);
    PutStringFont            (screen, Font3, 338, 20, str);
  }
  SDL_Flip(screen);
}

void DrawIMG(SDL_Surface *img, int x, int y)
{
  SDL_Rect dest;
  dest.x = x;
  dest.y = y;
  SDL_BlitSurface(img, NULL, screen, &dest);
}

int main(int argc, char *argv[])
{ 
  icon = png_load("Graphics/2pongicon.png");
  SDL_SetColorKey(icon, SDL_SRCCOLORKEY, 0);
  int a,b,i,done=0,start=0,menu=1,levels=0;
  if ( SDL_Init(SDL_INIT_AUDIO|SDL_INIT_VIDEO|SDL_INIT_TIMER) < 0 )
  {
    printf("Unable to init SDL: %s\n", SDL_GetError());
    exit(1);
  }
  ta=SDL_GetTicks();
  atexit(SDL_Quit);
  back = SDL_LoadBMP("Graphics/back1.bmp");
  back2 = SDL_LoadBMP("Graphics/back2.bmp");
  back3 = SDL_LoadBMP("Graphics/back3.bmp");
  back4 = SDL_LoadBMP("Graphics/back4.bmp");
  back5 = SDL_LoadBMP("Graphics/back5.bmp");
  ball = SDL_LoadBMP("Graphics/ball.bmp");
  ball2 = SDL_LoadBMP("Graphics/ball2.bmp");
  paddle = SDL_LoadBMP("Graphics/paddle.bmp");
  paddle2 = SDL_LoadBMP("Graphics/paddleo.bmp");
  paddle3 = SDL_LoadBMP("Graphics/paddle2.bmp");
  paddle4 = SDL_LoadBMP("Graphics/paddle2o.bmp");
  paddle5 = SDL_LoadBMP("Graphics/paddle5.bmp");
  midring = SDL_LoadBMP("Graphics/midring.bmp");
  ring = SDL_LoadBMP("Graphics/ring.bmp");
  SDL_SetColorKey(ring, SDL_SRCCOLORKEY,
                           SDL_MapRGB(ring->format, 0, 0, 0));
  SDL_SetColorKey(back5, SDL_SRCCOLORKEY,
                           SDL_MapRGB(back5->format, 0, 0, 0));
  SDL_SetColorKey(paddle, SDL_SRCCOLORKEY,
                           SDL_MapRGB(paddle->format, 0, 0, 0));
  SDL_SetColorKey(paddle2, SDL_SRCCOLORKEY,
                           SDL_MapRGB(paddle2->format, 0, 0, 0));
  SDL_SetColorKey(paddle3, SDL_SRCCOLORKEY,
                           SDL_MapRGB(paddle3->format, 0, 0, 0));
  SDL_SetColorKey(paddle4, SDL_SRCCOLORKEY,
                           SDL_MapRGB(paddle4->format, 0, 0, 0));
  SDL_SetColorKey(paddle5, SDL_SRCCOLORKEY,
                           SDL_MapRGB(paddle5->format, 0, 0, 0));
  SDL_SetColorKey(ball, SDL_SRCCOLORKEY,
                           SDL_MapRGB(ball->format, 0,0,0));
  SDL_SetColorKey(ball2, SDL_SRCCOLORKEY,
                           SDL_MapRGB(ball2->format, 0,0,0));
  SDL_WM_SetIcon(icon,NULL);
  screen=SDL_SetVideoMode(640,480,32,SDL_HWSURFACE|SDL_DOUBLEBUF);
  if ( screen == NULL )
  {
    printf("Unable to set 640x480 video: %s\n", SDL_GetError());
    exit(1);
  }
  SDL_WM_SetCaption(VERSION, VERSION);
  if (sound)  if(Mix_OpenAudio(44100, AUDIO_S16SYS, 2, 2048) < 0) printf("Warning: Couldn't set 44100 Hz 16-bit audio - Reason: %s\n", SDL_GetError());
  Font1 = LoadFont("Graphics/font01.png");
  Font2 = SetFontColor(Font1,225,225,225);
  Font3 = LoadFont("Graphics/font02.bmp");
  Font4 = SetFontColor(Font3,155,155,155);
  SetCurrentFont(Font3);
  if (sound) {
    hit_paddle=Mix_LoadWAV(PATH "Sound/hit.wav");
    hit_wall=Mix_LoadWAV("Sound/hit2.wav");
    win_snd=Mix_LoadWAV("Sound/win.wav");
    lose_snd=Mix_LoadWAV("Sound/lose.wav");
    menu_snd=Mix_LoadWAV("Sound/menu_snd.wav");
    click_snd=Mix_LoadWAV("Sound/click.wav");
  }
  SDL_Flip(screen);
  if (argc==3) {
    PORT=argv[2];
    server=1;
    start_server();
    if (recvfrom(s, buf, BUFLEN, 0, &si_other, &slen)==-1) diep("recvfrom()");
    start=1; playing=1; mode=10; menu=1; levels=0;
  }
  else if (argc==4) {
    SRV_IP=argv[2];
    server=2;
    PORT=argv[3];
    server=2;
    xs[0]=0;
    connect_server();
    sprintf(buf, "%d", j);
    if (sendto(s, buf, BUFLEN, 0, &si_other, slen)==-1) diep("sendto()");
    start=1; playing=1; mode=10; menu=1; levels=0;
  }
  else {
    server=0;
  }
  SDL_AddTimer(10, callback, 7);
  while(done == 0)
  {

    if ((SDL_GetAppState()<7 && !ppause) && start) SDL_WarpMouse(i,j);
    if (!start) SDL_ShowCursor(1);
    else if (start && !ppause) SDL_ShowCursor(0);
    SDL_Event event;
    while ( SDL_PollEvent(&event) )
    {
      if ( event.type == SDL_QUIT ) {
        SDL_FillRect(screen,NULL,0x000000);
        PutString (screen,  310, 230, "BYE");  SDL_Flip(screen); SDL_Delay(500);
        done=1;
      }
      if ( event.type == SDL_KEYDOWN  || event.type == SDL_MOUSEBUTTONDOWN)
      {
        if ( event.key.keysym.sym == SDLK_ESCAPE || event.key.keysym.sym == SDLK_q) { 
          if (server==1) {
            if (!sr) if (recvfrom(s, buf, BUFLEN, 0, &si_other, &slen)==-1) diep("recvfrom()");
            sprintf(buf, "%d", -666);
            if (sendto(s, buf, BUFLEN, 0, &si_other, slen)==-1)
            diep("sendto()");
            close(s);
          }
          else if (server==2) {
            sprintf(buf, "%d", -666);
            if (sendto(s, buf, BUFLEN, 0, &si_other, slen)==-1) diep("sendto()");
            close(s);
          }
          else if (start || playing || levels) {
            levels=0;
            menu=1; SDL_EnableKeyRepeat(0,0);
            if (start) { start=0; playing=0; scoree=0; scorep=0; SDL_EnableKeyRepeat(0,0); }
          }
          else if (!levels && !start && !playing) {
            SDL_FillRect(screen,NULL,0x000000);
            PutString (screen,  310, 230, "BYE");  SDL_Flip(screen); SDL_Delay(500);
            done = 1;
          }
        }
        if ( event.key.keysym.sym == SDLK_p ) {
          if (!ppause && start) {
            (void) time(&stp);
            start=0; SDL_EnableKeyRepeat(0,0);
            ppause=1;
         }
          else if (ppause==1 && !start) {  
            timerp2=timerp2+timerp;
            ppause=0; SDL_EnableKeyRepeat(1,1);
            start=1; 
          }
        }
        if ( event.key.keysym.sym == SDLK_s ) {
          if (sound && spress) { sound=0; PutString (screen,  230, 230, "SOUND: OFF"); SDL_Flip(screen); SDL_Delay(400);  spress=0;  SDL_AddTimer(1000, callback2, 7); }
          else if (spress) { sound=1; PutString (screen,  230, 230, "SOUND: ON");  SDL_Flip(screen); SDL_Delay(400);  spress=0; SDL_AddTimer(1000, callback2,7);}
        }
        if ( event.key.keysym.sym == SDLK_b ) {
          if (!btime && mode!=10 && mode!=0 && bav>0) {
	    bav--;
	    btime=1;
	    int xf=3;
	    if (!mode) xf=5;
	    xs[0]=xs[0]/xf;
	    xs[1]=xs[1]/xf;
	    ys[0]=ys[0]/xf;
	    ys[1]=ys[1]/xf;
	    SDL_EnableKeyRepeat(0,0);
	  }
        }
        if ( event.key.keysym.sym == SDLK_f ) {
          if (full && spress) { full=0; SDL_WM_ToggleFullScreen(screen); PutString (screen,  180, 230, "FULL SCREEN: OFF"); SDL_Flip(screen); SDL_Delay(1500);  spress=0;  SDL_AddTimer(1000, callback2,7); }
          else if (spress) { full=1; SDL_WM_ToggleFullScreen(screen); PutString (screen,  180, 230, "FULL SCREEN: ON");  SDL_Flip(screen); SDL_Delay(1500);  spress=0; SDL_AddTimer(1000,callback2,7); SDL_WarpMouse(i,j); }
        }
        if ( event.key.keysym.sym == SDLK_r ) {
          if (start) {
             PutString (screen,  180, 230, "RESTARTING GAME");  SDL_Flip(screen); SDL_Delay(500);
             count=1; start=0; (void) time(&stc); timerc=0; scoree=0; scorep=0;
            (void) time(&st); init(j);   SDL_EnableKeyRepeat(1,1); btime=0;
          }
        }
        if (( event.key.keysym.sym == SDLK_SPACE || event.key.keysym.sym == SDLK_KP_ENTER || event.key.keysym.sym == SDLK_RETURN || event.type == SDL_MOUSEBUTTONDOWN) && !ppause && !count) {
	  if (menu==4 && !levels && !start && !playing && !count) {
            SDL_FillRect(screen,NULL,0x000000);
            PutString (screen,  310, 230, "BYE");  SDL_Flip(screen); SDL_Delay(500);
            done=1;
          }
          if (levels || (menu!=1 && !levels && !start && !playing && !count)) {
            count=1; start=0; (void) time(&stc); timerc=0; btime=0;
            (void) time(&st); SDL_EnableKeyRepeat(1,1); bounced=0;
            if (sound) Mix_PlayChannel(0,click_snd,0); btc=0; bav=0;
            if (levels) { mode=menu; levels=0; if (menu==4) mode=5; levels=0; }
            else if (menu==2) mode=0;
            else if (menu==3) mode=4;
            init(j);
          }
          else if (!levels && menu==1 && !start && !playing && !count) { levels=1;  SDL_EnableKeyRepeat(0,0); if (sound) Mix_PlayChannel(0,click_snd,0); }
        }
        if ( event.key.keysym.sym == SDLK_DOWN ) {
          if (playing) j=j+7;
          if (!playing && !start) if (sound) Mix_PlayChannel(0,menu_snd,0);
          if (menu==4) menu=1;
          else menu+=1;
        }
        if ( event.key.keysym.sym == SDLK_UP ) {
          if (playing) j=j-7;
          if (!playing && !start) if (sound) Mix_PlayChannel(0,menu_snd,0);
          if (menu==1) menu=4;
          else menu-=1;
        }
      }
    } 
    if ((!start && !ppause) && !count) {
      SDL_FillRect(screen,NULL,0x000000);
      PutStringFont            (screen,Font3,  50, 50, VERSION);
      if (!levels) {
        PutStringFont            (screen,Font3,  130, 130, "Choose A Mode:");
        if (menu==1) PutStringFont            (screen, Font4, 200, 200, "Regular.");
        else PutStringFont            (screen,  Font3,200, 200, "Regular.");
        if (menu==2) PutStringFont            (screen, Font4, 200, 240, "Kickups.");
        else PutStringFont            (screen, Font3, 200, 240, "Kickups.");
        if (menu==3) PutStringFont            (screen, Font4, 200, 280, "2 vs 2.");
        else PutStringFont            (screen, Font3, 200, 280, "2 vs 2.");
        if (menu==4) PutStringFont            (screen, Font4, 200, 320, "Exit.");
        else PutStringFont            (screen, Font3, 200, 320, "Exit.");
      }
      else {
        PutStringFont            (screen,Font3,  130, 130, "Choose A Level:");
        if (menu==1) PutStringFont            (screen, Font4, 200, 200, "Easy.");
        else PutStringFont            (screen,  Font3,200, 200, "Easy.");
        if (menu==2) PutStringFont            (screen, Font4, 200, 240, "Normal.");
        else PutStringFont            (screen, Font3, 200, 240, "Normal.");
        if (menu==3) PutStringFont            (screen, Font4, 200, 280, "Hard.");
        else PutStringFont            (screen, Font3, 200, 280, "Hard.");
        if (menu==4) PutStringFont            (screen, Font4, 200, 320, "Harder.");
        else PutStringFont            (screen, Font3, 200, 320, "Harder.");
      }
      SDL_Flip(screen);
    }
    else if (!start && ppause) {
      (void) time(&nowp);
      timerp=(int) nowp-stp;
      SDL_FillRect(screen,NULL,0x000000);
      PutString            (screen,  50, 50, VERSION);
      PutStringFont            (screen, Font4, 200, 200, "Paused.");
      SDL_Flip(screen);
    }
    else if (count) {
      SDL_FillRect(screen,NULL,0x000000);
      DrawBall(screen,j-25,ej[0]-25);
      (void) time(&nowc);
      timerc=(int) nowc-stc;
      if (timerc>=4) {start=1; count=0; (void) time(&st); playing=1;}
      if (timerc==3) PutStringFont            (screen, Font3, 302, 196, "1..");
      if (timerc==2) PutStringFont            (screen, Font3, 302, 196, "2..");
      if (timerc==1) PutStringFont            (screen, Font3, 302, 196, "3..");
      SDL_UpdateRect(screen, 0, 0, 0, 0);
    }
    SDL_GetMouseState(&mi, &mj);
    i=mi;
    if (mj != mj2) { j=mj; mj2=mj; }
    if ((i>=200 && j>=200) && (i<=370 && j<=230) && menu!=1 && !playing && !start) { if (sound) Mix_PlayChannel(0,menu_snd,0); menu=1; }
    if ((i>=200 && j>=240) && (i<=370 && j<=270) && menu!=2 && !playing && !start) { if (sound) Mix_PlayChannel(0,menu_snd,0); menu=2; }
    if ((i>=200 && j>=280) && (i<=370 && j<=310) && menu!=3 && !playing && !start) { if (sound) Mix_PlayChannel(0,menu_snd,0); menu=3; }
    if ((i>=200 && j>=320) && (i<=370 && j<=350) && menu!=4 && !playing && !start) { if (sound) Mix_PlayChannel(0,menu_snd,0); menu=4; }
    if (j<37) j=37;
    if (j>444) j=444;
    if (mi<37) mi=37;
    if (mi>604) mi=604;
    if (mode) {
     if (ej[0]<40) ej[0]=40;
     if (ej[0]>439) ej[0]=439; 
    }
    if (mode == 10 && timern!=timern2) {
      char *sep;
      timern=timern2;
      if (server==1) {
        if (!sr) {
          if (recvfrom(s, buf, BUFLEN, 0, &si_other, &slen)==-1) diep("recvfrom()");
          ej[0]=atoi(buf);
          if (ej[0]==-666) {
            close(s);
            done = 1;
          }
          sr=1;
        }
        else {
          sprintf(buf, "%d %f %f %f %f %f %f %f %f %d %d", j,x[0],x[1],y[0],y[1],xs[0],xs[1],ys[0],ys[1],scoree,scorep);
          if (sendto(s, buf, BUFLEN, 0, &si_other, slen)==-1) diep("sendto()");
          sr=0;
        } 
      }
      else {
        if (!sr) {
          sprintf(buf, "%d", j);
          if (sendto(s, buf, BUFLEN, 0, &si_other, slen)==-1) diep("sendto()");
          sr=1;
        }
        else {
          if (recvfrom(s, buf, BUFLEN, 0, &si_other, &slen)==-1) diep("recvfrom()");
          sep=strtok(buf," ");
          ej[0]=atoi(sep);
          ej[0]=atoi(buf);
          if (ej[0]==-666) {
            close(s);
            done = 1;
          }
          sep=strtok(NULL," ");
          x[0]=atoi(sep);
          sep=strtok(NULL," ");
          x[1]=atof(sep);
          sep=strtok(NULL," ");
          y[0]=atof(sep);
          sep=strtok(NULL," ");
          y[1]=atof(sep);
          sep=strtok(NULL," ");
          xs[0]=atoi(sep);
          sep=strtok(NULL," ");
          xs[1]=atof(sep);
          sep=strtok(NULL," ");
          ys[0]=atof(sep);
          sep=strtok(NULL," ");
          ys[1]=atof(sep);
          sep=strtok(NULL," ");
          scoree=atof(sep);
          sep=strtok(NULL," ");
          scorep=atof(sep);
          sr=0;
        }
      }
      DrawBall(screen,j-25,ej[0]-25);
    }
    else if (mode == 10) {
      if (server == 1) {
        if (x[0]<x[1] && x[0]<0) es[0]=y[0]-ej[0];
        else if (x[1]<x[0] && x[1]<0) es[0]=y[1]-ej[0];
        else if (x[0]<0) es[0]=y[0]-ej[0];
        else if (x[1]<0) es[0]=y[0]-ej[1];
        if (es[0]>5) es[0]=5;
        if (es[0]<-5) es[0]=-5;
        ej[0]=ej[0]+es[0];
        DrawBall(screen,j-25,ej[0]-25);
      }
      else if (xs[0]) {
        x[0]=x[0]+xs[0];
        x[1]=x[1]+xs[1];
        y[0]=y[0]+ys[0];
        y[1]=y[1]+ys[1];
        DrawBall(screen,j-25,ej[0]-25);
      } 
    }
    if (((playing && start) && !ppause) && server != 2) { 
      (void) time(&now);
      timer=(int) now-st-timerp2;
      if (mode != 4 && mode != 10 && mode)      {
        for (b=0,a=1;b<=1;b++,a--) 
          if (xs[b]<0 && (x[b]<x[a] || xs[a]>0)) {
            if (mode==5) {
             es[0]=ai(ej[0],b);
            }
            else {
              es[0]=y[b]-ej[0];
              int mx=2.5+2*mode;
              if (es[0]>mx) es[0]=mx;
              else if ((es[0]<-1*mx)) es[0]=-1*mx;
            }
          }
        ej[0]=ej[0]+es[0];
      }
      else if (mode != 10) {
        for (b=0;b<=2;b++) {
          if (b==0) es[b]=y[0]-ej[b];
          else es[b]=x[0]-ej[b];
          if (!mode && b==0) es[b]=x[1]-ej[b];
          int mxs=7.8;
          if (es[b]>mxs) es[b]=mxs;
          else if (es[b]<-1*mxs) es[b]=-1*mxs;
          if (mode) if (((b==1 && ys[0]>0) || (b==2 && ys[0]<0)) || (b==0 && xs[0]>0)) es[b]=es[b]/4;
          if (btime) es[b]=es[b]/3;
          ej[b]=ej[b]+es[b];
          if (mode) {
            if (b==0 && ej[b]>445) ej[b]=445;
            else if (ej[b]>580) ej[b]=580;
            else if (ej[b]<35) ej[b]=35;
          }
        }
      }
      if (mode==4) l=0;
      else l=1;
      for (b=0;b<=l;b++) {
        if (playing) {
          if ((y[b]<11 || y[b]>459) && mode!=4 && mode)  {ys[b]=-1*ys[b]; if (sound) Mix_PlayChannel(0,hit_wall,0);}
          else if ((x[b]<11 || x[b]>629) && !mode)  {xs[b]=-1*xs[b]; if (sound) Mix_PlayChannel(0,hit_wall,0);}
          else if (b==l && mode == 0) Bounce3(mi,603);
        }
        if (mode && !b) MoveBall(b);
        else if (!mode) { 
          x[b]=x[b]+xs[b];
	  if (ys[b]>9)
	  ys[b]=9; 
	  if (ys[b]<-9) ys[b]=-9; 
	  if (!btime) ys[b]=ys[b]+grav; 
	  else ys[b]=ys[b]+0.02; y[b]=y[b]+ys[b];
          win_check(b);
          if (b) DrawBall(screen,j-25,ej[0]-25);
	}
      }
      if (btime)  xac=xac+1;
    }
  }
  return 0;
}
