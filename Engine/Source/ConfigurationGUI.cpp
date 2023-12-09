#include "ConfigurationGUI.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleImGUI.h"
#include "SDL.h"
#include "GL\glew.h"
#include "DirectXTex.h"
#include <cmath>
#include "windows.h"
void ConfigurationGUI::Draw(FrameRateData frames)
{
    ImGuiWindowFlags window_flags = 0;
    const ImGuiViewport* main_viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(ImVec2(main_viewport->WorkPos.x + 650, main_viewport->WorkPos.y + 20), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(550, 680), ImGuiCond_FirstUseEver);
    // Main body of the Demo window starts here.
    if (!ImGui::Begin("Editor configuration",NULL, ImGuiWindowFlags_MenuBar))
    {
        // Early out if the window is collapsed, as an optimization.
        ImGui::End();
        return;
    }
    DrawMainMenu();
	if (ImGui::CollapsingHeader("FPS graph")) {
        ImGui::Text("Average %f", std::round(frames.average));
        ImGui::PlotHistogram("FPS", frames.dataArray, IM_ARRAYSIZE(frames.dataArray), 0, NULL, 0.0f, frames.max, ImVec2(0, 80.0f));
	}
	if (ImGui::CollapsingHeader("Variables Congiguration")) {

	}
	if (ImGui::CollapsingHeader("System Information")) {
        // RAM
        try {
            MEMORYSTATUSEX memoryStatus;
            memoryStatus.dwLength = sizeof(memoryStatus);

            if (GlobalMemoryStatusEx(&memoryStatus)) {
                ImGui::Text("RAM usage (MB) %llu | %llu", memoryStatus.ullAvailPhys / (1024 * 1024), memoryStatus.ullTotalPhys / (1024 * 1024));
            }
        }
        catch (std::exception& e) {
            ImGui::Text("Error getting RAM information");
        }
        ImGui::Separator();
        // HW
        ImGui::Text("Vendor: %s", glGetString(GL_VENDOR));
        ImGui::Text("Renderer: %s", glGetString(GL_RENDERER));
        ImGui::Separator();
        // GRAPHIC API
        ImGui::Text("Using Glew %s", glewGetString(GLEW_VERSION));
        ImGui::Text("OpenGL version supported %s", glGetString(GL_VERSION));
        ImGui::Text("GLSL: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
        ImGui::Text("DirectXTex version: %i\n", DIRECTX_TEX_VERSION);

     }
    ImGui::End();
}

void ConfigurationGUI::DrawMainMenu()
{
    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("Editor"))
        {
            if (ImGui::MenuItem("Hide Editor windows", NULL, hideEditor)) {
                hideEditor = !hideEditor;
                if (hideEditor) {
                    SDL_HideWindow(App->GetWindow()->window);
                }
                else {
                    SDL_ShowWindow(App->GetWindow()->window);
                }
               
            }
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }
}