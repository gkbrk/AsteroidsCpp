#pragma once

#include <tuple>
#include <vector>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

class Sprite{
    public:
        Sprite(std::string path, int x, int y){
            image = IMG_Load(path.c_str());
            position.first = x;
            position.second = y;

            width = image->w;
            height = image->h;
        }

        virtual void InitSprite() {}
        virtual void Update() {}
        void Draw() {
            SDL_Rect spriteRect;
            spriteRect.x = position.first;
            spriteRect.y = position.second;
            SDL_BlitSurface(image, NULL, surface, &spriteRect);
        }

        void SetPosition(int x, int y){
            position.first = x;
            position.second = y;
        }

        void SetSurface(SDL_Surface *surface){
            this->surface = surface;
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

        SDL_Surface *surface;
        SDL_Surface *image;
        std::pair<int, int> position;

        int width, height;
};
