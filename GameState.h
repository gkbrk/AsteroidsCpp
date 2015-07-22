#pragma once

#include <SDL2/SDL.h>
#include <iostream>
#include <list>

class GameState{
    public:
        virtual ~GameState() {}
        virtual void InitState() {}
        virtual void HandleEvent(SDL_Event *e) {}
        virtual void Update() {}
        virtual void Update(double dt) {}
        virtual void Draw() {}
        
        bool stateFinished;
        bool gotoNext;
        bool quit;
        std::list<GameState*> *states;
};
