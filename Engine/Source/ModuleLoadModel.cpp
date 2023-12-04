#include "ModuleLoadModel.h"

ModuleLoadModel::ModuleLoadModel()
{
}

ModuleLoadModel::~ModuleLoadModel()
{
 
}

bool ModuleLoadModel::Init()
{
    model = new MyModel();
    //model->Load("triangleWithoutIndices.gltf");
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
