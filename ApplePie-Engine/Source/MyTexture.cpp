#include "MyTexture.h"
#include "Application.h"
#include "Globals.h"
#include "DirectXTex.h"
#include "SDL.h"
#include "GL\glew.h"
#include "Application.h"
#include "FileComponent.h"

using namespace DirectX;

MyTexture::MyTexture(GLuint id, GLsizei width, GLsizei height)
{
    textureID = id;
    imWidth = width;
    imHeight = height;
}

MyTexture::MyTexture()
{
}

MyTexture::~MyTexture()
{
    App->WriteIntoLog(INFO_LOG, "Deleting the current texture");
    glDeleteTextures(1, &textureID);
}

void MyTexture::LoadTexture(const char* textureName, const bool exterior)
{
    const wchar_t* filePath = FileComponent::CreateWideFilePath(TEXTURE_PATH, textureName);
    if (exterior)
    {
        filePath = FileComponent::ConvertToWideFilePath(textureName);
    }
    //1. Load image data with external library into CPU
    HRESULT result = E_FAIL;
    ScratchImage imageData;
    result = LoadFromDDSFile(filePath, DDS_FLAGS_NONE, nullptr, imageData);
    if (result != S_OK) {

        result = LoadFromTGAFile(filePath, TGA_FLAGS_NONE, nullptr, imageData);

        if (result != S_OK) {
            result = LoadFromWICFile(filePath, WIC_FLAGS_NONE, nullptr, imageData);
        }
    }
    assert(result == S_OK);
    unsigned textureId = 0u;
    //2. Create and load OpenGL texture into GPU
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glObjectLabel(GL_TEXTURE, textureId, -1, textureName);
    //2.1 Loading texture data to OpenGL
    GLint internalFormat = 0;
    GLsizei imWidth = imageData.GetMetadata().width;
    GLsizei imHeigh = imageData.GetMetadata().height;
    GLenum format = 0;
    GLenum  type = 0;

    switch (imageData.GetMetadata().format)
    {
        case DXGI_FORMAT_R8G8B8A8_UNORM_SRGB:
        case DXGI_FORMAT_R8G8B8A8_UNORM:
            internalFormat = GL_RGBA8;
            format = GL_RGBA;
            type = GL_UNSIGNED_BYTE;
            break;
        case DXGI_FORMAT_B8G8R8A8_UNORM_SRGB:
        case DXGI_FORMAT_B8G8R8A8_UNORM:
            internalFormat = GL_RGBA8;
            format = GL_BGRA;
            type = GL_UNSIGNED_BYTE;
            break;
        case DXGI_FORMAT_B5G6R5_UNORM:
            internalFormat = GL_RGB8;
            format = GL_BGR;
            type = GL_UNSIGNED_BYTE;
            break;
        default:
            assert(false && "Unsupported format");
    }

    // if ScratchImage contains mipmaps, we can load them into Video RAM by calling glTexImage2D several times
    if (imageData.GetMetadata().mipLevels > 0) {
        for (size_t i = 0; i < imageData.GetMetadata().mipLevels; ++i)
        {
            //Image* mip = imageData->GetImage(i, 0, 0);
            const Image* mip = imageData.GetImage(i, 0, 0);
            glTexImage2D(GL_TEXTURE_2D, i, internalFormat, mip->width, mip->height, 0, format, type, mip->pixels);

        }
    }
    else {
        void* imData = imageData.GetPixels();
        glTexImage2D(GL_TEXTURE_2D, 0, internalFormat,
            imWidth, imHeigh, 0,
            format, type, imData
        );
        glGenerateMipmap(GL_TEXTURE_2D);
    }

    // Deformation when scale
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, imageData.GetMetadata().mipLevels - 1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


    textureID = textureId;
    imWidth = imWidth;
    imHeight = imHeigh;
}

