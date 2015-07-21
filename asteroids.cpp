#include <iostream>
#include <vector>
#include <list>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

#include "GameState.h"
#include "SplashScreen.h"
#include "StartScreen.h"
#include "AsteroidsGame.h"

SDL_Window *window = NULL;
SDL_Surface *surface = NULL;

bool init_sdl(){
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0){
        return false;
    }else{
        window = SDL_CreateWindow("Asteroids", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);
        if (window == NULL){
            return false;
        }else{
            surface = SDL_GetWindowSurface(window);
            
            IMG_Init(IMG_INIT_PNG);
            TTF_Init();
            Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 2048);
            
            return true;
        }
    }
}

int main(){
    std::list<GameState*> gameStates;
    gameStates.push_back(new SplashScreen("SplashScreen/", 3, 250));
    gameStates.push_back(new StartScreen());
    gameStates.push_back(new AsteroidsGame());
    if (init_sdl()){
        while (gameStates.size() > 0){
            GameState *state = gameStates.front();
            gameStates.pop_front();

            state->stateFinished = false;
            state->quit = false;
            state->window = window;
            state->surface = surface;
            state->states = &gameStates;

            state->InitState();
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
                state->Update();
                state->Draw();
            }
            delete state;
        }
        SDL_DestroyWindow(window);
    }else{
        std::cerr << "Failed to initialize SDL." << std::endl;
    }
    SDL_Quit();

    return 0;
}
