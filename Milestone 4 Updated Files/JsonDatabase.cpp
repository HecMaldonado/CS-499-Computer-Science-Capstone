#include "JsonDatabase.h"
#include <fstream>
#include <vector>
#include <glm/glm.hpp>

/***********************************************************
 *					*** ENHANCEMENT ***
 *                  *** MILESTONE 4 ***
 *
 *
 *
 ***********************************************************/


// Create a convenient alias 'json' for the nlohmann::json class, 
// allowing us to use 'json' instead of the longer 'nlohmann::json' throughout this file.
// This makes the code cleaner and easier to read when working
// with JSON serialization and deserialization.

using json = nlohmann::json;

// --- SceneObject serialization ---
// Enhancement: Adds serialization functions for SceneObject
// to enable JSON save/load.

static void to_json(json& j, const SceneObject& obj) {
    j = json{
        {"position", {obj.position.x, obj.position.y, obj.position.z}},
        {"boundingRadius", obj.boundingRadius},
        {"tag", obj.tag}
    };
}
static void from_json(const json& j, SceneObject& obj) {
    auto pos = j.at("position");
    obj.position = glm::vec3(pos[0], pos[1], pos[2]);
    obj.boundingRadius = j.at("boundingRadius").get<float>();
    obj.tag = j.at("tag").get<std::string>();
}

// --- CameraState serialization ---
// Enhancement: Adds serialization functions for CameraState
// to enable JSON save/load of camera state.

static void to_json(json& j, const CameraState& cam) {
    j = json{
        {"position", {cam.position.x, cam.position.y, cam.position.z}},
        {"front", {cam.front.x, cam.front.y, cam.front.z}},
        {"up", {cam.up.x, cam.up.y, cam.up.z}},
        {"zoom", cam.zoom}
    };
}
static void from_json(const json& j, CameraState& cam) {
    auto pos = j.at("position");
    cam.position = glm::vec3(pos[0], pos[1], pos[2]);
    auto front = j.at("front");
    cam.front = glm::vec3(front[0], front[1], front[2]);
    auto up = j.at("up");
    cam.up = glm::vec3(up[0], up[1], up[2]);
    cam.zoom = j.at("zoom").get<float>();
}

// --- Save/load scene objects only ---
// Enhancement: Implements saving/loading
// of scene objects to/from JSON.

bool JsonDatabase::SaveSceneObjects(const std::vector<SceneObject>& objects, const std::string& filename) {
    json j = objects;
    std::ofstream file(filename);
    if (!file.is_open()) return false;
    file << j.dump(4);
    return true;
}

bool JsonDatabase::LoadSceneObjects(std::vector<SceneObject>& objects, const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) return false;
    json j;
    try {
        file >> j;
        objects = j.get<std::vector<SceneObject>>();
    }
    catch (...) {
        return false;
    }
    return true;
}

// --- Save/load scene + camera ---
// Enhancement: Implements saving/loading of both
// scene objects and camera state to/from JSON.

bool JsonDatabase::SaveSceneAndCamera(const std::vector<SceneObject>& objects, const CameraState& camera, const std::string& filename) {
    json j;
    j["objects"] = objects;
    j["camera"] = camera;
    std::ofstream file(filename);
    if (!file.is_open()) return false;
    file << j.dump(4);
    return true;
}

bool JsonDatabase::LoadSceneAndCamera(std::vector<SceneObject>& objects, CameraState& camera, const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) return false;
    json j;
    try {
        file >> j;
        objects = j.at("objects").get<std::vector<SceneObject>>();
        camera = j.at("camera").get<CameraState>();
    }
    catch (...) {
        return false;
    }
    return true;
}
