#pragma once

#include <vector>
#include <sstream>
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

        SplashScreen(){
            frame = 0;
            totalFrames = 200;
        }

        SplashScreen(std::string path, int imageCount, int totalframes) :SplashScreen(){
            this->totalFrames = totalframes;
            for (int i=0;i<imageCount;i++){
                std::ostringstream imagePath;
                imagePath << path << i << ".png";
                images.push_back(IMG_Load(imagePath.str().c_str()));
            }
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
            SDL_BlitSurface(images[currentImage], NULL, Helpers::surface, NULL);
        }

        std::vector<SDL_Surface*> images;
        int frame, totalFrames;
};
