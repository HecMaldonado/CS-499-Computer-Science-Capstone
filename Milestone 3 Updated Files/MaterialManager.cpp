/***********************************************************
 *  MaterialManager Implementation
 *
 *  Implements all material management logic, moved out of
 *  SceneManager for better modularity and code reuse.
 ***********************************************************/

#include "MaterialManager.h"


void MaterialManager::AddMaterial(const ObjectMaterial& material) {
    // Adds a new material to the internal list
    m_materials.push_back(material);
}

bool MaterialManager::FindMaterial(const std::string& tag, ObjectMaterial& material) const {
    // Searches for a material by tag and copies it to the output parameter if found
    for (const auto& mat : m_materials) {
        if (mat.tag == tag) {
            material = mat;
            return true;
        }
    }
    return false;
}

void MaterialManager::Clear() {
    // Removes all materials from the manager
    m_materials.clear();
}
