#include "MyTexture.h"
#include "Application.h"
#include "Globals.h"
MyTexture::MyTexture(GLuint id, GLsizei width, GLsizei height)
{
    textureID = id;
    imWidth = width;
    imHeight = height;
}

MyTexture::~MyTexture()
{
    App->WriteIntoLog(INFO_LOG, "Deleting the current texture");
    glDeleteTextures(1, &textureID);
}

