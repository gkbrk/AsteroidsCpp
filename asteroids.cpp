#include <iostream>
#include <vector>
#include <list>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

#include "Helpers.h"
#include "SoundStore.h"
#include "DeltaTimer.h"
#include "GameState.h"
#include "SplashScreen.h"
#include "StartScreen.h"
#include "AsteroidsGame.h"

bool init_sdl(){
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0){
        return false;
    }else{
        Helpers::window = SDL_CreateWindow("Asteroids", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);
        if (Helpers::window == NULL){
            return false;
        }else{
            Helpers::surface = SDL_GetWindowSurface(Helpers::window);
            
            IMG_Init(IMG_INIT_PNG);
            TTF_Init();
            Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 2048);
            
            return true;
        }
    }
}

int main(int argc, char *argv[]){
    std::list<GameState*> gameStates;
    gameStates.push_back(new SplashScreen("SplashScreen/", 3, 700));
    gameStates.push_back(new StartScreen());
    gameStates.push_back(new AsteroidsGame());

    double smoothTime = 0;
    if (init_sdl()){
        while (gameStates.size() > 0){
            GameState *state = gameStates.front();
            gameStates.pop_front();

            state->stateFinished = false;
            state->quit = false;
            state->states = &gameStates;

            if (!state->gotoNext){
                state->InitState();
            }else{
                state->gotoNext = false;
            }
            DeltaTimer timer;
            while (!state->stateFinished){
                SDL_Event e;
                while (SDL_PollEvent(&e) != 0){
                    if (e.type == SDL_QUIT){
                        gameStates.clear();
                        state->stateFinished = true;
                    }
                    state->HandleEvent(&e);
                }
                if (state->quit){
                    gameStates.clear();
                    state->stateFinished = true;
                }
                if (state->gotoNext){
                    gameStates.push_back(state);
                    break;
                }
                double delta = timer.GetDelta();
                state->Update(delta);
                state->Draw();
                SDL_UpdateWindowSurface(Helpers::window);
            }
            if (!state->gotoNext){
                delete state;
            }
        }
        SDL_DestroyWindow(Helpers::window);
    }else{
        std::cerr << "Failed to initialize SDL." << std::endl;
    }
    SDL_Quit();

    return 0;
}
