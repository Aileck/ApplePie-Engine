#include "MyTexture.h"

MyTexture::MyTexture(GLuint id, GLsizei width, GLsizei height)
{
    textureID = id;
    imWidth = width;
    imHeight = height;
}

MyTexture::~MyTexture()
{
    glDeleteTextures(1, &textureID);
}

