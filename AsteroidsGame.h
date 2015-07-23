#pragma once

#include <vector>
#include <tuple>
#include <random>
#include <algorithm>
#include <sstream>
#include <iostream>
#include <ctime>
#include <memory>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

#include "Helpers.h"
#include "Sprite.h"
#include "AnimatedSprite.h"
#include "Text.h"
#include "GameState.h"
#include "StartScreen.h"
#include "FrameAnimation.h"
#include "Spaceship.h"

class AsteroidsGame: public GameState{
    public:
        ~AsteroidsGame(){
            delete scoreText;
            delete highScoreText;
            delete spaceship;
            
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

            spaceship = new Spaceship();
            
            asteroidSpritePaths.push_back("Sprites/asteroid1.png");
            asteroidSpritePaths.push_back("Sprites/asteroid2.png");
            asteroidSpritePaths.push_back("Sprites/asteroid3.png");
            asteroidSpritePaths.push_back("Sprites/Meteors/meteorBrown0.png");
            asteroidSpritePaths.push_back("Sprites/Meteors/meteorBrown1.png");
            asteroidSpritePaths.push_back("Sprites/Meteors/meteorBrown2.png");
            asteroidSpritePaths.push_back("Sprites/Meteors/meteorBrown3.png");
            asteroidSpritePaths.push_back("Sprites/Meteors/meteorBrown4.png");
            asteroidSpritePaths.push_back("Sprites/Meteors/meteorBrown5.png");
            asteroidSpritePaths.push_back("Sprites/Meteors/meteorBrown6.png");
            asteroidSpritePaths.push_back("Sprites/Meteors/meteorBrown7.png");

            SoundStore::LoadSoundIfNotLoaded("Sounds/laserLoud.wav", "laser1");
            SoundStore::LoadSoundIfNotLoaded("Sounds/explosion1.wav", "explosion");
            SoundStore::LoadMusicIfNotLoaded("Sounds/hb_starter_fantasy_battle.ogg", "background");

            highscore = Helpers::getHighScore();

            scoreText = new Text("Fonts/Beon-Regular.ttf", 17);
            highScoreText = new Text("Fonts/Beon-Regular.ttf", 17);
        }

        static bool outsideScreen(std::pair<short, short> coords){
            if (coords.first<0 || coords.first>800 || coords.second<0 || coords.second>600){
                return true;
            }else{
                return false;
            }
        }

        static bool spriteOutsideScreen(std::shared_ptr<Sprite> spritePtr){
            Sprite *sprite = spritePtr.get();
            return outsideScreen(std::make_pair(sprite->position.first, sprite->position.second));
        }

        void HandleEvent(SDL_Event *e){
            if (e->type == SDL_MOUSEMOTION){
                int x, y;
                SDL_GetMouseState(&x, &y);

                spaceship->SetPosition((x-spaceship->width/2), 500);
            }

            if (e->type == SDL_MOUSEBUTTONDOWN && !hit){
                score -= 25;

                int offset;
                if (e->button.button == SDL_BUTTON_LEFT){
                    if (spaceship->leftGunCharge < 430){
                        offset = -10;
                        spaceship->leftGunCharge += 150;
                    }else{
                        offset = 0;
                    }
                }else{
                    if (spaceship->rightGunCharge < 430){
                        offset = 35;
                        spaceship->rightGunCharge += 150;
                    }else{
                        offset = 0;
                    }
                }

                if (offset != 0){
                    Sprite *laser = new Sprite("Sprites/laserRed09.png", spaceship->position.first+offset, 510);
                    bullets.push_back(std::shared_ptr<Sprite> (laser));
                    Mix_PlayChannel(-1, SoundStore::GetSound("laser1"), 0);
                }
            }

            if (e->type == SDL_KEYUP && e->key.keysym.sym == SDLK_r){
                states->push_back(new StartScreen());
                gotoNext = true;
            }
        }

        void Update(double dt){
            frame++;

            if (!Mix_PlayingMusic()){
                Mix_PlayMusic(SoundStore::GetMusic("background"), -1);
            }

            score += dt * 30;
            if (frame % 15 == 0 && !hit){
                if (score > highscore){
                    highscore = (int)score;
                }

                std::ostringstream scoreStream;
                scoreStream << "Score: " << (int)score;
                scoreText->SetText(scoreStream.str());

                std::ostringstream highScoreStream;
                highScoreStream << "High-score: " << highscore;
                highScoreText->SetText(highScoreStream.str());
            }

            if (frame % 8 == 0 && !hit){
                stars.erase(std::remove_if(stars.begin(), stars.end(), AsteroidsGame::outsideScreen), stars.end()); // Remove old stars.

                int star_pos = rng()%800;
                stars.push_back(std::make_pair(star_pos, 0));
            }

            for (int i=0;i<stars.size();i++){
                if (!hit){
                    stars[i].second += dt * 200;
                }
            }

            for (int i=0;i<bullets.size();i++){
                bullets[i].get()->position.second -= dt * 400;
            }

            if (frame % 8 == 0){
                bullets.erase(std::remove_if(bullets.begin(), bullets.end(), AsteroidsGame::spriteOutsideScreen), bullets.end());
            }

            if (frame % 120 == 0 && !hit){
                asteroids.erase(std::remove_if(asteroids.begin(), asteroids.end(), AsteroidsGame::spriteOutsideScreen), asteroids.end()); // Remove old asteroids.

                int asteroidPos = rng() % 800;
                Sprite *asteroid = new Sprite(asteroidSpritePaths[rng()%asteroidSpritePaths.size()], asteroidPos, -40);
                asteroids.push_back(std::shared_ptr<Sprite> (asteroid));
            }

            for (int i=0;i<asteroids.size();i++){
                if (!hit){
                    asteroids[i]->position.second += dt * 400;
                }

                if (spaceship->collidesWith(asteroids[i].get()) && !hit){
                    hit = true;
                    spaceship->explosion->Start();
                    states->push_back(new StartScreen());
                    states->push_back(new AsteroidsGame());
                    Mix_PlayChannel(-1, SoundStore::GetSound("explosion"), 0);
                }

                for (int i2=0;i2<bullets.size();i2++){
                    if (bullets[i2].get()->collidesWith(asteroids[i].get())){
                        asteroids[i].get()->position.second += 900;
                        bullets[i2].get()->position.second += 900;
                        score += 60;
                    }
                }

                if (hit && spaceship->explosion->done){
                    stateFinished = true;
                }
            }

            spaceship->Update(dt);
        }

        void Draw(){
            SDL_FillRect(Helpers::surface, NULL, SDL_MapRGB(Helpers::surface->format, 0, 0, 0));
            for (int i=0;i<stars.size();i++){
                SDL_Rect star_rect;
                star_rect.x = stars[i].first;
                star_rect.y = stars[i].second;
                star_rect.w = 4;
                star_rect.h = 4;
                SDL_FillRect(Helpers::surface, &star_rect, SDL_MapRGB(Helpers::surface->format, 255, 255, 255));
            }
            
            for (int i=0;i<asteroids.size();i++){
                asteroids[i].get()->Draw();
            }

            for (int i=0;i<bullets.size();i++){
                bullets[i].get()->Draw();
            }

            spaceship->Draw();

            scoreText->Draw(5, 5);
            highScoreText->Draw(5, 25);
        }
        std::vector<std::pair<double, double>> stars;
        std::vector<std::shared_ptr<Sprite>> asteroids;
        std::vector<std::shared_ptr<Sprite>> bullets;
        std::vector<std::string> asteroidSpritePaths;
        std::default_random_engine rng;
        Spaceship *spaceship;
        std::vector<SDL_Surface*> asteroidSprites;
        Text *scoreText;
        Text *highScoreText;
        long frame;
        double score;
        long highscore;
        bool hit;
};
