#pragma once
#include "Module.h"
#include <chrono>

using namespace std;
using namespace chrono;
class ModuleTimer :
    public Module
{
public:
    ModuleTimer();
    ~ModuleTimer();
    bool Init();
    update_status PreUpdate();
    bool CleanUp();
    inline float GetDeltaTime() const { return deltaTime; };

private:
    void ComputeDeltaTime();

    float deltaTime;
    high_resolution_clock::time_point lastFrameTime;
};

