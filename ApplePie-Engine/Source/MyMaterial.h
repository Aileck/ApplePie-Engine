#pragma once
#include "./include/MathGeoLib/Math/MathAll.h"
#include "./include/tinygltf/tiny_gltf.h"
class MyTexture;
class MyMaterial
{
public:
    MyMaterial();
    ~MyMaterial();

    void LoadMaterial(const tinygltf::Model& model, const tinygltf::Material& material);
    MyTexture* GetDiffuseMap() const { return mDiffuseTexture; };
    MyTexture* GetSpecularMap()const { return mSpecularGlossinessTexture; };
    float4 GetDiffuseFactor()const { return mDiffuseFactor; };
    float3 GetSpecularFactor()const { return mSpecularFactor; };
    int GetGlossinessFactor()const { return mGlossinessFactor; };

private:

    float4 mDiffuseFactor;
    float3 mSpecularFactor;
    float mGlossinessFactor;

    MyTexture* mDiffuseTexture = nullptr;
    MyTexture* mSpecularGlossinessTexture = nullptr;
};

