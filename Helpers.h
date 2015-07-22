#pragma once

#include <fstream>

#include <SDL2/SDL.h>

namespace Helpers{
    void setHighScore(long score){
        std::ofstream scoreFile;
        scoreFile.open("highscore");

        scoreFile << score;
        scoreFile.close();
    }

    long getHighScore(){
        long score;
        std::ifstream scoreFile;
        scoreFile.open("highscore");

        if (!scoreFile){
            Helpers::setHighScore(0);
            return 0;
        }
        scoreFile >> score;
        scoreFile.close();
        return score;
    }

    SDL_Window *window;
    SDL_Surface *surface;
    bool menuMusicPlayed = false;
}
