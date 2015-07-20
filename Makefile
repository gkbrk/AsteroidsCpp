compile: asteroids.cpp
	clang++ -std=c++11 `sdl2-config --cflags` -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -O3 -o asteroids asteroids.cpp

run:
	make compile
	./asteroids

clean:
	rm -f asteroids
