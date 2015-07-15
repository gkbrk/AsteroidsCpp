#pragma once

#include <vector>
#include <tuple>
#include <random>
#include <algorithm>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "GameState.h"

class StartScreen: public GameState{
    public:
        void InitState(){
            frame = 0;
            textPosition = 0;
            textDown = false;

            TTF_Font *font1 = TTF_OpenFont("Fonts/Amburegul.ttf", 24);
            SDL_Color textColor = {255, 255, 255};

            start_text = TTF_RenderText_Solid(font1, "Press ENTER to start the game", textColor);
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
            }
        }

        void Update(){
            frame++;

            if (frame % 8 == 0){
                stars.erase(std::remove_if(stars.begin(), stars.end(), StartScreen::outsideScreen), stars.end()); // Remove old stars.

                int starPos = rng() % 800;
                stars.push_back(std::make_pair(starPos, 0));
            }

            for (int i=0;i<stars.size();i++){
                stars[i].second += 2;
            }

            if (frame % 5 == 0){
                if (textDown){
                    textPosition += 1;
                    if (textPosition >= 40){
                        textDown = false;
                    }
                }else{
                    textPosition -= 1;
                    if (textPosition <= 0){
                        textDown = true;
                    }
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

            SDL_Rect textRect;
            textRect.x = 170;
            textRect.y = 300 + textPosition;
            SDL_BlitSurface(start_text, NULL, surface, &textRect);
            SDL_UpdateWindowSurface(window);
        }

        std::vector<std::pair<short, short>> stars;
        std::default_random_engine rng;
        SDL_Surface *start_text;
        long frame;
        int textPosition;
        bool textDown;
};
