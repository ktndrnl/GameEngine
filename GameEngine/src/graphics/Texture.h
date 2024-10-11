#pragma once
#include <GL/glew.h>

class Texture
{
public:
    Texture();
    explicit Texture(const char* fileLoc);
    ~Texture();

    bool loadTexture(bool withAlpha);
    void useTexture() const;
    void clearTexture();
private:
    GLuint textureId;
    int width, height, bitDepth;
    const char* fileLocation;
};

