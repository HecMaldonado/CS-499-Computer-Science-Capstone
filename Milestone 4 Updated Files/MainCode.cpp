/***********************************************************
 *
 *  MainCode.cpp
 *	============
 *  
 *
 ***********************************************************/

#include <iostream>         // error handling and output
#include <cstdlib>          // EXIT_FAILURE
#include <fstream>			// file input/output

#include <GL/glew.h>        // GLEW library
#include "GLFW/glfw3.h"     // GLFW library

// GLM Math Header inclusions
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../TextureManager.h"
#include "../MaterialManager.h"
#include "SceneManager.h"
#include "ViewManager.h"
#include "ShapeMeshes.h"
#include "ShaderManager.h"



// Namespace for declaring global variables
namespace
{
	// Macro for window title
	const char* const WINDOW_TITLE = "7-1 FinalProject and Milestones";

	// Main GLFW window
	GLFWwindow* g_Window = nullptr;

	// scene manager object for managing the 3D scene prepare and render
	SceneManager* g_SceneManager = nullptr;
	// shader manager object for dynamic interaction with the shader code
	ShaderManager* g_ShaderManager = nullptr;
	// view manager object for managing the 3D view setup and projection to 2D
	ViewManager* g_ViewManager = nullptr;
}

// Function declarations - all functions that are called manually
// need to be pre-declared at the beginning of the source code.
bool InitializeGLFW();
bool InitializeGLEW();


/***********************************************************
 *  main(int, char*)
 *
 *  This function gets called after the application has been
 *  launched.
 ***********************************************************/
int main(int argc, char* argv[])
{
	// if GLFW fails initialization, then terminate the application
	if (InitializeGLFW() == false)
	{
		return(EXIT_FAILURE);
	}

	// try to create a new shader manager object
	g_ShaderManager = new ShaderManager();
	// try to create a new view manager object
	g_ViewManager = new ViewManager(
		g_ShaderManager);

	// try to create the main display window
	g_Window = g_ViewManager->CreateDisplayWindow(WINDOW_TITLE);

	// if GLEW fails initialization, then terminate the application
	if (InitializeGLEW() == false)
	{
		return(EXIT_FAILURE);
	}

	// load the shader code from the external GLSL files
	g_ShaderManager->LoadShaders(
		"../../Utilities/shaders/vertexShader.glsl",
		"../../Utilities/shaders/fragmentShader.glsl");
	g_ShaderManager->use();

	// try to create a new scene manager object and prepare the 3D scene
	// The SceneManager now uses the new TextureManager and MaterialManager
	TextureManager* g_TextureManager = new TextureManager();
	MaterialManager* g_MaterialManager = new MaterialManager();
	g_SceneManager = new SceneManager(g_ShaderManager, g_TextureManager, g_MaterialManager);


/***********************************************************
 *						*** ENHANCEMENT ***
 *						*** MILESTONE 4 ***
 *				Enhancement: Save/Load from JSON
 *				
 *
 *
 ***********************************************************/

	g_SceneManager->PrepareScene(); 

	// Enhancement: Always initialize the scene, 
	// then optionally load from JSON if a save file exists.
	std::ifstream infile("scene_save.json");
	if (infile.good()) {
		g_SceneManager->LoadSceneAndCameraFromJson("scene_save.json");
		std::cout << "Loaded scene and camera from scene_save.json" << std::endl;
	}

	// Enhancement: Loads the scene and camera from JSON if available,
	// otherwise loads the default scene.
	else {
		g_SceneManager->PrepareScene();
		std::cout << "No save found, loaded default scene." << std::endl;
	}

	// loop will keep running until the application is closed 
	// or until an error has occurred

	while (!glfwWindowShouldClose(g_Window))
	{
		glEnable(GL_DEPTH_TEST);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		g_ViewManager->PrepareSceneView();
		g_SceneManager->RenderScene();

		// --- Enhancement: This block for save/load functionality ---
		if (glfwGetKey(g_Window, GLFW_KEY_F5) == GLFW_PRESS) {
			g_SceneManager->SaveSceneAndCameraToJson("scene_save.json");
			std::cout << "Scene and camera saved to scene_save.json" << std::endl;
		}
		if (glfwGetKey(g_Window, GLFW_KEY_F9) == GLFW_PRESS) {
			g_SceneManager->LoadSceneAndCameraFromJson("scene_save.json");
			std::cout << "Scene and camera loaded from scene_save.json" << std::endl;
		}
		// --------------------------------------------------
		//					Enhancement
		// Press F5 to save the scene and camera to JSON,
		// F9 to load them from JSON at runtime.
		// 
		// -------------   End of Enhancement	-------------

		glfwSwapBuffers(g_Window);
		glfwPollEvents();
	}


	// clear the allocated manager objects from memory
	if (NULL != g_SceneManager)
	{
		delete g_SceneManager;
		g_SceneManager = NULL;
	}
	if (NULL != g_ViewManager)
	{
		delete g_ViewManager;
		g_ViewManager = NULL;
	}
	if (NULL != g_ShaderManager)
	{
		delete g_ShaderManager;
		g_ShaderManager = NULL;
	}

	// Terminates the program successfully
	exit(EXIT_SUCCESS);
}

/***********************************************************
 *	InitializeGLFW()
 *
 *  This function is used to initialize the GLFW library.
 ***********************************************************/
bool InitializeGLFW()
{
	// GLFW: initialize and configure library
	// --------------------------------------
	glfwInit();

#ifdef __APPLE__
	// set the version of OpenGL and profile to use
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#else
	// set the version of OpenGL and profile to use
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif
	// GLFW: end -------------------------------

	return(true);
}

/***********************************************************
 *	InitializeGLEW()
 *
 *  This function is used to initialize the GLEW library.
 ***********************************************************/
bool InitializeGLEW()
{
	// GLEW: initialize
	// -----------------------------------------
	GLenum GLEWInitResult = GLEW_OK;

	// try to initialize the GLEW library
	GLEWInitResult = glewInit();
	if (GLEW_OK != GLEWInitResult)
	{
		std::cerr << glewGetErrorString(GLEWInitResult) << std::endl;
		return false;
	}
	// GLEW: end -------------------------------

	// Displays a successful OpenGL initialization message
	std::cout << "INFO: OpenGL Successfully Initialized\n";
	std::cout << "INFO: OpenGL Version: " << glGetString(GL_VERSION) << "\n" << std::endl;

	return(true);
}
