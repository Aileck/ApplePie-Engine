#include "PropertiesGUI.h"
#include "backends\imgui.h"
#include "backends\imgui_impl_sdl2.h"
#include "backends\imgui_impl_opengl3.h"
void PropertiesGUI::Draw()
{
    if (!ImGui::Begin("Object propertie", NULL, ImGuiWindowFlags_MenuBar))
    {
        // Early out if the window is collapsed, as an optimization.
        ImGui::End();
        return;
    }

    if (ImGui::CollapsingHeader("FPS graph")) {

    }
    if (ImGui::CollapsingHeader("Variables Congiguration")) {

    }
    if (ImGui::CollapsingHeader("System Information")) {
        // RAM


    }
    ImGui::End();
}
