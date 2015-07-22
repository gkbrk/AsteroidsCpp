#pragma once

#include <map>

#include <SDL2/SDL_ttf.h>

namespace FontStore{
    std::map<std::string, TTF_Font*> fontMap;

    void LoadFontIfNotLoaded(std::string path){
        auto it = fontMap.find(path);

        if (it == fontMap.end()){
            fontMap[path] = TTF_OpenFont(path, 20);
        }
    }
}
