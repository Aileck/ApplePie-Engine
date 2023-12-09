#ifndef __ModuleGUI_H__
#define __ModuleGUI_H__

#include "Module.h"
#include "SDL/include/SDL.h"

struct FrameRateData {
    #define ARRAY_SIZE 1000

    float dataArray[ARRAY_SIZE];
    int top;  
    float average;
    float max;

public:
    FrameRateData() : top(-1), average(0.0f), max(0.0f) {}

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

        average = (top >= 0) ? (sum / ARRAY_SIZE) : 0;
    }

    void UpdateMax(float newValue) {
        if (newValue > max) {
            max = newValue;
        }
    }

};

class ConsoleGUI;
class ConfigurationGUI;

class ModuleImGUI :
    public Module
{
public:
	ModuleImGUI();
	~ModuleImGUI();

	bool Init();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();

	void DrawMainMenu();

	ConsoleGUI* GetConsole() { return logConsole; };
	

private:
    FrameRateData frameRates;
	ConsoleGUI* logConsole = nullptr;
	ConfigurationGUI* conf = nullptr;

	bool show_tool_about = false;

	bool quit = false;
};

#endif
