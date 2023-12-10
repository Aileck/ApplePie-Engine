#pragma once
#include "Module.h"
#include "cmath"
struct FrameRateData {
#define ARRAY_SIZE 1000

    float dataArray[ARRAY_SIZE];
    int top;
    float average;
    float max;

public:
    FrameRateData()
    {
        top = -1;
        average = 0.0f;
        max = 0.0f;
    }

    void AddNewFrame(float newValue) {
        if (top == ARRAY_SIZE - 1) {
            ShiftStack();
        }

        dataArray[++top] = newValue;

        CalculateAverage();
        UpdateMax(newValue);
    }
private:
    void ShiftStack() {
        for (int i = 0; i < top; ++i) {
            dataArray[i] = dataArray[i + 1];
        }

        --top;
    }

    void CalculateAverage() {
        float sum = 0.0f;
        for (int i = 0; i <= top; ++i) {
            sum += dataArray[i];
        }

        average = (top >= 0) ? (sum / (top + 1)) : 0;
    }

    void UpdateMax(float newValue) {
        if (std::isfinite(newValue) && (newValue > max || !std::isfinite(max)))
        {
            max = newValue;
        }
    }

};
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
    inline float GetFrame() const { return 1 / deltaTime; };
    inline FrameRateData* GetFrameRateData() const { return frameHisroty; };

private:
    void ComputeDeltaTime();

    FrameRateData* frameHisroty = nullptr;
    float deltaTime;
    unsigned lastTicks;
};

