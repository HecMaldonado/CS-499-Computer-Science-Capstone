/***********************************************************
 *
 *  Octree.h
 *	============
 *  manage the viewing of 3D objects within the viewport
 *
 ***********************************************************/

#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <string>

// Axis-aligned bounding box 
// --- Enhancement: Axis-aligned bounding box for spatial partitioning (Octree) ---

struct AABB {
    glm::vec3 min;
    glm::vec3 max;

    bool contains(const glm::vec3& point) const {
        return (point.x >= min.x && point.x <= max.x &&
            point.y >= min.y && point.y <= max.y &&
            point.z >= min.z && point.z <= max.z);
    }

    bool intersects(const AABB& other) const {
        return (min.x <= other.max.x && max.x >= other.min.x) &&
            (min.y <= other.max.y && max.y >= other.min.y) &&
            (min.z <= other.max.z && max.z >= other.min.z);
    }
};


// --- Enhancement: Minimal scene object for Octree spatial partitioning ---
struct SceneObject {
    glm::vec3 position;
    float boundingRadius;
    std::string tag;
    // Add a pointer or reference to your mesh/object data as needed
    // e.g., int meshID; or ShapeMeshes* mesh;
};


// --- Enhancement: Octree node for efficient spatial partitioning and culling ---
class OctreeNode {
public:
    AABB bounds;
    std::vector<SceneObject*> objects;
    OctreeNode* children[8] = { nullptr };
    int depth;
    static const int MAX_OBJECTS = 8;
    static const int MAX_DEPTH = 5;


    // --- Enhancement: Octree constructor for spatial partitioning ---
    OctreeNode(const AABB& bounds, int depth = 0);
    ~OctreeNode();

    // --- Enhancement: Insert object into Octree for spatial partitioning ---
    void insert(SceneObject* obj);

    // --- Enhancement: Subdivide Octree node for finer partitioning ---
    void subdivide();

    // --- Enhancement: Query Octree for objects within a region (e.g., camera frustum) ---
    void query(const AABB& range, std::vector<SceneObject*>& found);
};
