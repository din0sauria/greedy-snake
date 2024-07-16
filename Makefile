CC=g++
CXX=g++
CPPFLAGES=-O2 -pthread -pipe -std=c++11 -Wall
LDFLAGS=-std=c++11

computer:
	$(CC) $(CPPFLAGES) -o bin/$@ src/main_computer.c lib/libplayer.a

player:
	$(CC) $(CPPFLAGES) -o bin/$@ src/main_player.c lib/libplayer.a

check_%:
	$(CC) $(CPPFLAGES) -o bin/$@ src/$@.c lib/libplayer.a
