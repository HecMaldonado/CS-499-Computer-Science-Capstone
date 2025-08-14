/***********************************************************
 * 
 *	Southern New Hampshire University
 *  CS-499: CS Capstone Enhancement Project
 *  Professor: Ramsey Kraya
 *  Name: Hector Maldonado 
 *
 ***********************************************************/

 /***********************************************************
  *						*** UPDATED ***
  *
  * 
  *
  ***********************************************************/

#include "SceneManager.h"

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#endif

#include <glm/gtx/transform.hpp>

// declaration of global variables
namespace
{
	const char* g_ModelName = "model";
	const char* g_ColorValueName = "objectColor";
	const char* g_TextureValueName = "objectTexture";
	const char* g_UseTextureName = "bUseTexture";
	const char* g_UseLightingName = "bUseLighting";
}

/***********************************************************
 *  SceneManager()
 *
 *  The constructor for the class
 ***********************************************************/


SceneManager::SceneManager(ShaderManager* pShaderManager, TextureManager* pTextureManager, MaterialManager* pMaterialManager)
{
	m_pShaderManager = pShaderManager;
	m_textureManager = pTextureManager;
	m_materialManager = pMaterialManager;
	m_basicMeshes = new ShapeMeshes();
}

/***********************************************************
 *  ~SceneManager()
 *
 *  The destructor for the class
 ***********************************************************/


SceneManager::~SceneManager()
{
	m_pShaderManager = NULL;
	delete m_basicMeshes;
	m_basicMeshes = NULL;
}

/***********************************************************
 *						*** UPDATED ***
 * 
 *  CreateGLTexture(), BindGLTextures(), DestroyGLTextures(), FindTextureID(),
 *		 FindTextureSlot(), FindMaterial()
 *
 *	REMOVED TO USE NEW MANAGERS/Classes
 *
 ***********************************************************/



 /***********************************************************
  *  SetTransformations()
  *
  *  This method is used for setting the transform buffer
  *  using the passed in transformation values.
  ***********************************************************/


void SceneManager::SetTransformations(
	glm::vec3 scaleXYZ,
	float XrotationDegrees,
	float YrotationDegrees,
	float ZrotationDegrees,
	glm::vec3 positionXYZ)
{
	// variables for this method
	glm::mat4 modelView;
	glm::mat4 scale;
	glm::mat4 rotationX;
	glm::mat4 rotationY;
	glm::mat4 rotationZ;
	glm::mat4 translation;

	// set the scale value in the transform buffer
	scale = glm::scale(scaleXYZ);
	// set the rotation values in the transform buffer
	rotationX = glm::rotate(glm::radians(XrotationDegrees), glm::vec3(1.0f, 0.0f, 0.0f));
	rotationY = glm::rotate(glm::radians(YrotationDegrees), glm::vec3(0.0f, 1.0f, 0.0f));
	rotationZ = glm::rotate(glm::radians(ZrotationDegrees), glm::vec3(0.0f, 0.0f, 1.0f));
	// set the translation value in the transform buffer
	translation = glm::translate(positionXYZ);

	modelView = translation * rotationX * rotationY * rotationZ * scale;

	if (NULL != m_pShaderManager)
	{
		m_pShaderManager->setMat4Value(g_ModelName, modelView);
	}
}

/***********************************************************
 *						*** UPDATED ***
 * 
 *  SetShaderColor()
 *
 *  This method is used for setting the passed in color
 *  into the shader for the next draw command
 ***********************************************************/


void SceneManager::SetShaderColor(
	float redColorValue,
	float greenColorValue,
	float blueColorValue,
	float alphaValue)
{
	// variables for this method
	glm::vec4 currentColor;

	currentColor.r = redColorValue;
	currentColor.g = greenColorValue;
	currentColor.b = blueColorValue;
	currentColor.a = alphaValue;

	if (NULL != m_pShaderManager)
	{
		m_pShaderManager->setIntValue(g_UseTextureName, false);
		m_pShaderManager->setVec4Value(g_ColorValueName, currentColor);
	}
}

/***********************************************************
 *						*** UPDATED ***
 * 
 *  SetShaderTexture()
 *
 *  This method is used for setting the texture data
 *  associated with the passed in ID into the shader.
 ***********************************************************/


void SceneManager::SetShaderTexture(std::string textureTag)
{
	// Use the new TextureManager to look up the texture slot by tag,
	// instead of managing textures directly in SceneManager.
	// This improves modularity and makes texture management reusable and maintainable.
	if (NULL != m_pShaderManager)
	{
		// Enable texture usage in the shader
		m_pShaderManager->setIntValue(g_UseTextureName, true);

		// Find the texture slot (unit) for the given tag using TextureManager
		int textureSlot = m_textureManager->FindTextureSlot(textureTag);

		// Set the shader's sampler2D uniform to use the correct texture slot
		m_pShaderManager->setSampler2DValue(g_TextureValueName, textureSlot);
	}
}


/***********************************************************
 *  SetTextureUVScale()
 *
 *  This method is used for setting the texture UV scale
 *  values into the shader.
 ***********************************************************/


void SceneManager::SetTextureUVScale(float u, float v)
{
	if (NULL != m_pShaderManager)
	{
		m_pShaderManager->setVec2Value("UVscale", glm::vec2(u, v));
	}
}


/***********************************************************
 *						*** UPDATED ***
 * 
 *  LoadSceneTextures()
 *
 *  This method is used for preparing the 3D scene by loading
 *  the shapes, textures in memory to support the 3D scene
 *  rendering
 ***********************************************************/


void SceneManager::LoadSceneTextures()
{
	// Changed to use the new TextureManager class for loading and binding textures,
	// instead of managing texture loading and storage directly in SceneManager.
	// This improves modularity, reusability, and maintainability of texture handling.
	m_textureManager->LoadTexture("../../Utilities/textures/polkadots.jpg", "polkadots");
	m_textureManager->LoadTexture("../../Utilities/textures/floor.jpg", "floor");
	m_textureManager->LoadTexture("../../Utilities/textures/pompom.jpg", "pompom");
	m_textureManager->LoadTexture("../../Utilities/textures/woodcar.jpg", "woodcar");
	m_textureManager->LoadTexture("../../Utilities/textures/yellow.jpg", "yellow");
	m_textureManager->LoadTexture("../../Utilities/textures/red.jpg", "red");
	m_textureManager->LoadTexture("../../Utilities/textures/newgreen.jpg", "green");
	m_textureManager->LoadTexture("../../Utilities/textures/carpetbeige.jpg", "carpetbeige");
	m_textureManager->LoadTexture("../../Utilities/textures/carpetblue.jpg", "carpetblue");
	m_textureManager->LoadTexture("../../Utilities/textures/purple.jpg", "purple");
	m_textureManager->LoadTexture("../../Utilities/textures/drywall2.jpg", "drywall2");
	m_textureManager->BindTextures();
}


/***********************************************************
 *						*** UPDATED ***
 * 
 * 
 *  SetShaderMaterial()
 *
 *  This method is used for passing the material values
 *  into the shader.
***********************************************************/


void SceneManager::SetShaderMaterial(std::string materialTag)
{
	// Changed to use the new MaterialManager class for material lookup,
	// instead of managing materials directly in SceneManager.
	// This improves modularity, reusability, and maintainability of material handling.
	ObjectMaterial material;
	if (m_materialManager->FindMaterial(materialTag, material))
	{
		m_pShaderManager->setVec3Value("material.ambientColor", material.ambientColor);
		m_pShaderManager->setFloatValue("material.ambientStrength", material.ambientStrength);
		m_pShaderManager->setVec3Value("material.diffuseColor", material.diffuseColor);
		m_pShaderManager->setVec3Value("material.specularColor", material.specularColor);
		m_pShaderManager->setFloatValue("material.shininess", material.shininess);
	}
}


/**************************************************************
 *						*** UPDATED ***
 * 
 * Changed from using the old OBJECT_MATERIAL struct defined in SceneManager
 * to the new ObjectMaterial struct defined in MaterialManager.h.
 * This improves modularity by centralizing material definitions and management
 * in a dedicated MaterialManager class, making the codebase easier to maintain and extend.
 *
**************************************************************/


void SceneManager::DefineObjectMaterials()
{
	// Changed from OBJECT_MATERIAL to ObjectMaterial for improved modularity and maintainability.
	ObjectMaterial toyMaterial;
	toyMaterial.ambientColor = glm::vec3(0.1f, 0.1f, 0.1f);
	toyMaterial.ambientStrength = 0.1f;
	toyMaterial.diffuseColor = glm::vec3(0.3f, 0.3f, 0.3f);
	toyMaterial.specularColor = glm::vec3(0.1f, 0.1f, 0.1f);
	toyMaterial.shininess = 8.0f;
	toyMaterial.tag = "toy";
	m_materialManager->AddMaterial(toyMaterial);

	ObjectMaterial carpetMaterial;
	carpetMaterial.ambientColor = glm::vec3(0.1f, 0.1f, 0.1f);
	carpetMaterial.ambientStrength = 0.15f;
	carpetMaterial.diffuseColor = glm::vec3(0.2f, 0.2f, 0.2f);
	carpetMaterial.specularColor = glm::vec3(0.0f, 0.0f, 0.0f);
	carpetMaterial.shininess = 1.0f;
	carpetMaterial.tag = "carpet";
	m_materialManager->AddMaterial(carpetMaterial);

	ObjectMaterial hatMaterial;
	hatMaterial.ambientColor = glm::vec3(0.2f, 0.2f, 0.2f);
	hatMaterial.ambientStrength = 0.3f;
	hatMaterial.diffuseColor = glm::vec3(0.6f, 0.6f, 0.6f);
	hatMaterial.specularColor = glm::vec3(0.0f, 0.0f, 0.0f);
	hatMaterial.shininess = 1.0f;
	hatMaterial.tag = "hat";
	m_materialManager->AddMaterial(hatMaterial);

	ObjectMaterial blockMaterial;
	blockMaterial.ambientColor = glm::vec3(0.15f, 0.15f, 0.15f);
	blockMaterial.ambientStrength = 0.3f;
	blockMaterial.diffuseColor = glm::vec3(0.6f, 0.6f, 0.6f);
	blockMaterial.specularColor = glm::vec3(0.1f, 0.1f, 0.1f);
	blockMaterial.shininess = 8.0f;
	blockMaterial.tag = "block";
	m_materialManager->AddMaterial(blockMaterial);

	ObjectMaterial woodMaterial;
	woodMaterial.ambientColor = glm::vec3(0.2f, 0.15f, 0.1f);
	woodMaterial.ambientStrength = 0.3f;
	woodMaterial.diffuseColor = glm::vec3(0.6f, 0.45f, 0.3f);
	woodMaterial.specularColor = glm::vec3(0.1f, 0.1f, 0.1f);
	woodMaterial.shininess = 4.0f;
	woodMaterial.tag = "wood";
	m_materialManager->AddMaterial(woodMaterial);
}

void SceneManager::SetupSceneLights()
{
	// Enable lighting system in the shader
	m_pShaderManager->setBoolValue(g_UseLightingName, true);

	// Main ceiling light (warm white) - primary light source
	m_pShaderManager->setVec3Value("lightSources[0].position", 0.0f, 20.0f, 0.0f);
	m_pShaderManager->setVec3Value("lightSources[0].ambientColor", 0.15f, 0.15f, 0.15f);
	m_pShaderManager->setVec3Value("lightSources[0].diffuseColor", 0.5f, 0.48f, 0.45f);
	m_pShaderManager->setVec3Value("lightSources[0].specularColor", 0.2f, 0.2f, 0.2f);
	m_pShaderManager->setFloatValue("lightSources[0].focalStrength", 128.0f);
	m_pShaderManager->setFloatValue("lightSources[0].specularIntensity", 0.2f);

	// Secondary fill light - provides balanced illumination
	m_pShaderManager->setVec3Value("lightSources[1].position", -8.0f, 15.0f, 8.0f);
	m_pShaderManager->setVec3Value("lightSources[1].ambientColor", 0.05f, 0.05f, 0.06f);
	m_pShaderManager->setVec3Value("lightSources[1].diffuseColor", 0.2f, 0.2f, 0.25f);
	m_pShaderManager->setVec3Value("lightSources[1].specularColor", 0.1f, 0.1f, 0.12f);
	m_pShaderManager->setFloatValue("lightSources[1].focalStrength", 96.0f);
	m_pShaderManager->setFloatValue("lightSources[1].specularIntensity", 0.1f);

	// Corner lights - extremely dim ambient fill
	float cornerAmbient = 0.0005f;    // Drastically reduced from 0.02f to 0.0005f
	float cornerDiffuse = 0.001f;     // Drastically reduced from 0.05f to 0.001f
	float cornerSpecular = 0.0f;      // No specular reflection
	float cornerStrength = 32.0f;     // Reduced spread

	// Front-left corner light
	m_pShaderManager->setVec3Value("lightSources[2].position", -12.0f, 8.0f, -12.0f);
	m_pShaderManager->setVec3Value("lightSources[2].ambientColor", cornerAmbient, cornerAmbient, cornerAmbient);
	m_pShaderManager->setVec3Value("lightSources[2].diffuseColor", cornerDiffuse, cornerDiffuse, cornerDiffuse);
	m_pShaderManager->setVec3Value("lightSources[2].specularColor", cornerSpecular, cornerSpecular, cornerSpecular);
	m_pShaderManager->setFloatValue("lightSources[2].focalStrength", cornerStrength);
	m_pShaderManager->setFloatValue("lightSources[2].specularIntensity", 0.0f);

	// Back-right corner light
	m_pShaderManager->setVec3Value("lightSources[3].position", 12.0f, 8.0f, 12.0f);
	m_pShaderManager->setVec3Value("lightSources[3].ambientColor", cornerAmbient, cornerAmbient, cornerAmbient);
	m_pShaderManager->setVec3Value("lightSources[3].diffuseColor", cornerDiffuse, cornerDiffuse, cornerDiffuse);
	m_pShaderManager->setVec3Value("lightSources[3].specularColor", cornerSpecular, cornerSpecular, cornerSpecular);
	m_pShaderManager->setFloatValue("lightSources[3].focalStrength", cornerStrength);
	m_pShaderManager->setFloatValue("lightSources[3].specularIntensity", 0.0f);
}


/***********************************************************
 *  PrepareScene()
 *
 *  This method is used for preparing the 3D scene by loading
 *  the shapes, textures in memory to support the 3D scene
 *  rendering
 ***********************************************************/


void SceneManager::PrepareScene()
{
	// Load textures first
	LoadSceneTextures();

	// Define materials for objects
	DefineObjectMaterials();

	// Setup lighting
	SetupSceneLights();

	// Load meshes
	m_basicMeshes->LoadBoxMesh();
	m_basicMeshes->LoadPlaneMesh();
	m_basicMeshes->LoadCylinderMesh();
	m_basicMeshes->LoadConeMesh();
	m_basicMeshes->LoadPrismMesh();
	m_basicMeshes->LoadPyramid4Mesh();
	m_basicMeshes->LoadSphereMesh();
	m_basicMeshes->LoadTaperedCylinderMesh();
	m_basicMeshes->LoadTorusMesh();

}


/***********************************************************
 *  RenderScene()
 *
 *  This method is used for rendering the 3D scene by
 *  transforming and drawing the basic 3D shapes
 ***********************************************************/


 /********* BACKGROUND ****************************************************************************/


void SceneManager::RenderScene()
{
	// Set a darker brown background color
	glClearColor(0.2f, 0.15f, 0.1f, 1.0f);  // Brown background
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Add basic lighting
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	// Set up light position and properties
	GLfloat lightPos[] = { 0.0f, 10.0f, 0.0f, 1.0f };  // Light from above
	GLfloat lightAmb[] = { 0.2f, 0.2f, 0.2f, 1.0f };   // Ambient light
	GLfloat lightDif[] = { 1.0f, 1.0f, 1.0f, 1.0f };   // Diffuse light

	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
	glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmb);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDif);


	// declare the variables for the transformations
	glm::vec3 scaleXYZ;
	float XrotationDegrees = 0.0f;
	float YrotationDegrees = 0.0f;
	float ZrotationDegrees = 0.0f;
	glm::vec3 positionXYZ;

	/********* WALL(s) ****************************************************************************/

	// Back wall - pushed further back
	scaleXYZ = glm::vec3(16.0f, 1.0f, 16.0f);
	XrotationDegrees = -90.0f;  // Rotate to make vertical
	YrotationDegrees = 0.0f;
	ZrotationDegrees = 0.0f;
	positionXYZ = glm::vec3(0.0f, 7.5f, -10.0f);  // Moved further back from -7.5f to -10.0f
	SetTransformations(scaleXYZ, XrotationDegrees, YrotationDegrees, ZrotationDegrees, positionXYZ);
	SetShaderTexture("drywall2");
	SetTextureUVScale(1.0f, 1.0f);
	m_basicMeshes->DrawPlaneMesh();

	/********* FLOOR/CARPET(s) ****************************************************************************/

	// Draw the floor plane
	scaleXYZ = glm::vec3(15.0f, 1.0f, 15.0f);  // Made wider and deeper
	XrotationDegrees = 0.0f;
	YrotationDegrees = 0.0f;
	ZrotationDegrees = 0.0f;
	positionXYZ = glm::vec3(0.0f, 0.0f, 0.0f);  // Moved slightly down

	SetTransformations(
		scaleXYZ,
		XrotationDegrees,
		YrotationDegrees,
		ZrotationDegrees,
		positionXYZ);

	// Floor with texture
	SetShaderMaterial("carpet");
	SetShaderTexture("floor");
	SetTextureUVScale(1.5f, 1.5f);
	m_basicMeshes->DrawPlaneMesh();

	// Draw circular carpets - smaller and positioned to touch at corners
	// Light blue carpet in back-right
	scaleXYZ = glm::vec3(3.0f, 0.01f, 3.0f);  // Reduced from 4.0f to 3.0f
	positionXYZ = glm::vec3(2.0f, 0.05f, -2.0f);  // Moved to back-right
	SetTransformations(scaleXYZ, XrotationDegrees, YrotationDegrees, ZrotationDegrees, positionXYZ);
	SetShaderMaterial("carpet");
	SetShaderTexture("carpetblue");
	m_basicMeshes->DrawCylinderMesh();

	// Beige carpet in front-left
	positionXYZ = glm::vec3(-2.0f, 0.05f, 2.0f);  // Moved to front-left
	SetTransformations(scaleXYZ, XrotationDegrees, YrotationDegrees, ZrotationDegrees, positionXYZ);
	SetShaderMaterial("carpet");
	SetShaderTexture("carpetbeige");
	m_basicMeshes->DrawCylinderMesh();


	/********* PARTY HAT **********************************************************************************/

	// Draw the cone (main hat body)
	// Party hat with pom-poms at base
	scaleXYZ = glm::vec3(1.0f, 2.0f, 1.0f);
	positionXYZ = glm::vec3(3.5f, 0.0f, -2.0f);  // Right of blocks
	SetTransformations(scaleXYZ, XrotationDegrees, YrotationDegrees, ZrotationDegrees, positionXYZ);
	SetShaderTexture("polkadots");
	SetShaderMaterial("hat");
	m_basicMeshes->DrawConeMesh();;

	// Top pom-pom
	scaleXYZ = glm::vec3(0.3f);
	positionXYZ = glm::vec3(3.5f, 2.0f, -2.0f);
	SetTransformations(scaleXYZ, XrotationDegrees, YrotationDegrees, ZrotationDegrees, positionXYZ);
	SetShaderTexture("pompom");
	SetShaderMaterial("hat");
	m_basicMeshes->DrawSphereMesh();


	// Draw a slightly smaller cylinder for the rounded edge
	// Draw the fluffy brim using multiple small spheres
	float radius = 1.0f;  // Radius of the circle where spheres will be placed
	int numSpheres = 16;  // Number of spheres to create the brim
	float sphereSize = 0.2f;  // Size of each sphere in the brim

	for (int i = 0; i < numSpheres; i++) {
		float angle = (float)i * (2.0f * 3.14159f / numSpheres);
		float xPos = 3.5f + (radius * cos(angle));
		float zPos = -2.0f + (radius * sin(angle));

		scaleXYZ = glm::vec3(sphereSize, sphereSize, sphereSize);
		XrotationDegrees = 0.0f;
		YrotationDegrees = 0.0f;
		ZrotationDegrees = 0.0f;
		positionXYZ = glm::vec3(xPos, 0.0f, zPos);  // At the base of the cone

		SetTransformations(
			scaleXYZ,
			XrotationDegrees,
			YrotationDegrees,
			ZrotationDegrees,
			positionXYZ);

		// Use the pompom texture for the brim spheres
		SetShaderTexture("pompom");
		SetTextureUVScale(1.0f, 1.0f);
		SetShaderMaterial("hat");
		m_basicMeshes->DrawSphereMesh();
	}


	/********** BLOCKS **********************************************************************/

	// Blocks stacked on blue carpet
	scaleXYZ = glm::vec3(1.0f, 1.0f, 1.0f);  // Standard size for all blocks

	// Blocks configuration on blue carpet (back-right)
	// Yellow block (bottom)
	scaleXYZ = glm::vec3(1.0f);
	positionXYZ = glm::vec3(2.0f, 0.5f, -2.0f);  // Moved to blue carpet
	SetTransformations(scaleXYZ, XrotationDegrees, YrotationDegrees, ZrotationDegrees, positionXYZ);
	SetShaderTexture("yellow");
	SetShaderMaterial("block");
	m_basicMeshes->DrawBoxMesh();

	// Red block (middle-left on yellow)
	positionXYZ = glm::vec3(1.7f, 1.5f, -2.0f);  // Slightly left of center
	YrotationDegrees = -15.0f;
	SetTransformations(scaleXYZ, XrotationDegrees, YrotationDegrees, ZrotationDegrees, positionXYZ);
	SetShaderTexture("red");
	SetShaderMaterial("block");
	m_basicMeshes->DrawBoxMesh();

	// Green block (beside stack)
	YrotationDegrees = 0.0f;
	positionXYZ = glm::vec3(0.5f, 0.5f, -2.0f);  // Left of yellow block
	SetTransformations(scaleXYZ, XrotationDegrees, YrotationDegrees, ZrotationDegrees, positionXYZ);
	SetShaderTexture("green");
	SetShaderMaterial("block");
	m_basicMeshes->DrawBoxMesh();


	/*********** TOY CAR ********************************************************************/
	// Wooden cars
	// First car (next to green block)
	// Car body
	scaleXYZ = glm::vec3(0.6f, 0.2f, 0.3f);
	positionXYZ = glm::vec3(-0.5f, 0.25f, -2.0f);  // Moved further left from green block
	YrotationDegrees = 0.0f;
	SetTransformations(scaleXYZ, XrotationDegrees, YrotationDegrees, ZrotationDegrees, positionXYZ);
	SetShaderTexture("woodcar");
	SetShaderMaterial("wood");
	m_basicMeshes->DrawBoxMesh();

	// First car roof
	scaleXYZ = glm::vec3(0.3f, 0.2f, 0.3f);  // Half width of body
	positionXYZ = glm::vec3(-0.65f, 0.45f, -2.0f);  // Left side of car body
	SetTransformations(scaleXYZ, XrotationDegrees, YrotationDegrees, ZrotationDegrees, positionXYZ);
	SetShaderTexture("woodcar");
	SetShaderMaterial("wood");
	m_basicMeshes->DrawBoxMesh();

	// First car wheels (4 wheels)
	scaleXYZ = glm::vec3(0.15f, 0.05f, 0.15f);
	for (int i = 0; i < 4; i++) {
		float xOffset = (i % 2 * 0.4f) - 0.2f;  // Spread wheels wider
		float zOffset = (i < 2 ? 0.12f : -0.12f);  // Front/back wheel spacing
		positionXYZ = glm::vec3(-0.5f + xOffset, 0.175f, -2.0f + zOffset);
		SetTransformations(scaleXYZ, 90.0f, 0.0f, 0.0f, positionXYZ);
		SetShaderColor(0.2f, 0.2f, 0.2f, 1.0f);
		m_basicMeshes->DrawCylinderMesh();
	}

	// Second car (on beige carpet)
	// Car body
	scaleXYZ = glm::vec3(0.6f, 0.2f, 0.3f);
	positionXYZ = glm::vec3(-1.5f, 0.25f, 1.5f);  // Moved closer to carpet intersection
	YrotationDegrees = 0.0f;
	SetTransformations(scaleXYZ, XrotationDegrees, YrotationDegrees, ZrotationDegrees, positionXYZ);
	SetShaderTexture("woodcar");
	SetShaderMaterial("wood");
	m_basicMeshes->DrawBoxMesh();

	// Second car roof
	scaleXYZ = glm::vec3(0.3f, 0.2f, 0.3f);  // Half size of body
	positionXYZ = glm::vec3(-1.65f, 0.45f, 1.5f);  // Left side of car body
	SetTransformations(scaleXYZ, XrotationDegrees, YrotationDegrees, ZrotationDegrees, positionXYZ);
	SetShaderTexture("woodcar");
	SetShaderMaterial("wood");
	m_basicMeshes->DrawBoxMesh();

	// Second car wheels (4 wheels)
	scaleXYZ = glm::vec3(0.15f, 0.05f, 0.15f);
	for (int i = 0; i < 4; i++) {
		float xOffset = (i % 2 * 0.45f) - 0.225f;  // Spread wheels to edges
		float zOffset = (i < 2 ? 0.12f : -0.12f);  // Front/back wheel spacing
		positionXYZ = glm::vec3(-1.5f + xOffset, 0.175f, 1.5f + zOffset);
		SetTransformations(scaleXYZ, 90.0f, 0.0f, 0.0f, positionXYZ);
		SetShaderColor(0.2f, 0.2f, 0.2f, 1.0f);
		m_basicMeshes->DrawCylinderMesh();
	}

	//*********** KICKBALL **************************************************************************/

	// Draw the red ball
	scaleXYZ = glm::vec3(0.35f, 0.35f, 0.35f);
	XrotationDegrees = 0.0f;
	YrotationDegrees = 45.0f;
	ZrotationDegrees = 0.0f;
	positionXYZ = glm::vec3(1.5f, 0.2f, 2.0f);

	SetTransformations(scaleXYZ, XrotationDegrees, YrotationDegrees, ZrotationDegrees, positionXYZ);
	SetShaderMaterial("toy");
	SetShaderTexture("purple");
	SetTextureUVScale(1.0f, 1.0f);
	m_basicMeshes->DrawSphereMesh();

}
