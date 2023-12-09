#pragma once
#include <GL/glew.h>

class MyTexture {
public:
    MyTexture(GLuint id, GLsizei width, GLsizei height);
    ~MyTexture();

    inline GLuint getTextureID() const { return textureID; }
    inline GLsizei getWidth() const { return imWidth; }
    inline GLsizei getHeight() const { return imHeight; }

private:
    unsigned textureID;
    unsigned imWidth;
    unsigned imHeight;
};