/***********************************************************
 *
 *  Octree.cpp
 *	============
 *  manage the viewing of 3D objects within the viewport
 *
 ***********************************************************/

#include "Octree.h"


// --- Enhancement: Octree constructor implementation ---
OctreeNode::OctreeNode(const AABB& bounds, int depth)
    : bounds(bounds), depth(depth) {
}


// --- Enhancement: Octree destructor to clean up children ---
OctreeNode::~OctreeNode() {
    for (int i = 0; i < 8; ++i)
        if (children[i]) delete children[i];
}


// --- Enhancement: Insert object into Octree for spatial partitioning ---
void OctreeNode::insert(SceneObject* obj) {
    if (depth == MAX_DEPTH) {
        objects.push_back(obj);
        return;
    }
    if (objects.size() < MAX_OBJECTS) {
        objects.push_back(obj);
        return;
    }
    if (!children[0]) subdivide();
    for (int i = 0; i < 8; ++i) {
        if (children[i]->bounds.contains(obj->position)) {
            children[i]->insert(obj);
            return;
        }
    }
    // If object doesn't fit in any child, keep it here
    objects.push_back(obj);
}


// --- Enhancement: Subdivide Octree node for finer partitioning ---
void OctreeNode::subdivide() {
    glm::vec3 size = (bounds.max - bounds.min) * 0.5f;
    glm::vec3 center = bounds.min + size;
    for (int i = 0; i < 8; ++i) {
        glm::vec3 offset(
            (i & 1) ? size.x : 0,
            (i & 2) ? size.y : 0,
            (i & 4) ? size.z : 0
        );
        AABB childBounds = {
            bounds.min + offset,
            bounds.min + offset + size
        };
        children[i] = new OctreeNode(childBounds, depth + 1);
    }
    // Move existing objects into children
    for (auto obj : objects) {
        for (int i = 0; i < 8; ++i) {
            if (children[i]->bounds.contains(obj->position)) {
                children[i]->insert(obj);
                break;
            }
        }
    }
    objects.clear();
}


// --- Enhancement: Query Octree for objects within a region (e.g., camera frustum) ---
void OctreeNode::query(const AABB& range, std::vector<SceneObject*>& found) {
    if (!bounds.intersects(range)) return;
    for (auto obj : objects) {
        if (range.contains(obj->position))
            found.push_back(obj);
    }
    if (!children[0]) return;
    for (int i = 0; i < 8; ++i)
        children[i]->query(range, found);
}
