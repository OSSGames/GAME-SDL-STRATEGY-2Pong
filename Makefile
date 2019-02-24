CFLAGS = $(shell sdl-config --cflags) -Wall
CXXFLAGS = $(CFLAGS)
LIBS = -lSDL_image $(shell sdl-config --libs) -lSDL_mixer
OBJECTS = src/BFont.o
PRE="/usr/local"
PREFIX=$(PRE)/share/2Pong/

all: 2Pong


2Pong: $(OBJECTS) src/2Pong.o
	gcc $(CFLAGS) -o 2Pong $(OBJECTS) src/2Pong.o $(LIBS)


clean:
	@rm -f src/*.o src/Test* src/2Pong
install:	2Pong
	echo "#!/bin/bash" > src/2Pong
	echo "cd $(PREFIX)" >> src/2Pong
	echo "./2Pong" >> src/2Pong
	mkdir -p $(PREFIX)/Graphics/
	mkdir -p $(PREFIX)/Sound/
	install 2Pong $(PREFIX)
	install src/2Pong $(PRE)/bin
	install Graphics/* $(PREFIX)/Graphics/
	install Sound/* $(PREFIX)/Sound/

