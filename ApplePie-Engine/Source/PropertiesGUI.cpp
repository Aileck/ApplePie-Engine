#include "PropertiesGUI.h"
#include "backends\imgui.h"
#include "backends\imgui_impl_sdl2.h"
#include "backends\imgui_impl_opengl3.h"
#include "MyModel.h"
#include "MyMesh.h"
#include "MyTexture.h"
#include <cmath>
void PropertiesGUI::Draw(const MyModel* model)
{
    if (!ImGui::Begin("Geometry propertie", NULL, ImGuiWindowFlags_MenuBar))
    {
        ImGui::End();
        return;
    }
    if (model != nullptr) {
        if (ImGui::CollapsingHeader("Geometry")) {
            std::vector<MyMesh*> meshes = model->GetMeshes();
            for (size_t i = 0; i < meshes.size(); ++i) {
                const MyMesh* mesh = meshes[i];

                ImGui::Text("Mesh id [%i] ", i);
                ImGui::Text("%i Vertices", mesh->GetVerticesCount());
                if (mesh->GetIndexCount() > 0) {
                    ImGui::Text("%f Triangles", std::round(mesh->GetIndexCount() / 3.0));
                }

                if (mesh->GetTextureID() != 0) {
                    ImGui::Text("UV1");
                }

                ImGui::Separator();
            }
            // Verteix, triangle, uv1
        }
        if (ImGui::CollapsingHeader("Texture")) {
            
            std::vector<MyTexture*> textures = model->GetTextures();
            if (textures.size() <= 0)
            {
                ImGui::Text("No texture information");
            }
            else {
                for (size_t i = 0; i < textures.size(); ++i) {
                    ImGui::Text("Texture id [%i] ", i);
                    ImGui::Text("size = %d x %d", textures[0]->getHeight(), textures[0]->getWidth());
                    ImGui::Image((void*)(intptr_t)textures[0]->getTextureID(), ImVec2(64, 64));
                    ImGui::Separator();
                }

            }

            // Mesh 1 Tex
            // Size, preview

        }
        ImGui::End();
    }

}
