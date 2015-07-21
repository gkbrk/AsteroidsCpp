#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <vector>
#include <sstream>

#include "Helpers.h"

using namespace Helpers;

class AnimatedSprite: public Sprite{
    public:
        AnimatedSprite(std::string path, int frameCount, int updatesPerFrame){
            frame = 0;
            currentFrame = 0;
            playing = true;
            
            this->updatesPerFrame = updatesPerFrame;

            for (int i=0;i<frameCount;i++){
                std::ostringstream pathStream;
                pathStream << path << "-" << i << ".png";
                frames.push_back(IMG_Load(pathStream.str().c_str()));
            }

            width = frames[0]->w;
            height = frames[0]->h;
        }

        AnimatedSprite(std::string path, int frameCount, int updatesPerFrame, int x, int y): AnimatedSprite(path, frameCount, updatesPerFrame){
            position.first = x;
            position.second = y;
        }

        ~AnimatedSprite(){
            for (int i=0;i<frames.size();i++){
                SDL_FreeSurface(frames[i]);
            }
        }

        void Update(){
            if (playing){
                frame++;

                if (frame % updatesPerFrame == 0){
                    currentFrame++;
                    currentFrame = currentFrame % frames.size();
                }
            }
        }

        void Draw(){
            SDL_Rect drawRect;
            drawRect.x = position.first;
            drawRect.y = position.second;
            SDL_BlitSurface(frames[currentFrame], NULL, Helpers::surface, &drawRect);
        }

        void SetPlaying(bool status){
            playing = status;
        }

        std::vector<SDL_Surface*> frames;
        int currentFrame;
        int updatesPerFrame;
        long frame;
        bool playing;
};
