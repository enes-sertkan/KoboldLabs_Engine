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
#include "sSTData.h"
#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


struct Particle {
	glm::vec3 position;
	glm::vec3 velocity;
	glm::vec4 color;
	float size;
	float lifetime;
	float lifeRemaining;

	// New: 3D rotation represented as Euler angles (in degrees)
	glm::vec3 rotation = glm::vec3(0.0f);

	// New: Rotation speed as Euler angle increments (degrees per second)
	glm::vec3 rotationSpeed = glm::vec3(0.0f);
	glm::vec3 targetPos = glm::vec3(0.0f);

	bool active = false;
	bool returning = false;
};

struct GPUParticle{
	GPUParticle(Particle cpuParticle)
	{
		position = cpuParticle.position;
		velocity = cpuParticle.velocity;
		color = cpuParticle.color;
		size = cpuParticle.size;
		lifetime = cpuParticle.lifeRemaining;
		rotation = cpuParticle.rotation;  // Copy over current rotation
	}
	glm::vec3 position;   // 12 bytes
	float size;           // 4 bytes (offset 12)

	glm::vec3 velocity;   // 12 bytes (offset 16)
	float lifetime;       // 4 bytes (offset 28)

	glm::vec4 color;      // 16 bytes (offset 32)

	glm::vec3 rotation;   // 12 bytes (offset 48)
	float padding;        // 4 bytes (offset 60)

	// Explicit padding to match std140 requirements
	//float _pad0[3];       // 12 bytes (offset 64)
};

struct sMesh
{
	sMesh() { // Constructor
		positionXYZ = glm::vec3(0.0f);
		rotationEulerXYZ = glm::vec3(0.0f);
		uniformScale = 1.0f;
		objectColourRGBA = glm::vec4(1.0f);
		bOverrideObjectColour = false;
		bDoNotLight = false;
		// Initialize ALL members here
		transperency = 1.0f;
		metal = 0.0f;
		smoothness = 0.0f;
		// ... other members
	}

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
	float shakePower = 0;
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
	bool useMetalTexture = true;
	bool useSmoothTexture = true;

	bool shellTexturing = false;
	sSTData stData;
	sSTCollider stColliders[20];


	unsigned int uniqueID = 0;

	float zoomPower = 0;
	float chromaticPower = 0;

	bool drawBothFaces = false;
	std::string getState(void);
	bool loadState(std::string newState);

	bool isParticleEmitter = false;
	std::vector<Particle>* pParticles =  new   std::vector<Particle>();;
	GLuint particleSSBO;
	GLuint particleUBO;

	UVWave waves[10];
	void spawnWave(glm::vec2 uvPos) {
		// Loop through the wave array.
		for (unsigned int i = 0; i < 10; ++i) {
			// Only replace an inactive wave.
			if (!waves[i].active) {
			    waves[i].uv = uvPos;    // Set the new UV position
			    waves[i].time = 0.f;     // Reset the time value
				waves[i].active = true;   // Mark the wave as active
				return;  // Only spawn one wave, then exit.
			}
		}
	}

	int CreateCollider( glm::vec3 pos, float radius = 0.5f, float blendingRadius = 0.5f)
	{
		for (int i = 0; i < 20; ++i)
		{
			if (!stColliders[i].isOn)
			{
				stColliders[i].isOn = true;
				stColliders[i].position = pos;
				stColliders[i].radius = radius;
				stColliders[i].blendingRadius = blendingRadius;
				return i;
			}
		}
		return -1; // No available collider slot
	}


	bool RemoveCollider(unsigned int index)
	{
		if (index < 20 && stColliders[index].isOn)
		{
			stColliders[index].isOn = false;
			return true;
		}
		return false;
	}

	bool DestroyCollider(unsigned int index)
	{
		if (index < 20)
		{
			stColliders[index].isOn = false;
			return true;
		}
		return false;
	}

	bool UpdateColliderPosition(unsigned int index, glm::vec3 pos)
	{
		if (index < 20 && stColliders[index].isOn)
		{
			stColliders[index].position= pos;
			return true;
		}
		return false;
	}




//	sMesh* pChildMeshes[100];
};