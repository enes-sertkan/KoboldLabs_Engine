#pragma once

// This is the 3D drawing information
//	to draw a single mesh (single PLY file)
#define GLM_ENABLE_EXPERIMENTAL 
#include <string>
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include "UVWave.h"

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
	std::string AOtexture;
	std::string STTexture;
	std::string NMTexture;
	float metal=0;
	float smoothness=0;
	bool useMetalTexture = false;
	bool useSmoothTexture = false;


	unsigned int uniqueID = 0;

	float zoomPower = 0;
	float chromaticPower = 0;

	bool drawBothFaces = false;
	std::string getState(void);
	bool loadState(std::string newState);


	UVWave waves[10];
	void spawnWave(glm::vec2 uvPos) {
		// Loop through the wave array.
		for (unsigned int i = 0; i < 10; ++i) {
			// Only replace an inactive wave.
			if (!waves[i].active) {
			    waves[i].uv = uvPos;    // Set the new UV position
			    waves[i].time = 0.5f;     // Reset the time value
				waves[i].active = true;   // Mark the wave as active
				return;  // Only spawn one wave, then exit.
			}
		}
	}



//	sMesh* pChildMeshes[100];
};