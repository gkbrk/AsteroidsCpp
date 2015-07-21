#pragma once

#include <vector>
#include <string>
#include <sstream>
#include <iostream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

using namespace Helpers;

class FrameAnimation{
    public:
        FrameAnimation(){
            frame = 0;
            animationFrame = 0;
            updatesPerFrame = 1;
            loops = 1;
            done = false;
            started = false;
        }

        FrameAnimation(std::string path, int frameCount): FrameAnimation(){
            for (int i=0;i<frameCount;i++){
                std::ostringstream pathStream;
                pathStream << path << "-" << i << ".png";
                frames.push_back(IMG_Load(pathStream.str().c_str()));
            }
        }

        FrameAnimation(std::string path, int frameCount, int loops): FrameAnimation(path, frameCount){
            this->loops = loops;
        }

        FrameAnimation(std::string path, int frameCount, int loops, int updatesPerFrame): FrameAnimation(path, frameCount, loops){
            this->updatesPerFrame = updatesPerFrame;
        }

        ~FrameAnimation(){
            for (int i=0;i<frames.size();i++){
                SDL_FreeSurface(frames[i]);
            }
        }

        void Update(){
            if (!started){
                return;
            }
            
            frame++;

            if (loops != -1 && frame > loops * frames.size() * updatesPerFrame){
                done = true;
                return;
            }

            if (frame % (updatesPerFrame + 1) == 0){
                animationFrame++;
                animationFrame = animationFrame % frames.size();
            }
        }

        void Draw(SDL_Surface *surface, int x, int y){
            if (!done && started){
                SDL_Rect rect;
                rect.x = x;
                rect.y = y;
                SDL_BlitSurface(frames[animationFrame], NULL, Helpers::surface, &rect);
            }
        }

        void Start(){
            started = true;
        }

        std::vector<SDL_Surface*> frames;
        bool done;
        bool started;
        int frame;
        int animationFrame;
        int updatesPerFrame;
        int loops;
};
