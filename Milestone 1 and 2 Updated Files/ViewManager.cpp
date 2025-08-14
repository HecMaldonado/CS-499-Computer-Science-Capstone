///////////////////////////////////////////////////////////////////////////////
// viewmanager.h
// ============
// manage the viewing of 3D objects within the viewport
//
//  AUTHOR: Brian Battersby - SNHU Instructor / Computer Science
//	Created for CS-330-Computational Graphics and Visualization, Nov. 1st, 2023
///////////////////////////////////////////////////////////////////////////////

#include "ViewManager.h"

// GLM Math Header inclusions
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>    

// declaration of the global variables and defines
namespace
{
	// Variables for window width and height
	const int WINDOW_WIDTH = 1000;
	const int WINDOW_HEIGHT = 800;
	const char* g_ViewName = "view";
	const char* g_ProjectionName = "projection";

	// camera object used for viewing and interacting with
	// the 3D scene
	Camera* g_pCamera = nullptr;

	// these variables are used for mouse movement processing
	float gLastX = WINDOW_WIDTH / 2.0f;
	float gLastY = WINDOW_HEIGHT / 2.0f;
	bool gFirstMouse = true;

	// time between current frame and last frame
	float gDeltaTime = 0.0f;
	float gLastFrame = 0.0f;

	// if orthographic projection is on, this value will be
	// true
	bool bOrthographicProjection = false;

	// Movement speed controls
	float gMovementSpeed = 2.0f;
	float gMouseSensitivity = 3.5f;
	float gScrollSensitivity = 0.1f;

	// Orbit controls
	bool gOrbitMode = false;
	float gOrbitRadius = 10.0f;
	float gOrbitAngle = 0.0f;
}

/***********************************************************
 *  ViewManager()
 *
 *  The constructor for the class
 ***********************************************************/
ViewManager::ViewManager(
	ShaderManager* pShaderManager)
{
	// initialize the member variables
	m_pShaderManager = pShaderManager;
	m_pWindow = NULL;
	g_pCamera = new Camera();

	// default camera view parameters
	g_pCamera->Position = glm::vec3(0.8f, 0.6f, 3.5f);
	g_pCamera->Front = glm::vec3(-1.3f, -1.8f, -15.0f);
	g_pCamera->Up = glm::vec3(0.0f, 11.0f, 0.0f);
	g_pCamera->Zoom = 80;
}

/***********************************************************
 *  ~ViewManager()
 *
 *  The destructor for the class
 ***********************************************************/
ViewManager::~ViewManager()
{
	// free up allocated memory
	m_pShaderManager = NULL;
	m_pWindow = NULL;
	if (NULL != g_pCamera)
	{
		delete g_pCamera;
		g_pCamera = NULL;
	}
}

/***********************************************************
 *  CreateDisplayWindow()
 *
 *  This method is used to create the main display window.
 ***********************************************************/
GLFWwindow* ViewManager::CreateDisplayWindow(const char* windowTitle)
{
	GLFWwindow* window = nullptr;

	// try to create the displayed OpenGL window
	window = glfwCreateWindow(
		WINDOW_WIDTH,
		WINDOW_HEIGHT,
		windowTitle,
		NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return NULL;
	}
	glfwMakeContextCurrent(window);

	// Add mouse scroll callback
	glfwSetScrollCallback(window, &ViewManager::Mouse_Scroll_Callback);

	// Add this line to register the mouse position callback
	glfwSetCursorPosCallback(window, &ViewManager::Mouse_Position_Callback);
	glfwSetScrollCallback(window, &ViewManager::Mouse_Scroll_Callback);

	// Enable cursor for camera orientation control
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);


	m_pWindow = window;

	return(window);
}

/***********************************************************
 *  Mouse_Position_Callback()
 *
 *  This method is automatically called from GLFW whenever
 *  the mouse is moved within the active GLFW display window.
 ***********************************************************/
void ViewManager::Mouse_Scroll_Callback(GLFWwindow* window, double xOffset, double yOffset)
{
	// Adjust movement speed based on scroll wheel
	// Positive yOffset means scroll up, negative means scroll down
	gMovementSpeed += static_cast<float>(yOffset) * 0.2f;

	// Clamp movement speed between reasonable values
	if (gMovementSpeed < 0.5f) gMovementSpeed = 0.5f;
	if (gMovementSpeed > 10.0f) gMovementSpeed = 10.0f;
}


void ViewManager::Mouse_Position_Callback(GLFWwindow* window, double xMousePos, double yMousePos)
{
	// when the first mouse move event is received, this needs to be recorded so that
	// all subsequent mouse moves can correctly calculate the X position offset and Y
	// position offset for proper operation
	// Initialize first mouse movement
	if (gFirstMouse)
	{
		gLastX = xMousePos;
		gLastY = yMousePos;
		gFirstMouse = false;
		return;
	}

	// Calculate mouse movement offset
	float xOffset = (xMousePos - gLastX) * gMouseSensitivity;
	float yOffset = (gLastY - yMousePos) * gMouseSensitivity;

	// Update last known position
	gLastX = xMousePos;
	gLastY = yMousePos;

	// Apply sensitivity after calculating offset
	xOffset *= gMouseSensitivity;
	yOffset *= gMouseSensitivity;

	// Check if left mouse button is pressed for camera rotation
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		if (gOrbitMode)
		{
			// Update orbit angle and position
			gOrbitAngle += xOffset;
			float x = sin(glm::radians(gOrbitAngle)) * gOrbitRadius;
			float z = cos(glm::radians(gOrbitAngle)) * gOrbitRadius;
			g_pCamera->Position = glm::vec3(x, g_pCamera->Position.y, z);
			g_pCamera->Front = -glm::normalize(g_pCamera->Position);
		}
		else
		{
			// Regular camera rotation
			g_pCamera->ProcessMouseMovement(xOffset, yOffset);
		}
	}

}

/***********************************************************
 *  ProcessKeyboardEvents()
 *
 *  This method is called to process any keyboard events
 *  that may be waiting in the event queue.
 ***********************************************************/
void ViewManager::ProcessKeyboardEvents()
{
	// close the window if the escape key has been pressed
	if (glfwGetKey(m_pWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(m_pWindow, true);
	}

	// if the camera object is null, then exit this method
	if (NULL == g_pCamera)
	{
		return;
	}

	float currentSpeed = gMovementSpeed * gDeltaTime;

	// WASD keys for forward/backward/left/right movement
	if (glfwGetKey(m_pWindow, GLFW_KEY_W) == GLFW_PRESS)
	{
		g_pCamera->ProcessKeyboard(FORWARD, currentSpeed);
	}
	if (glfwGetKey(m_pWindow, GLFW_KEY_S) == GLFW_PRESS)
	{
		g_pCamera->ProcessKeyboard(BACKWARD, currentSpeed);
	}
	if (glfwGetKey(m_pWindow, GLFW_KEY_A) == GLFW_PRESS)
	{
		g_pCamera->ProcessKeyboard(LEFT, currentSpeed);
	}
	if (glfwGetKey(m_pWindow, GLFW_KEY_D) == GLFW_PRESS)
	{
		g_pCamera->ProcessKeyboard(RIGHT, currentSpeed);
	}

	// QE keys for up/down movement
	if (glfwGetKey(m_pWindow, GLFW_KEY_Q) == GLFW_PRESS)
	{
		g_pCamera->Position += g_pCamera->Up * currentSpeed;
	}
	if (glfwGetKey(m_pWindow, GLFW_KEY_E) == GLFW_PRESS)
	{
		g_pCamera->Position -= g_pCamera->Up * currentSpeed;
	}

	// Toggle orbit mode with O key
	if (glfwGetKey(m_pWindow, GLFW_KEY_O) == GLFW_PRESS)
	{
		gOrbitMode = !gOrbitMode;
	}
}

/***********************************************************
 *  PrepareSceneView()
 *
 *  This method is used for preparing the 3D scene by loading
 *  the shapes, textures in memory to support the 3D scene
 *  rendering
 ***********************************************************/
void ViewManager::PrepareSceneView()
{
	glm::mat4 view;
	glm::mat4 projection;

	// per-frame timing
	float currentFrame = glfwGetTime();
	gDeltaTime = currentFrame - gLastFrame;
	gLastFrame = currentFrame;

	// process any keyboard events that may be waiting in the 
	// event queue
	ProcessKeyboardEvents();

	// get the current view matrix from the camera
	view = g_pCamera->GetViewMatrix();

	// define the current projection matrix
	projection = glm::perspective(glm::radians(g_pCamera->Zoom), (GLfloat)WINDOW_WIDTH / (GLfloat)WINDOW_HEIGHT, 0.1f, 100.0f);

	// if the shader manager object is valid
	if (NULL != m_pShaderManager)
	{
		// set the view matrix into the shader for proper rendering
		m_pShaderManager->setMat4Value(g_ViewName, view);
		// set the view matrix into the shader for proper rendering
		m_pShaderManager->setMat4Value(g_ProjectionName, projection);
		// set the view position of the camera into the shader for proper rendering
		m_pShaderManager->setVec3Value("viewPosition", g_pCamera->Position);
	}
}