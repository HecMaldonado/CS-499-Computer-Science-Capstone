# CS-499 Enhancement Project

## Overview

This project is an OpenGL-based 3D scene renderer, originally developed in October 2024. It demonstrates modular C++ design, real-time rendering, and interactive camera controls. The codebase is organized into several classes, including `SceneManager`, `ShaderManager`, `ViewManager`, `TextureManager`, and `MaterialManager`.

## Planned Enhancements

### 1. Modular Resource Management

- **Refactor texture and material handling:**  
  Move texture and material management out of `SceneManager` and into dedicated `TextureManager` and `MaterialManager` classes.
- **Benefits:**  
  - Improved code modularity and separation of concerns  
  - Easier to extend and maintain resource management  
  - Better support for collaborative development

### 2. Design Pattern Implementation

- **Apply design patterns:**  
  Implement Octree Spatial Partitioning and Code Quality Improvements
- **Benefits:**  
  - Added octree-based spatial partitioning for efficient scene object culling and improved rendering performance.
  - Refactored code to initialize all variables and struct members, reducing potential bugs and compiler warnings.
  - Moved large local arrays to heap allocation to enhance stability and portability.
  - Overall, these changes make the project more robust, maintainable, and performant.

### 3. Enhanced Documentation

- **Add in-line comments documentation:**  
  Improve code readability and maintainability for future contributors.

### 4. Database Integration (Planned)

- **Scene persistence:**  
  Integrate a lightweight database (e.g., SQLite) to allow saving and loading of scene configurations.
- **Benefits:**  
  - Enables persistent scene editing  
  - Demonstrates database integration skills

### 5. Improved Error Handling

- **Robust error checking:**  
  Add comprehensive error handling for resource loading and OpenGL operations.

## Getting Started

1. Clone the repository.
2. Install dependencies (GLEW, GLFW, GLM, stb_image).
3. Build the project using your preferred C++ IDE or build system.
4. Run the executable to view the 3D scene.

## Project Structure
  /Utilities 
  /shaders 
  /textures 
    ShaderManager.cpp/h 
    TextureManager.cpp/h 
    MaterialManager.cpp/h
    Octree.cpp/h
  /Source 
    MainCode.cpp 
    SceneManager.cpp/h 
    ViewManager.cpp/h 
    ShapeMeshes.cpp/h

---

> **Note:** This project is part of my CS-499 ePortfolio and is actively being enhanced to demonstrate advanced software engineering skills.
