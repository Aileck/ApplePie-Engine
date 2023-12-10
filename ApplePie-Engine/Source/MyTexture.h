#pragma once
class MyTexture {
public:
    MyTexture();
    ~MyTexture();

    void LoadTexture(const char* textureName, const bool exterior = false);

    inline unsigned getTextureID() const { return textureID; }
    inline unsigned getWidth() const { return imWidth; }
    inline unsigned getHeight() const { return imHeight; }

private:
    unsigned textureID;
    unsigned imWidth;
    unsigned imHeight;
};