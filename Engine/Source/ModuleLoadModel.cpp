#include "ModuleLoadModel.h"
#include "ModuleCamera.h"

#include "./include/MathGeoLib/Math/MathAll.h"
#include <cmath>
ModuleLoadModel::ModuleLoadModel()
{
}

ModuleLoadModel::~ModuleLoadModel()
{
 
}

bool ModuleLoadModel::Init()
{
    model = new MyModel();
    //model->Load("Triangle.gltf");
    //model->Load("BoxTextured.gltf");
    model->Load("BakerHouse/BakerHouse.gltf");
    //model->Load("Duck/Duck.gltf");
    
    return true;
}

update_status ModuleLoadModel::PreUpdate()
{
    return UPDATE_CONTINUE;
}

update_status ModuleLoadModel::Update()
{
    model->Draw();
    return UPDATE_CONTINUE;
}

update_status ModuleLoadModel::PostUpdate()
{
    return UPDATE_CONTINUE;
}

bool ModuleLoadModel::CleanUp()
{
    delete model;
    return true;
}

void ModuleLoadModel::LoadExteriorModel(const char* fullpath)
{
    App->WriteIntoLog(INFO_LOG, "Deleting the current model");
    delete model;
    model = nullptr;
    model = new MyModel(fullpath,true);
    AdjustCameraPosition();
}

void ModuleLoadModel::AdjustCameraPosition()
{
    float3 minBound = float3(model->GetMinX(), model->GetMinY(), model->GetMinZ());
    float3 maxBound = float3(model->GetMaxX(), model->GetMaxY(), model->GetMaxZ());
    float3 sceneCenter = (minBound + maxBound) * 0.5f;

    float distance = sceneCenter.Distance(maxBound);

    float3 newCameraPos = sceneCenter + float3(0.0f, 0.0f, distance*4);
    App->GetCamera()->SetCameraPosX(newCameraPos.x);
    App->GetCamera()->SetCameraPosY(newCameraPos.y);
    App->GetCamera()->SetCameraPosZ(newCameraPos.z);
}

