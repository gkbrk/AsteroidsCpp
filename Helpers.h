#pragma once

#include <fstream>

class Helpers{
    public:
        static long getHighScore(){
            long score;
            std::ifstream scoreFile;
            scoreFile.open("highscore");

            if (!scoreFile){
                setHighScore(0);
                return 0;
            }
            scoreFile >> score;
            scoreFile.close();
            return score;
        }

        static void setHighScore(long score){
            std::ofstream scoreFile;
            scoreFile.open("highscore");

            scoreFile << score;
            scoreFile.close();
        }
};
