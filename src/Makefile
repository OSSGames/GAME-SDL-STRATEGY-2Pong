####### Compiler, tools and options

CC       = gcc
CXX      = g++
LEX      = flex
YACC     = yacc
CFLAGS   = -pipe -fno-exceptions -Wall -W -O2 -march=i486 -mcpu=i686 -D_REENTRANT
CXXFLAGS = -pipe -fno-exceptions -Wall -W -O2 -march=i486 -mcpu=i686 -D_REENTRANT
LEXFLAGS = 
YACCFLAGS= -d
INCPATH  = `xml2-config --cflags` `sdl-config --cflags`
LINK     = g++
LFLAGS   = -fno-exceptions  -Wl,-rpath,/usr/lib/qt/lib
LIBS     = `xml2-config --libs` `sdl-config --libs` -lSDL_net -lSDL_mixer
AR       = ar cqs
RANLIB   = 
COPY     = cp -f
COPY_FILE= $(COPY)
COPY_DIR = $(COPY) -r
INSTALL_FILE= $(COPY_FILE)
INSTALL_DIR = $(COPY_DIR)
DEL_FILE = rm -f
SYMLINK  = ln -sf
DEL_DIR  = rmdir
MOVE     = mv -f
CHK_DIR_EXISTS= test -d
MKDIR    = mkdir -p

####### Output directory

OBJECTS_DIR = ./

####### Files

HEADERS = 
SOURCES = defs.cpp CSpriteBase.cpp CSprite.cpp font.cpp ai.cpp physics.cpp net.cpp menu.cpp draw.cpp powerups.cpp balls.cpp paddles.cpp game.cpp main.cpp
OBJECTS = defs.o CSpriteBase.o CSprite.o font.o ai.o physics.o net.o menu.o draw.o powerups.o balls.o paddles.o game.o main.o
FORMS = 
UICDECLS = 
UICIMPLS = 
SRCMOC   = 
OBJMOC = 
DESTDIR  = 
TARGET   = 2pong

first: all
####### Implicit rules

.SUFFIXES: .c .o .cpp .cc .cxx .C

.cpp.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<

.cc.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<

.cxx.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<

.C.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<

.c.o:
	$(CC) -c $(CFLAGS) $(INCPATH) -o $@ $<

####### Build rules

all: Makefile $(TARGET)

$(TARGET):  $(UICDECLS) $(OBJECTS)
	$(LINK) $(LFLAGS) -o $(TARGET) $(OBJECTS) $(OBJCOMP) $(LIBS)
	mv $(TARGET) ../

clean:
	-$(DEL_FILE) $(OBJECTS)

####### Sub-libraries

distclean: clean
	-$(DEL_FILE) $(TARGET) $(TARGET)


FORCE:

####### Compile

main.o: main.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o main.o main.cpp

####### Install

install:  

uninstall:  

