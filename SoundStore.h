#pragma once

#include <map>

#include <SDL2/SDL_mixer.h>

namespace SoundStore{
    std::map<std::string, Mix_Chunk*> soundMap;
    std::map<std::string, Mix_Music*> musicMap;

    void LoadSoundIfNotLoaded(std::string path){
        std::map<std::string, Mix_Chunk*>::iterator it;

        it = soundMap.find(path);
        if (it == soundMap.end()){
            soundMap[path] = Mix_LoadWAV(path.c_str());
        }
    }

    void LoadSoundIfNotLoaded(std::string path, std::string key){
        std::map<std::string, Mix_Chunk*>::iterator it;

        it = soundMap.find(key);
        if (it == soundMap.end()){
            soundMap[key] = Mix_LoadWAV(path.c_str());
        }
    }

    void LoadMusicIfNotLoaded(std::string path){
        std::map<std::string, Mix_Music*>::iterator it;

        it = musicMap.find(path);
        if (it == musicMap.end()){
            musicMap[path] = Mix_LoadMUS(path.c_str());
        }
    }
    
    void LoadMusicIfNotLoaded(std::string path, std::string key){
        std::map<std::string, Mix_Music*>::iterator it;

        it = musicMap.find(key);
        if (it == musicMap.end()){
            musicMap[key] = Mix_LoadMUS(path.c_str());
        }
    }

    Mix_Chunk* GetSound(std::string key){
        return soundMap[key];
    }

    Mix_Music* GetMusic(std::string key){
        return musicMap[key];
    }
}
