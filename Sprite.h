#pragma once

#include <tuple>
#include <vector>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "Helpers.h"

class Sprite{
    public:
        Sprite(){
            #ifdef DEBUGMODE
                std::cout << "Created new Sprite." << std::endl;
            #endif
            image = NULL;
        }

        Sprite(std::string path): Sprite(){
            image = IMG_Load(path.c_str());

            width = image->w;
            height = image->h;
        }

        Sprite(std::string path, int x, int y): Sprite(path){
            position.first = x;
            position.second = y;
        }

        ~Sprite(){
            #ifdef DEBUGMODE
                std::cout << "Destroyed sprite." << std::endl;
            #endif
            SDL_FreeSurface(image);
        }

        virtual void InitSprite() {}
        virtual void Update() {}
        virtual void Update(double dt) {}
        virtual void HandleEvent(SDL_Event *e){}
        void Draw() {
            SDL_Rect spriteRect;
            spriteRect.x = position.first;
            spriteRect.y = position.second;
            SDL_BlitSurface(image, NULL, Helpers::surface, &spriteRect);
        }

        void SetPosition(int x, int y){
            position.first = x;
            position.second = y;
        }

        bool collidesWith(SDL_Rect otherObject){
            int leftA, leftB, rightA, rightB, topA, topB, bottomA, bottomB;

            //Sides of rect1
            leftA = position.first;
            rightA = position.first + width;
            topA = position.second;
            bottomA = position.second + height;

            //Sides of rect2
            leftB = otherObject.x;
            rightB = otherObject.x + otherObject.w;
            topB = otherObject.y;
            bottomB = otherObject.y + otherObject.h;

            //Detect collision
            if (bottomA <= topB || topA >= bottomB || rightA <= leftB || leftA >= rightB){
                return false;
            }else{
                return true;
            }
        }

        bool collidesWith(Sprite *other){
            int leftA, rightA, topA, bottomA, leftB, rightB, topB, bottomB;

            //Sides of first sprite
            leftA = position.first;
            rightA = position.first + width;
            topA = position.second;
            bottomA = position.second + height;

            //Sides of second sprite
            leftB = other->position.first;
            rightB = other->position.first + other->width;
            topB = other->position.second;
            bottomB = other->position.second + other->height;

            //Detect collision
            if (bottomA <= topB || topA >= bottomB || rightA <= leftB || leftA >= rightB){
                return false;
            }else{
                return true;
            }

        }

        SDL_Surface *image;
        std::pair<double, double> position;

        int width, height;
};
