/***********************************************************
 *  TextureManager Implementation
 *
 *  Implements all texture management logic, moved out of
 *  SceneManager for better modularity and code reuse.
 ***********************************************************/

#include "TextureManager.h"
#include "stb_image.h"
#include <iostream>


TextureManager::TextureManager() {}

TextureManager::~TextureManager() {
    DestroyTextures(); // Ensure all textures are cleaned up
}

bool TextureManager::LoadTexture(const char* filename, const std::string& tag) {
    // Loads an image file and creates an OpenGL texture, storing it with the given tag
    int width, height, colorChannels;
    GLuint textureID;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* image = stbi_load(filename, &width, &height, &colorChannels, 0);

    if (image) {
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        if (colorChannels == 3)
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
        else if (colorChannels == 4)
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
        else {
            std::cout << "Unsupported channel count: " << colorChannels << std::endl;
            stbi_image_free(image);
            return false;
        }

        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(image);
        glBindTexture(GL_TEXTURE_2D, 0);

        m_textures.push_back({ tag, textureID }); // Store the texture info
        return true;
    }
    std::cout << "Could not load image: " << filename << std::endl;
    return false;
}

void TextureManager::BindTextures() {
    // Binds each loaded texture to its corresponding texture unit
    for (size_t i = 0; i < m_textures.size(); ++i) {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, m_textures[i].ID);
    }
}

void TextureManager::DestroyTextures() {
    // Deletes all textures from OpenGL and clears the list
    for (auto& tex : m_textures) {
        glDeleteTextures(1, &tex.ID);
    }
    m_textures.clear();
}

int TextureManager::FindTextureID(const std::string& tag) const {
    // Returns the OpenGL texture ID for the given tag, or -1 if not found
    for (const auto& tex : m_textures) {
        if (tex.tag == tag) return tex.ID;
    }
    return -1;
}

int TextureManager::FindTextureSlot(const std::string& tag) const {
    // Returns the texture slot (unit) for the given tag, or -1 if not found
    for (size_t i = 0; i < m_textures.size(); ++i) {
        if (m_textures[i].tag == tag) return static_cast<int>(i);
    }
    return -1;
}
