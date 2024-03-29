#include "ModuleLoadModel.h"
#include "ModuleCamera.h"


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
    model->Load("ZomBunny/Zombunny.gltf");
    //model->Load("Wall/Wall.gltf");
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
    
    delete model;
    model = nullptr;
    model = new MyModel(fullpath,true);
    App->GetCamera()->FocusOn(model->GetCenter());
}

void ModuleLoadModel::LoadExteriorTexture(const char* fullpath)
{
    model->LoadExternalTexture(fullpath);
}





