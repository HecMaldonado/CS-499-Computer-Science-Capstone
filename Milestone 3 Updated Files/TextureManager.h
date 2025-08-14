/***********************************************************
 *  TextureManager
 *
 *  This new class was added to modularize and centralize
 *  all texture loading, binding, and management operations.
 *  This replaces the previous approach where SceneManager
 *  handled textures directly, improving modularity and
 *  maintainability.
 ***********************************************************/

#pragma once
#include <string>
#include <vector>
#include <GL/glew.h>


class TextureManager {
public:
    // Structure to store texture tag and OpenGL ID
    struct TextureInfo {
        std::string tag;
        GLuint ID;
    };

    TextureManager();   // Constructor
    ~TextureManager();  // Destructor

    // Loads a texture from file and associates it with a tag
    bool LoadTexture(const char* filename, const std::string& tag);

    // Binds all loaded textures to their respective texture units
    void BindTextures();

    // Deletes all loaded textures from OpenGL and clears the list
    void DestroyTextures();

    // Finds the OpenGL texture ID by tag
    int FindTextureID(const std::string& tag) const;

    // Finds the texture slot (unit) by tag
    int FindTextureSlot(const std::string& tag) const;

private:
    // Stores all loaded textures and their tags
    std::vector<TextureInfo> m_textures;
};
