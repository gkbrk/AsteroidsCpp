compile:
	clang++ -std=c++11 `sdl2-config --cflags` -lSDL2 -lSDL2_image -lSDL2_ttf -O3 -o asteroids *.cpp

run:
	make compile
	./asteroids

clean:
	rm -f asteroids
