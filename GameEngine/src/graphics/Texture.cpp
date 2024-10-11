#include "Texture.h"

#include <iostream>
#include <stb_image.h>

Texture::Texture()
{
    fileLocation = "";
    textureId = 0;
    width = 0;
    height = 0;
    bitDepth = 0;
}

Texture::Texture(const char* fileLoc)
{
    fileLocation = fileLoc;
    textureId = 0;
    width = 0;
    height = 0;
    bitDepth = 0;
}

Texture::~Texture()
{
    clearTexture();
}

bool Texture::loadTexture(const bool withAlpha)
{
    unsigned char* texData = stbi_load(fileLocation, &width, &height, &bitDepth, 0);
    if (!texData)
    {
        std::cout << "Failed to find: " << fileLocation << std::endl;
        return false;
    }

    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // GL_LINEAR for smoother texture, GL_NEAREST for pixelated texture
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // 1 byte alignment, needed when texture width is not divisible by 4
    const GLenum format = withAlpha ? GL_RGBA : GL_RGB;
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, texData);
    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);

    stbi_image_free(texData);
    return true;
}

void Texture::useTexture() const
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureId);
}

void Texture::clearTexture()
{
    glDeleteTextures(1, &textureId);
    textureId = 0;
    width = 0;
    height = 0;
    bitDepth = 0;
    fileLocation = "";
}
