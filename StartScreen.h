#pragma once

#include <vector>
#include <tuple>
#include <random>
#include <algorithm>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "GameState.h"
#include "Text.h"
#include "Sprite.h"

class StartScreen: public GameState{
    public:
        ~StartScreen(){
            delete logo;
            delete startText;
        }

        void InitState(){
            frame = 0;
            textPosition = 0;
            textDown = false;

            SoundStore::LoadMusicIfNotLoaded("Sounds/inspirational_ftl.ogg", "menumusic");
            if (!Helpers::menuMusicPlayed){
                Mix_PlayMusic(SoundStore::GetMusic("menumusic"), -1);
            }

            startText = new Text("Fonts/Amburegul.ttf", 24);
            startText->SetText("Press ENTER to start the game");

            logo = new Sprite("Sprites/logo1.png");
            logo->SetPosition(400-logo->width/2, 150-logo->height/2);
        }
        
        static bool outsideScreen(std::pair<short, short> coords){
            if (coords.first<0 || coords.first>800 || coords.second<0 || coords.second>600){
                return true;
            }else{
                return false;
            }
        }

        void HandleEvent(SDL_Event *e){
            if (e->type == SDL_KEYUP && e->key.keysym.sym == SDLK_RETURN){
                stateFinished = true;
                if (!Helpers::menuMusicPlayed){
                    Mix_FadeOutMusic(3000);
                    Helpers::menuMusicPlayed = true;
                }
            }else if (e->type == SDL_KEYUP && e->key.keysym.sym == SDLK_ESCAPE){
                quit = true;
            }
        }

        void Update(double dt){
            frame++;

            if (frame % 8 == 0){
                stars.erase(std::remove_if(stars.begin(), stars.end(), StartScreen::outsideScreen), stars.end()); // Remove old stars.

                int starPos = rng() % 800;
                stars.push_back(std::make_pair(starPos, 0));
            }

            for (int i=0;i<stars.size();i++){
                stars[i].second += dt * 500;
            }

            if (textDown){
                textPosition += dt * 130;
                if (textPosition >= 40){
                    textDown = false;
                }
            }else{
                textPosition -= dt * 130;
                if (textPosition <= 0){
                    textDown = true;
                }
            }
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

            logo->Draw();
            startText->Draw(170, 300 + textPosition);
        }

        std::vector<std::pair<double, double>> stars;
        std::default_random_engine rng;
        Text *startText;
        Sprite *logo;
        long frame;
        double textPosition;
        bool textDown;
};
