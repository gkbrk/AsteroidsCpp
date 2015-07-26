#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <vector>
#include <sstream>

#include "Helpers.h"

using namespace Helpers;

class AnimatedSprite: public Sprite{
    public:
        AnimatedSprite(){
            #ifdef DEBUGMODE
                std::cout << "New animated sprite." << std::endl;
            #endif
        }

        AnimatedSprite(std::string path, int frameCount, int speed): AnimatedSprite(){
            frame = 0;
            currentFrame = 0;
            playing = true;
            
            this->speed = speed;

            for (int i=0;i<frameCount;i++){
                std::ostringstream pathStream;
                pathStream << path << "-" << i << ".png";
                frames.push_back(IMG_Load(pathStream.str().c_str()));
            }

            width = frames[0]->w;
            height = frames[0]->h;
        }

        AnimatedSprite(std::string path, int frameCount, int speed, int x, int y): AnimatedSprite(path, frameCount, speed){
            position.first = x;
            position.second = y;
        }

        ~AnimatedSprite(){
            #ifdef DEBUGMODE
                std::cout << "Destroying animated sprite." << std::endl;
            #endif

            for (int i=0;i<frames.size();i++){
                #ifdef DEBUGMODE
                    std::cout << "Destroying frame " << i << "." << std::endl;
                #endif
                SDL_FreeSurface(frames[i]);
            }
        }

        void Update(double dt){
            if (playing){
                frame += dt * speed;
                
                currentFrame = (int)frame % frames.size();
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
        int speed;
        double frame;
        bool playing;
};
