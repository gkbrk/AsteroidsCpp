#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "Helpers.h"

class Text{
    public:
        Text(){}

        Text(std::string fontPath, int fontSize){
            font = TTF_OpenFont(fontPath.c_str(), fontSize);
            textColor = {255, 255, 255};
            SetText(" ");
        }

        ~Text(){
            #ifdef DEBUGMODE
                std::cout << "Cleaning text object." << std::endl;
            #endif
            TTF_CloseFont(font);
            SDL_FreeSurface(renderedFont);
        }

        void SetText(std::string text, SDL_Color color){
            renderedFont = TTF_RenderText_Solid(font, text.c_str(), color);
            width = renderedFont->w;
            height = renderedFont->h;
        }

        void SetText(std::string text){
            SetText(text, textColor);
        }

        void SetColor(SDL_Color color){
            textColor = color;
        }

        void SetColor(int r, int g, int b){
            textColor = {(Uint8)r, (Uint8)g, (Uint8)b};
        }

        void Draw(int x, int y){
            SDL_Rect drawRect;
            drawRect.x = x;
            drawRect.y = y;
            SDL_BlitSurface(renderedFont, NULL, Helpers::surface, &drawRect);
        }

        TTF_Font *font;
        SDL_Color textColor;
        SDL_Surface *renderedFont;

        int width, height;
};
