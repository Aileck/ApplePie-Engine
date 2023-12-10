#include "ConfigurationGUI.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleImGUI.h"
#include "ModuleCamera.h"
#include "ModuleTimer.h"
#include "SDL.h"
#include "GL\glew.h"
#include "DirectXTex.h"
#include <cmath>
#include "windows.h"

ConfigurationGUI::ConfigurationGUI()
{
    temporalHeight = App->GetWindow()->GetCurrentHeight();
    temporalWidth = App->GetWindow()->GetCurrentWidth();
}
int InputSizeCallback(ImGuiInputTextCallbackData* data)
{
    // if is not digit
    if (data->EventChar != 0 && !isdigit(data->EventChar))
        return 1; 
    
    return 0; 
}

void ConfigurationGUI::Draw()
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
        FrameRateData* frameHistory = App->GetTimer()->GetFrameRateData();
        ImGui::Text("Average %f", std::round(frameHistory->average));
        ImGui::PlotHistogram("FPS", frameHistory->dataArray, IM_ARRAYSIZE(frameHistory->dataArray), 0, NULL, 0.0f, frameHistory->max, ImVec2(0, 80.0f));
	}	
	if (ImGui::CollapsingHeader("Window Configuration")) {
        // Case: have unsubmit input
        if (App->GetWindow()->GetCurrentHeight() != temporalHeight ||
            App->GetWindow()->GetCurrentWidth() != temporalWidth)
        {
            hasNoFinishedInput = true;
        }

        // Number to set size
        static char bufHeight[5];
        static char bufWidth[5];
        sprintf_s(bufHeight, sizeof(bufHeight), "%d", temporalHeight);
        sprintf_s(bufWidth, sizeof(bufWidth), "%d", temporalWidth);

        ImGui::SetNextItemWidth(50);
        ImGui::InputText("Height", bufHeight, 5, ImGuiInputTextFlags_CallbackEdit, InputSizeCallback);
        ImGui::SameLine();
        ImGui::Text("x");
        ImGui::SameLine();
        ImGui::SetNextItemWidth(50);
        ImGui::InputText("Width", bufWidth, 5, ImGuiInputTextFlags_CallbackEdit, InputSizeCallback);

        temporalHeight = std::atoi(bufHeight);
        temporalWidth = std::atoi(bufWidth);

        // Set or reset
        setWindow = ImGui::Button("Set Window Size");
        if (setWindow) 
        {
            App->GetWindow()->SetWindowSize(temporalHeight, temporalWidth);
            hasNoFinishedInput = false;
        }
        if (hasNoFinishedInput) 
        {
            ImGui::SameLine();
            resetWindow = ImGui::Button("Reset values");
            if (resetWindow) {
                temporalHeight = App->GetWindow()->GetCurrentHeight();
                temporalWidth = App->GetWindow()->GetCurrentWidth();
                hasNoFinishedInput = false;
            }
        }
        // FullScreen
        if (ImGui::Checkbox("Editor Fullscreen", &editorFullcreen)) {
            App->GetWindow()->SetFullscreen(editorFullcreen);   
        }

        // Borderless
        if (ImGui::Checkbox("Editor No Border", &editorNoBorder)) 
        {
            App->GetWindow()->SetBorderless(editorNoBorder);
        }
	}
    if (ImGui::CollapsingHeader("Camera Configuration")) {
        static char bufX[80];
        sprintf_s(bufX, sizeof(bufX), "%f", App->GetCamera()->GetCamera()->pos.x);
        static char bufY[80];
        sprintf_s(bufY, sizeof(bufY), "%f", App->GetCamera()->GetCamera()->pos.y);
        static char bufZ[80];
        sprintf_s(bufZ, sizeof(bufZ), "%f", App->GetCamera()->GetCamera()->pos.z);

        static char bufFrontX[80];
        sprintf_s(bufFrontX, sizeof(bufFrontX), "%f", App->GetCamera()->GetCamera()->front.x);
        static char bufFrontY[80];
        sprintf_s(bufFrontY, sizeof(bufFrontY), "%f", App->GetCamera()->GetCamera()->front.y);
        static char bufFrontZ[80];
        sprintf_s(bufFrontZ, sizeof(bufFrontZ), "%f", App->GetCamera()->GetCamera()->front.z);

        static char bufUpX[80];    
        sprintf_s(bufUpX, sizeof(bufUpX), "%f", App->GetCamera()->GetCamera()->up.x);
        static char bufUpY[80];     
        sprintf_s(bufUpY, sizeof(bufUpY), "%f", App->GetCamera()->GetCamera()->up.y);
        static char bufUpZ[80];       
        sprintf_s(bufUpZ, sizeof(bufUpZ), "%f", App->GetCamera()->GetCamera()->up.z);
        
        //sprintf_s(bufHeight, sizeof(bufHeight), "%d", temporalHeight);
        //sprintf_s(bufWidth, sizeof(bufWidth), "%d", temporalWidth);


        ImGui::Text("Position");
        ImGui::Text("X");
        ImGui::SameLine();
        ImGui::SetNextItemWidth(50);
        if (ImGui::InputText("##X1", bufX, 80, ImGuiInputTextFlags_CallbackEdit, InputSizeCallback)) 
        {
            App->GetCamera()->SetCameraPosX(std::atof(bufX));
        }
        ImGui::SameLine();
        
        ImGui::Text("Y");
        ImGui::SameLine();
        ImGui::SetNextItemWidth(50);
        if (ImGui::InputText("##Y1", bufY, 80, ImGuiInputTextFlags_CallbackEdit, InputSizeCallback))
        {
            App->GetCamera()->SetCameraPosY(std::atof(bufY));
        }
        ImGui::SameLine();

        ImGui::Text("Z");
        ImGui::SameLine();
        ImGui::SetNextItemWidth(50);
        if (ImGui::InputText("##Z1", bufZ, 80, ImGuiInputTextFlags_CallbackEdit, InputSizeCallback))
        {
            App->GetCamera()->SetCameraPosZ(std::atof(bufZ));
        }

        ImGui::Text("Front");
        ImGui::Text("X");
        ImGui::SameLine();
        ImGui::SetNextItemWidth(50);
        if(ImGui::InputText("##X2", bufFrontX, 80, ImGuiInputTextFlags_CallbackEdit, InputSizeCallback))
        {
            App->GetCamera()->SetFrontX(std::atof(bufFrontX));
        }
        ImGui::SameLine();

        ImGui::Text("Y");
        ImGui::SameLine();
        ImGui::SetNextItemWidth(50);
        if (ImGui::InputText("##Y2", bufFrontY, 80, ImGuiInputTextFlags_CallbackEdit, InputSizeCallback))
        {
            App->GetCamera()->SetFrontY(std::atof(bufFrontY));
        }
        ImGui::SameLine();

        ImGui::Text("Z");
        ImGui::SameLine();
        ImGui::SetNextItemWidth(50);
        if (ImGui::InputText("##Z2", bufFrontZ, 80, ImGuiInputTextFlags_CallbackEdit, InputSizeCallback)) 
        {
            App->GetCamera()->SetFrontZ(std::atof(bufFrontZ));
        }


        ImGui::Text("Up");
        ImGui::Text("X");
        ImGui::SameLine();
        ImGui::SetNextItemWidth(50);
        if (ImGui::InputText("##X3", bufUpX, 80, ImGuiInputTextFlags_CallbackEdit, InputSizeCallback))
        {
            App->GetCamera()->SetUpX(std::atof(bufUpX));
        }

        ImGui::SameLine();

        ImGui::Text("Y");
        ImGui::SameLine();
        ImGui::SetNextItemWidth(50);
        if (ImGui::InputText("##Y3", bufUpY, 80, ImGuiInputTextFlags_CallbackEdit, InputSizeCallback))
        {
            App->GetCamera()->SetUpY(std::atof(bufUpY));
        }
        ImGui::SameLine();

        ImGui::Text("Z");
        ImGui::SameLine();
        ImGui::SetNextItemWidth(50);
        if (ImGui::InputText("##Z3", bufUpZ, 80, ImGuiInputTextFlags_CallbackEdit, InputSizeCallback))
        {
            App->GetCamera()->SetUpZ(std::atof(bufUpZ));
        }


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
                    SDL_HideWindow(App->GetWindow()->GetWindow());
                }
                else {
                    SDL_ShowWindow(App->GetWindow()->GetWindow());
                }
               
            }
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }
}