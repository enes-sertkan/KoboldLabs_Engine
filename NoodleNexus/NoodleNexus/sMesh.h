#pragma once

// This is the 3D drawing information
//	to draw a single mesh (single PLY file)

#include <string>
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/rotate_vector.hpp>

struct sMesh
{
	std::string modelFileName;			// "bunny.ply"
	std::string uniqueFriendlyName;		// "Fred", "Ali", "Bunny_007"

	glm::vec3 positionXYZ;
	glm::vec3 rotationEulerXYZ;		// 90 degrees around the x axis
	float uniformScale = 1.0f;				// Same for each axis

	glm::vec4 objectColourRGBA;
	// If true, it uses the colour above
	bool bOverrideObjectColour = true;

	bool bIsWireframe = false;
	bool bIsVisible = true;
	bool bDoNotLight = false;
	bool bIsStencilTexture = false;

	glm::vec2 textureSpeed = glm::vec2(0.0);
	float time = 0;

	// unique ID is read only
	// TODO: change so we do not use from mesh
	unsigned int getUniqueID(void);

	unsigned int m_uniqueID = 0;
	// 
	static unsigned int m_NextUniqueID;// = 100;

	int stencilTextureID = 100;//SMELLS LIKE A BAD IDEA Maybe TODO later.
								//If you use stencil texture you MUST change this.
	std::string stencilTexture; // you also must set this up.

	float transperency=1.f;
	
	static const unsigned int MAX_NUM_TEXTURES = 4;
	std::string textures[MAX_NUM_TEXTURES];
	float blendRatio[MAX_NUM_TEXTURES];
	int textureFillType[MAX_NUM_TEXTURES];

	unsigned int uniqueID = 0;

	std::string getState(void);
	bool loadState(std::string newState);

//	sMesh* pChildMeshes[100];
};