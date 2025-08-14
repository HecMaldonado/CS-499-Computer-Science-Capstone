#pragma once
#pragma once
#include <string>
#include <vector>
#include <fstream>
#include <glm/glm.hpp>
#include "json.hpp"
#include "Octree.h"

/***********************************************************
 *				    *** ENHANCEMENT ***
 *                  *** Milestone 4 ***
 *
 *
 ***********************************************************/

// --- Camera state struct for saving/loading ---
// Enhancement: CameraState struct added to support saving/loading camera
// position, orientation, and zoom.

// --- Save/load scene + camera ---
// Enhancement: Adds methods to save/load both scene objects and camera state to/from JSON.

struct CameraState {
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    float zoom;
};


class JsonDatabase {
public:
    static bool SaveSceneObjects(const std::vector<SceneObject>& objects, const std::string& filename);
    static bool LoadSceneObjects(std::vector<SceneObject>& objects, const std::string& filename);

    // --- Save/load scene + camera ---
    static bool SaveSceneAndCamera(const std::vector<SceneObject>& objects, const CameraState& camera, const std::string& filename);
    static bool LoadSceneAndCamera(std::vector<SceneObject>& objects, CameraState& camera, const std::string& filename);
};
