///////////////////////////////////////////////////////////////////////////////
// shadermanager.h
// ============
// manage the loading and rendering of 3D scenes
//
//  AUTHOR: Brian Battersby - SNHU Instructor / Computer Science
//	Created for CS-330-Computational Graphics and Visualization, Nov. 1st, 2023
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include "ShaderManager.h"
#include "ShapeMeshes.h"

#include <string>
#include <vector>
#include "../TextureManager.h"
#include "../MaterialManager.h"

/***********************************************************
 *  SceneManager
 *
 *  This class contains the code for preparing and rendering
 *  3D scenes, including the shader settings.
 ***********************************************************/


class SceneManager
{
public:
	// NEW constructor, changed to recognize the new managers
	SceneManager(ShaderManager* pShaderManager, TextureManager* pTextureManager,
		MaterialManager* pMaterialManager);
	// destructor
	~SceneManager();

	struct TEXTURE_INFO
	{
		std::string tag;
		uint32_t ID;
	};

private:


	// pointer to shader manager object
	ShaderManager* m_pShaderManager;
	// pointer to basic shapes object
	ShapeMeshes* m_basicMeshes;
	TextureManager* m_textureManager;
	MaterialManager* m_materialManager;


	// REMOVED TEXTURE_INFO & m_textureIDs to use TextureManager and MaterialManager

	// set the transformation values 
	// into the transform buffer
	void SetTransformations(
		glm::vec3 scaleXYZ,
		float XrotationDegrees,
		float YrotationDegrees,
		float ZrotationDegrees,
		glm::vec3 positionXYZ);

	// set the color values into the shader
	void SetShaderColor(
		float redColorValue,
		float greenColorValue,
		float blueColorValue,
		float alphaValue);

	// set the texture data into the shader
	void SetShaderTexture(
		std::string textureTag);

	// set the UV scale for the texture mapping
	void SetTextureUVScale(
		float u, float v);

	// set the object material into the shader
	void SetShaderMaterial(
		std::string materialTag);

public:

	// prepare the 3D scene for rendering
	void PrepareScene();
	// render the objects in the 3D scene
	void RenderScene();

	// load all of the needed textures before rendering
	void LoadSceneTextures();
	// define all the object materials before rendering
	void DefineObjectMaterials();
	// add and define the light sources before rendering
	void SetupSceneLights();

	// methods for rendering the various objects in the 3D scene

};
