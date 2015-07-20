#pragma once

#include <vector>
#include <iostream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "GameState.h"

class SplashScreen: public GameState{
    public:
        ~SplashScreen(){
            for (int i=0;i<images.size();i++){
                SDL_FreeSurface(images[i]);
            }
        }

        void InitState(){
            frame = 0;
            totalFrames = 250;

            images.push_back(IMG_Load("SplashScreen/0.png"));
            images.push_back(IMG_Load("SplashScreen/1.png"));
            images.push_back(IMG_Load("SplashScreen/2.png"));
        }

        void Update(){
            frame++;
            if ((frame+1)/(totalFrames/images.size()) >= images.size()){
                stateFinished = true;
            }
        }

        void Draw(){
            int timePerPicture = totalFrames / images.size();
            int currentImage = frame/timePerPicture;
            int alpha = (float)(frame%timePerPicture)/timePerPicture*255;

            SDL_SetSurfaceBlendMode(images[currentImage], SDL_BLENDMODE_BLEND);
            SDL_SetSurfaceAlphaMod(images[currentImage], alpha);
            SDL_BlitSurface(images[currentImage], NULL, surface, NULL);
            SDL_UpdateWindowSurface(window);
        }

        std::vector<SDL_Surface*> images;
        int frame, totalFrames;
};
