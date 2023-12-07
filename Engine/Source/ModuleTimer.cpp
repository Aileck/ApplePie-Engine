#include "ModuleTimer.h"
#include "Application.h"

ModuleTimer::ModuleTimer()
{
}

ModuleTimer::~ModuleTimer()
{
}

bool ModuleTimer::Init()
{
    return true;
}

update_status ModuleTimer::PreUpdate()
{
    ComputeDeltaTime();
    //LOG("%f", deltaTime);
    return UPDATE_CONTINUE;
}

bool ModuleTimer::CleanUp()
{
    return true;
}

void ModuleTimer::ComputeDeltaTime()
{
    // The interval in seconds from the last frame to the current one (Unity).
    auto currentFrameTime = high_resolution_clock::now();
    duration<float> duration = currentFrameTime - lastFrameTime;
    deltaTime = duration.count();
    lastFrameTime = currentFrameTime;
}
