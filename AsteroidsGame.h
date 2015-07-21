#pragma once

#include <vector>
#include <tuple>
#include <random>
#include <algorithm>
#include <sstream>
#include <iostream>
#include <ctime>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

#include "Helpers.h"
#include "Sprite.h"
#include "GameState.h"
#include "StartScreen.h"
#include "FrameAnimation.h"

class AsteroidsGame: public GameState{
    public:
        ~AsteroidsGame(){
            delete explosion;
            delete flame;
            SDL_FreeSurface(scoreText);
            
            for (int i=0;i<asteroidSprites.size();i++){
                SDL_FreeSurface(asteroidSprites[i]);
            }

            Helpers::setHighScore(highscore);
        }

        void InitState(){
            frame = 0;
            score = 0;
            
            hit = false;

            rng.seed(time(NULL));

            spaceship = new Sprite("Sprites/spaceship2.png", 400, 500);
            spaceship->SetSurface(surface);
            
            asteroidSpritePaths.push_back("Sprites/asteroid1.png");
            asteroidSpritePaths.push_back("Sprites/asteroid2.png");
            asteroidSpritePaths.push_back("Sprites/asteroid3.png");

            scoreFont = TTF_OpenFont("Fonts/Beon-Regular.ttf", 17);
            scoreColor = {255, 255, 255};

            explosionSounds.push_back(Mix_LoadWAV("Sounds/explosion1.wav"));
            backMusic = Mix_LoadMUS("Sounds/background1.wav");

            if (!Mix_PlayingMusic()){
                Mix_PlayMusic(backMusic, -1);
            }

            explosion = new FrameAnimation("Sprites/Explosion/explosion", 23, 1, 12);
            flame = new FrameAnimation("Sprites/Flame/flame", 4, -1, 30);
            flame->Start();

            highscore = Helpers::getHighScore();
        }

        static bool outsideScreen(std::pair<short, short> coords){
            if (coords.first<0 || coords.first>800 || coords.second<0 || coords.second>600){
                return true;
            }else{
                return false;
            }
        }

        static bool asteroidOutsideScreen(Sprite *asteroid){
            return outsideScreen(std::make_pair(asteroid->position.first, asteroid->position.second));
        }

        void HandleEvent(SDL_Event *e){
            if (e->type == SDL_MOUSEMOTION){
                int x, y;
                SDL_GetMouseState(&x, &y);

                spaceship->SetPosition((x-spaceship->width/2), 500);
            }
        }

        void Update(){
            frame++;

            if (frame % 15 == 0 && !hit){
                score++;

                if (score > highscore){
                    highscore = score;
                }

                std::ostringstream scoreStream;
                scoreStream << "Score: " << score;
                SDL_FreeSurface(scoreText);
                scoreText = TTF_RenderText_Solid(scoreFont, scoreStream.str().c_str(), scoreColor);

                std::ostringstream highScoreStream;
                highScoreStream << "High-score: " << highscore;
                SDL_FreeSurface(highScoreText);
                highScoreText = TTF_RenderText_Solid(scoreFont, highScoreStream.str().c_str(), scoreColor);
            }

            if (frame % 8 == 0 && !hit){
                stars.erase(std::remove_if(stars.begin(), stars.end(), AsteroidsGame::outsideScreen), stars.end()); // Remove old stars.

                int star_pos = rng()%800;
                stars.push_back(std::make_pair(star_pos, 0));
            }

            for (int i=0;i<stars.size();i++){
                if (!hit){
                    stars[i].second += 3;
                }
            }

            if (frame % 120 == 0 && !hit){
                asteroids.erase(std::remove_if(asteroids.begin(), asteroids.end(), AsteroidsGame::asteroidOutsideScreen), asteroids.end()); // Remove old asteroids.

                int asteroidPos = rng() % 800;
                Sprite *asteroid = new Sprite(asteroidSpritePaths[rng()%asteroidSpritePaths.size()], asteroidPos, -40);
                asteroid->SetSurface(surface);
                asteroids.push_back(asteroid);
            }

            for (int i=0;i<asteroids.size();i++){
                if (!hit){
                    asteroids[i]->position.second += 1;
                }

                if (spaceship->collidesWith(asteroids[i]) && !hit){
                    hit = true;
                    explosion->Start();
                    states->push_back(new StartScreen());
                    states->push_back(new AsteroidsGame());
                    Mix_PlayChannel(-1, explosionSounds[0], 0);
                }

                if (hit && explosion->done){
                    stateFinished = true;
                }
            }

            explosion->Update();
            flame->Update();
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
                asteroids[i]->Draw();
            }

            spaceship->Draw();
            
            explosion->Draw(surface, (spaceship->position.first-10), 500);

            flame->Draw(surface, (spaceship->position.first), 560);

            SDL_Rect scoreRect;
            scoreRect.x = 5;
            scoreRect.y = 5;
            SDL_BlitSurface(scoreText, NULL, surface, &scoreRect);

            scoreRect.y = 25;
            SDL_BlitSurface(highScoreText, NULL, surface, &scoreRect);

            SDL_UpdateWindowSurface(window);
        }
        std::vector<std::pair<short, short>> stars;
        std::vector<Sprite*> asteroids;
        std::vector<std::string> asteroidSpritePaths;
        std::vector<Mix_Chunk*> explosionSounds;
        Mix_Music *backMusic;
        std::default_random_engine rng;
        Sprite *spaceship;
        std::vector<SDL_Surface*> asteroidSprites;
        FrameAnimation *explosion;
        FrameAnimation *flame;
        long frame;
        long score;
        long highscore;
        SDL_Surface *scoreText;
        SDL_Surface *highScoreText;
        TTF_Font *scoreFont;
        SDL_Color scoreColor;
        bool hit;
};
