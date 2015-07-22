compile: asteroids.cpp
	clang++ -std=c++11 `sdl2-config --cflags` -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -O2 -o asteroids asteroids.cpp

debug: asteroids.cpp
	clang++ -g -DDEBUGMODE -std=c++11 `sdl2-config --cflags` -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -O2 -o asteroids asteroids.cpp

windows: asteroids.cpp
	i686-w64-mingw32-g++ -std=c++11 -O2 -o asteroidsWin.exe asteroids.cpp -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf -mwindows -ISDLWIN/i686-w64-mingw32/include/SDL2/ -ISDLWIN/i686-w64-mingw32/include/ -LSDLWIN/i686-w64-mingw32/lib/ -static-libgcc -static-libstdc++

run:
	make compile
	./asteroids

clean:
	rm -f asteroids
	rm -f asteroidsWin.exe
