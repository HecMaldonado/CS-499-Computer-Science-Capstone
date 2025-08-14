/***********************************************************
 *  MaterialManager
 *
 *  This new class was added to modularize and centralize
 *  all material definition and lookup operations.
 *  This replaces the previous approach where SceneManager
 *  handled materials directly, improving modularity and
 *  maintainability.
 ***********************************************************/

#pragma once
#include <string>
#include <vector>
#include <glm/glm.hpp>


struct ObjectMaterial {
    float ambientStrength = 0.0f;
    glm::vec3 ambientColor = glm::vec3(0.0f);
    glm::vec3 diffuseColor = glm::vec3(0.0f);
    glm::vec3 specularColor = glm::vec3(0.0f);
    float shininess = 0.0f;
    std::string tag;
};

class MaterialManager {
public:
    // Adds a new material to the manager
    void AddMaterial(const ObjectMaterial& material);

    // Finds a material by tag and returns it via reference
    bool FindMaterial(const std::string& tag, ObjectMaterial& material) const;

    // Clears all stored materials
    void Clear();

private:
    // Stores all defined materials
    std::vector<ObjectMaterial> m_materials;
};
