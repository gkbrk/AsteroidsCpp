#pragma once

#include <vector>
#include <tuple>
#include <random>
#include <algorithm>
#include <sstream>
#include <iostream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "GameState.h"
#include "StartScreen.h"

class AsteroidsGame: public GameState{
    public:
        void InitState(){
            frame = 0;
            score = 0;
            spaceshipPosition = 400;
            
            spaceship = IMG_Load("Sprites/spaceship2.png");
            asteroid = IMG_Load("Sprites/asteroid1.png");

            scoreFont = TTF_OpenFont("Fonts/Beon-Regular.ttf", 17);
            scoreColor = {255, 255, 255};


        }

        static bool outsideScreen(std::pair<short, short> coords){
            if (coords.first<0 || coords.first>800 || coords.second<0 || coords.second>600){
                return true;
            }else{
                return false;
            }
        }

        bool checkCollision(SDL_Rect rect1, SDL_Rect rect2){
            int leftA, leftB, rightA, rightB, topA, topB, bottomA, bottomB;

            //Sides of rect1
            leftA = rect1.x;
            rightA = rect1.x + rect1.w;
            topA = rect1.y;
            bottomA = rect1.y + rect1.h;

            //Sides of rect2
            leftB = rect2.x;
            rightB = rect2.x + rect2.w;
            topB = rect2.y;
            bottomB = rect2.y + rect2.h;

            //Detect collision

            if (bottomA <= topB || topA >= bottomB || rightA <= leftB || leftA >= rightB){
                return false;
            }else{
                return true;
            }
        }

        void HandleEvent(SDL_Event *e){
            if (e->type == SDL_MOUSEMOTION){
                int y;
                SDL_GetMouseState(&spaceshipPosition, &y);
            }
        }

        void Update(){
            frame++;

            if (frame % 15 == 0){
                score++;
                std::ostringstream scoreStream;
                scoreStream << "Score: " << score;
                scoreText = TTF_RenderText_Solid(scoreFont, scoreStream.str().c_str(), scoreColor);
            }

            if (frame % 8 == 0){
                stars.erase(std::remove_if(stars.begin(), stars.end(), AsteroidsGame::outsideScreen), stars.end()); // Remove old stars.

                int star_pos = rng()%800;
                stars.push_back(std::make_pair(star_pos, 0));
            }

            for (int i=0;i<stars.size();i++){
                stars[i].second += 3;
            }

            if (frame % 125 == 0){
                asteroids.erase(std::remove_if(asteroids.begin(), asteroids.end(), AsteroidsGame::outsideScreen), asteroids.end()); // Remove old asteroids.

                int asteroidPos = rng() % 800;
                asteroids.push_back(std::make_pair(asteroidPos, -40));
            }

            for (int i=0;i<asteroids.size();i++){
                asteroids[i].second += 1;

                SDL_Rect spaceship;
                SDL_Rect asteroid;
                spaceship.x = spaceshipPosition - 25;
                spaceship.y = 500;
                spaceship.w = 50;
                spaceship.h = 69;

                asteroid.x = asteroids[i].first;
                asteroid.y = asteroids[i].second;
                asteroid.w = 45;
                asteroid.h = 63;
                if (checkCollision(spaceship, asteroid)){
                    states->push_back(new StartScreen());
                    states->push_back(new AsteroidsGame());
                    stateFinished = true;
                }
            }
        }

        void Draw(){
            SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, 0, 0, 0));
            for (int i=0;i<stars.size();i++){
                SDL_Rect star_rect;
                star_rect.x = stars[i].first;
                star_rect.y = stars[i].second;
                star_rect.w = 4;
                star_rect.h = 4;
                SDL_FillRect(surface, &star_rect, SDL_MapRGB(surface->format, 255, 255, 255));
            }
            
            for (int i=0;i<asteroids.size();i++){
                SDL_Rect asteroid_rect;
                asteroid_rect.x = asteroids[i].first;
                asteroid_rect.y = asteroids[i].second;
                SDL_BlitSurface(asteroid, NULL, surface, &asteroid_rect);
            }

            SDL_Rect spaceship_rect;
            spaceship_rect.x = spaceshipPosition - 25;
            spaceship_rect.y = 500;
            SDL_BlitSurface(spaceship, NULL, surface, &spaceship_rect);

            SDL_Rect scoreRect;
            scoreRect.x = 5;
            scoreRect.y = 5;
            SDL_BlitSurface(scoreText, NULL, surface, &scoreRect);
            SDL_UpdateWindowSurface(window);
        }
        std::vector<std::pair<short, short>> stars;
        std::vector<std::pair<short, short>> asteroids;
        std::default_random_engine rng;
        SDL_Surface *spaceship;
        SDL_Surface *asteroid;
        long frame;
        long score;
        SDL_Surface *scoreText;
        TTF_Font *scoreFont;
        SDL_Color scoreColor;
        int spaceshipPosition;
};
