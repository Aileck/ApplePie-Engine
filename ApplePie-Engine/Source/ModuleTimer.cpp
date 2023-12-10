#include "ModuleTimer.h"
#include "Application.h"
#include "SDL.h"
ModuleTimer::ModuleTimer()
{
}

ModuleTimer::~ModuleTimer()
{
    delete frameHisroty;
}

bool ModuleTimer::Init()
{
    frameHisroty = new FrameRateData();
    lastTicks = SDL_GetTicks();
    return true;
}

update_status ModuleTimer::PreUpdate()
{
    ComputeDeltaTime();
    frameHisroty->AddNewFrame(GetFrame());
    return UPDATE_CONTINUE;
}

bool ModuleTimer::CleanUp()
{
    return true;
}

void ModuleTimer::ComputeDeltaTime()
{
    // The interval in seconds from the last frame to the current one (Unity).
    Uint32 currentTicks = SDL_GetTicks();

    deltaTime = (currentTicks - lastTicks) / 1000.0f; // ud = second
    lastTicks = currentTicks;
}
