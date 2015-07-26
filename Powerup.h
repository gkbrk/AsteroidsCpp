#pragma once

#include "Sprite.h"

class Powerup: public Sprite{
    public:
        Powerup(std::string path, int x, int y) :Sprite(path, x, y){}
        int type;
};
