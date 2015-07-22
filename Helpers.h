#pragma once

#include <fstream>

#include <SDL2/SDL.h>

namespace Helpers{
    static void setHighScore(long score){
        std::ofstream scoreFile;
        scoreFile.open("highscore");

        scoreFile << score;
        scoreFile.close();
    }

    static long getHighScore(){
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

    static SDL_Window *window;
    static SDL_Surface *surface;
    static bool menuMusicPlayed = false;
}
