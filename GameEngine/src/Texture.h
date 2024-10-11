#pragma once
#include <GL/glew.h>

class Texture
{
public:
    Texture();
    explicit Texture(const char* fileLoc);
    ~Texture();

    void loadTexture();
    void useTexture() const;
    void clearTexture();
private:
    GLuint textureId;
    int width, height, bitDepth;
    const char* fileLocation;
};

