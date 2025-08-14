/***********************************************************
 * 
 *	Southern New Hampshire University
 *  CS-499: CS Capstone Enhancement Project
 *  Professor: Ramsey Kraya
 *  Name: Hector Maldonado 
 *
 ***********************************************************/

 /***********************************************************
  *						*** ENHANCEMENT ***
  *
  * 
  *
  ***********************************************************/

#include "SceneManager.h"
#include <glm/gtx/transform.hpp>
#include "camera.h"
#include "../Octree.h" 

// Enhancement Milestone 4: ViewManager is now included so we can access the global camera pointer 
// (g_pCamera) for camera save/load functionality.
#include "ViewManager.h" 

// Enhancement Milestone 4: JsonDatabase is included to enable saving/loading scene objects and camera 
// state to/from JSON files.
#include "../JsonDatabase.h" 


#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#endif



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
 *						*** ENHANCEMENT ***
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
 *						*** ENHANCEMENT ***
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
 *						*** ENHANCEMENT ***
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
 *						*** ENHANCEMENT ***
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
 *						*** ENHANCEMENT ***
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
 *						*** ENHANCEMENT ***
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

	// --- OCTREE INTEGRATION START ---

	AABB sceneBounds;
	sceneBounds.min = glm::vec3(-20.0f, -1.0f, -20.0f);
	sceneBounds.max = glm::vec3(20.0f, 20.0f, 20.0f);

	m_sceneObjects.clear();

	// ----------------  Backwall --------------------
	m_sceneObjects.push_back({ glm::vec3(0.0f, 7.5f, -10.0f), 8.0f, "backwall" });
	
	// ----------------  Floor --------------------
	m_sceneObjects.push_back({ glm::vec3(0.0f, 0.0f, 0.0f), 8.0f, "floor" });
	

	// ----------------  Carpets  --------------------=
	// Blue carpet (cylinder)
	m_sceneObjects.push_back({ glm::vec3(2.0f, 0.05f, -2.0f), 3.0f, "carpetblue" });
	
	// Beige carpet (cylinder)
	m_sceneObjects.push_back({ glm::vec3(-2.0f, 0.05f, 2.0f), 3.0f, "carpetbeige" });
	

	// ----------------  Party Hat  --------------------
	// Party hat (cone)
	m_sceneObjects.push_back({ glm::vec3(3.5f, 0.0f, -2.0f), 2.0f, "partyhat" });
	// Party hat pom-pom (sphere)
	m_sceneObjects.push_back({ glm::vec3(3.5f, 2.0f, -2.0f), 0.3f, "hatpompom" });
	// Brim spheres (16 around base)
	for (int i = 0; i < 16; i++) {
		float angle = (float)i * (2.0f * 3.14159f / 16.0f);
		float xPos = 3.5f + (1.0f * cos(angle));
		float zPos = -2.0f + (1.0f * sin(angle));
		m_sceneObjects.push_back({ glm::vec3(xPos, 0.0f, zPos), 0.2f, "hatbrimsphere" });
	}
	
	// ----------------  Toy blocks (cubes)  --------------------
	// Yellow block
	m_sceneObjects.push_back({ glm::vec3(2.0f, 0.5f, -2.0f), 0.7f, "yellowblock" });
	
	// Red block
	m_sceneObjects.push_back({ glm::vec3(1.7f, 1.5f, -2.0f), 0.7f, "redblock" });
	
	// Green block
	m_sceneObjects.push_back({ glm::vec3(0.5f, 0.5f, -2.0f), 0.7f, "greenblock" });
	

	// ----------------  Toy Car 1  --------------------
	// First car body
	m_sceneObjects.push_back({ glm::vec3(-0.5f, 0.25f, -2.0f), 0.6f, "car1body" });
	
	// First car roof
	m_sceneObjects.push_back({ glm::vec3(-0.65f, 0.45f, -2.0f), 0.3f, "car1roof" });
	
	// First car wheels (4)
	for (int i = 0; i < 4; i++) {
		float xOffset = (i % 2 * 0.4f) - 0.2f;
		float zOffset = (i < 2 ? 0.12f : -0.12f);
		m_sceneObjects.push_back({ glm::vec3(-0.5f + xOffset, 0.175f, -2.0f + zOffset), 0.15f, "car1wheel" });
	}
	

	// ----------------  Toy Car 2 --------------------
	// Second car body
	m_sceneObjects.push_back({ glm::vec3(-1.5f, 0.25f, 1.5f), 0.6f, "car2body" });
	
	// Second car roof
	m_sceneObjects.push_back({ glm::vec3(-1.65f, 0.45f, 1.5f), 0.3f, "car2roof" });
	
	// Second car wheels (4)
	for (int i = 0; i < 4; i++) {
		float xOffset = (i % 2 * 0.45f) - 0.225f;
		float zOffset = (i < 2 ? 0.12f : -0.12f);
		m_sceneObjects.push_back({ glm::vec3(-1.5f + xOffset, 0.175f, 1.5f + zOffset), 0.15f, "car2wheel" });
	}


	// ----------------  Kickball  --------------------
	m_sceneObjects.push_back({ glm::vec3(1.5f, 0.2f, 2.0f), 0.35f, "kickball" });


	if (m_octreeRoot) delete m_octreeRoot;
	m_octreeRoot = new OctreeNode(sceneBounds);
	for (auto& obj : m_sceneObjects)
		m_octreeRoot->insert(&obj);

	// --- OCTREE INTEGRATION END ---


}


/***********************************************************
 *  RenderScene()
 *
 *  This method is used for rendering the 3D scene by
 *  transforming and drawing the basic 3D shapes
 ***********************************************************/


void SceneManager::RenderScene()
{
	// --- OCTREE INTEGRATION START ---

	extern Camera* g_pCamera; // from ViewManager.cpp
	glm::vec3 camPos = g_pCamera ? g_pCamera->Position : glm::vec3(0.0f);
	float viewRange = 15.0f;
	AABB cameraAABB;
	cameraAABB.min = camPos - glm::vec3(viewRange);
	cameraAABB.max = camPos + glm::vec3(viewRange);

	std::vector<SceneObject*> visibleObjects;
	if (m_octreeRoot)
		m_octreeRoot->query(cameraAABB, visibleObjects);

	for (SceneObject* obj : visibleObjects) {
		if (obj->tag == "backwall") {
			SetTransformations(glm::vec3(16.0f, 1.0f, 16.0f), -90.0f, 0.0f, 0.0f, obj->position);
			SetShaderTexture("drywall2");
			SetTextureUVScale(1.0f, 1.0f);
			m_basicMeshes->DrawPlaneMesh();
		}

		else if (obj->tag == "floor") {
			SetTransformations(glm::vec3(15.0f, 1.0f, 15.0f), 0.0f, 0.0f, 0.0f, obj->position);
			SetShaderMaterial("carpet");
			SetShaderTexture("floor");
			SetTextureUVScale(1.5f, 1.5f);
			m_basicMeshes->DrawPlaneMesh();
		}

		else if (obj->tag == "carpetblue") {
			SetTransformations(glm::vec3(3.0f, 0.01f, 3.0f), 0.0f, 0.0f, 0.0f, obj->position);
			SetShaderMaterial("carpet");
			SetShaderTexture("carpetblue");
			m_basicMeshes->DrawCylinderMesh();
		}

		else if (obj->tag == "carpetbeige") {
			SetTransformations(glm::vec3(3.0f, 0.01f, 3.0f), 0.0f, 0.0f, 0.0f, obj->position);
			SetShaderMaterial("carpet");
			SetShaderTexture("carpetbeige");
			m_basicMeshes->DrawCylinderMesh();
		}

		else if (obj->tag == "partyhat") {
			SetTransformations(glm::vec3(1.0f, 2.0f, 1.0f), 0.0f, 0.0f, 0.0f, obj->position);
			SetShaderTexture("polkadots");
			SetShaderMaterial("hat");
			m_basicMeshes->DrawConeMesh();
		}

		else if (obj->tag == "hatpompom") {
			SetTransformations(glm::vec3(0.3f), 0.0f, 0.0f, 0.0f, obj->position);
			SetShaderTexture("pompom");
			SetShaderMaterial("hat");
			m_basicMeshes->DrawSphereMesh();
		}

		else if (obj->tag == "hatbrimsphere") {
			SetTransformations(glm::vec3(0.2f, 0.2f, 0.2f), 0.0f, 0.0f, 0.0f, obj->position);
			SetShaderTexture("pompom");
			SetTextureUVScale(1.0f, 1.0f);
			SetShaderMaterial("hat");
			m_basicMeshes->DrawSphereMesh();
		}

		else if (obj->tag == "yellowblock") {
			SetTransformations(glm::vec3(1.0f), 0.0f, 0.0f, 0.0f, obj->position);
			SetShaderTexture("yellow");
			SetShaderMaterial("block");
			m_basicMeshes->DrawBoxMesh();
		}

		else if (obj->tag == "redblock") {
			SetTransformations(glm::vec3(1.0f), 0.0f, -15.0f, 0.0f, obj->position);
			SetShaderTexture("red");
			SetShaderMaterial("block");
			m_basicMeshes->DrawBoxMesh();
		}

		else if (obj->tag == "greenblock") {
			SetTransformations(glm::vec3(1.0f), 0.0f, 0.0f, 0.0f, obj->position);
			SetShaderTexture("green");
			SetShaderMaterial("block");
			m_basicMeshes->DrawBoxMesh();
		}

		else if (obj->tag == "car1body") {
			SetTransformations(glm::vec3(0.6f, 0.2f, 0.3f), 0.0f, 0.0f, 0.0f, obj->position);
			SetShaderTexture("woodcar");
			SetShaderMaterial("wood");
			m_basicMeshes->DrawBoxMesh();
		}

		else if (obj->tag == "car1roof") {
			SetTransformations(glm::vec3(0.3f, 0.2f, 0.3f), 0.0f, 0.0f, 0.0f, obj->position);
			SetShaderTexture("woodcar");
			SetShaderMaterial("wood");
			m_basicMeshes->DrawBoxMesh();
		}

		else if (obj->tag == "car1wheel") {
			SetTransformations(glm::vec3(0.15f, 0.05f, 0.15f), 90.0f, 0.0f, 0.0f, obj->position);
			SetShaderColor(0.2f, 0.2f, 0.2f, 1.0f);
			m_basicMeshes->DrawCylinderMesh();
		}

		else if (obj->tag == "car2body") {
			SetTransformations(glm::vec3(0.6f, 0.2f, 0.3f), 0.0f, 0.0f, 0.0f, obj->position);
			SetShaderTexture("woodcar");
			SetShaderMaterial("wood");
			m_basicMeshes->DrawBoxMesh();
		}

		else if (obj->tag == "car2roof") {
			SetTransformations(glm::vec3(0.3f, 0.2f, 0.3f), 0.0f, 0.0f, 0.0f, obj->position);
			SetShaderTexture("woodcar");
			SetShaderMaterial("wood");
			m_basicMeshes->DrawBoxMesh();
		}

		else if (obj->tag == "car2wheel") {
			SetTransformations(glm::vec3(0.15f, 0.05f, 0.15f), 90.0f, 0.0f, 0.0f, obj->position);
			SetShaderColor(0.2f, 0.2f, 0.2f, 1.0f);
			m_basicMeshes->DrawCylinderMesh();
		}

		else if (obj->tag == "kickball") {
			SetTransformations(glm::vec3(0.35f, 0.35f, 0.35f), 0.0f, 45.0f, 0.0f, obj->position);
			SetShaderMaterial("toy");
			SetShaderTexture("purple");
			SetTextureUVScale(1.0f, 1.0f);
			m_basicMeshes->DrawSphereMesh();
		}
	}

	// --- OCTREE INTEGRATION END ---

}

// --- JSON Scene Save/Load Enhancement ---

void SceneManager::SaveSceneToJson(const std::string& filename) {
	JsonDatabase::SaveSceneObjects(m_sceneObjects, filename);
}

void SceneManager::LoadSceneFromJson(const std::string& filename) {
	std::vector<SceneObject> loadedObjects;
	if (JsonDatabase::LoadSceneObjects(loadedObjects, filename)) {
		m_sceneObjects = loadedObjects;
		// Rebuild octree
		if (m_octreeRoot) delete m_octreeRoot;
		AABB sceneBounds;
		sceneBounds.min = glm::vec3(-20.0f, -1.0f, -20.0f);
		sceneBounds.max = glm::vec3(20.0f, 20.0f, 20.0f);
		m_octreeRoot = new OctreeNode(sceneBounds);
		for (auto& obj : m_sceneObjects)
			m_octreeRoot->insert(&obj);
	}
}

void SceneManager::SaveSceneAndCameraToJson(const std::string& filename) {
	CameraState cam;
	extern Camera* g_pCamera;
	cam.position = g_pCamera->Position;
	cam.front = g_pCamera->Front;
	cam.up = g_pCamera->Up;
	cam.zoom = g_pCamera->Zoom;
	JsonDatabase::SaveSceneAndCamera(m_sceneObjects, cam, filename);
}

void SceneManager::LoadSceneAndCameraFromJson(const std::string& filename) {
	CameraState cam;
	std::vector<SceneObject> loadedObjects;
	if (JsonDatabase::LoadSceneAndCamera(loadedObjects, cam, filename)) {
		m_sceneObjects = loadedObjects;

		// Rebuild octree
		if (m_octreeRoot) delete m_octreeRoot;
		AABB sceneBounds;
		sceneBounds.min = glm::vec3(-20.0f, -1.0f, -20.0f);
		sceneBounds.max = glm::vec3(20.0f, 20.0f, 20.0f);
		m_octreeRoot = new OctreeNode(sceneBounds);
		for (auto& obj : m_sceneObjects)
			m_octreeRoot->insert(&obj);

		// Restore camera
		extern Camera* g_pCamera;
		g_pCamera->Position = cam.position;
		g_pCamera->Front = cam.front;
		g_pCamera->Up = cam.up;
		g_pCamera->Zoom = cam.zoom;
	}
}
// --- End of JSON Scene Save/Load Enhancement ---
