#pragma once

#include <chrono>
#include <iostream>

using namespace std::chrono;

class DeltaTimer{
    public:
        DeltaTimer(){
            lastTime = high_resolution_clock::now();
        }

        double GetDelta(){
            time_point<high_resolution_clock> currentTime = high_resolution_clock::now();
            duration<double> diff = currentTime - lastTime;

            lastTime = high_resolution_clock::now();

            return diff.count();
        }

        time_point<high_resolution_clock> lastTime;
};
