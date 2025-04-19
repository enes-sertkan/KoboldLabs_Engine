#pragma once

#include <glm/glm.hpp>
#include <glm/vec3.hpp> // glm::vec3
#include <vector>
#include "cSoftBodyCollisions.h"

//#include "cVAOManager/sModelDrawInfo.h"
#include "cVAOManager/cVAOManager.h"	// for: sModelDrawInfo	


class SoftBodyCollision;
class cSoftBodyVerlet
{
public:

	std::vector<glm::vec3> m_InitialPositions;  // Add this line
	std::vector<glm::vec3> m_InitialOldPositions;  // Add this line


	void ResetToInitialState();
	void MoveAllParticles(const glm::vec3& delta);
	void CreateBendingConstraints()
	{
		// Create constraints between adjacent faces
		for (unsigned int i = 0; i < m_ModelVertexInfo.numberOfIndices; i += 3)
		{
			unsigned int a = m_ModelVertexInfo.pIndices[i];
			unsigned int b = m_ModelVertexInfo.pIndices[i + 1];
			unsigned int c = m_ModelVertexInfo.pIndices[i + 2];

			// Find adjacent triangles
			// (Implementation depends on your mesh structure)
			// For each adjacent triangle, create angular constraints
			sConstraint* bendConstraint = new sConstraint();
			bendConstraint->pParticleA = vec_pParticles[a];
			bendConstraint->pParticleB = vec_pParticles[c];
			bendConstraint->restLength = calcDistanceBetween(bendConstraint->pParticleA,
				bendConstraint->pParticleB)*2;
			
			vec_pConstraints.push_back(bendConstraint);
		}
	}
	void SetInitialVelocity(const glm::vec3& velocity);

	glm::vec3 m_geometricCentrePoint = glm::vec3(0);
	float yToJump = -7;
	cSoftBodyVerlet();
	~cSoftBodyVerlet();

	cSoftBodyVerlet(const cSoftBodyVerlet& other) {
		// 1. Copy basic members
		this->m_geometricCentrePoint = other.m_geometricCentrePoint;
		this->yToJump = other.yToJump;
		this->acceleration = other.acceleration;
		this->tightnessFactor = other.tightnessFactor;
		this->matching_VAO_MeshName = other.matching_VAO_MeshName;
		this->volume = other.volume;
		this->useVolume = other.useVolume;

		// 2. Deep copy particles
		for (sParticle* pOtherParticle : other.vec_pParticles) {
			sParticle* pNewParticle = new sParticle();
			*pNewParticle = *pOtherParticle;  // Copy all particle data
			this->vec_pParticles.push_back(pNewParticle);
		}

		for (size_t i = 0; i < other.m_InitialPositions.size(); ++i) {
			this->m_InitialPositions.push_back(other.m_InitialPositions[i]);
		}


		for (size_t i = 0; i < other.m_InitialOldPositions.size(); ++i) {
			this->m_InitialOldPositions.push_back(other.m_InitialOldPositions[i]);
		}

		// 3. Deep copy constraints (with remapped particle pointers)
		for (sConstraint* pOtherConstraint : other.vec_pConstraints) {
			sConstraint* pNewConstraint = new sConstraint();

			// Copy simple members
			pNewConstraint->restLength = pOtherConstraint->restLength;
			pNewConstraint->maxIterations = pOtherConstraint->maxIterations;
			pNewConstraint->bIsActive = pOtherConstraint->bIsActive;
			pNewConstraint->breakingDistance = pOtherConstraint->breakingDistance;
			pNewConstraint->bIsBreakable = pOtherConstraint->bIsBreakable;
			pNewConstraint->bIsLocked = pOtherConstraint->bIsLocked;

			// Remap particle pointers to OUR particles
			size_t indexA = std::distance(other.vec_pParticles.begin(),
				std::find(other.vec_pParticles.begin(),
					other.vec_pParticles.end(),
					pOtherConstraint->pParticleA));
			size_t indexB = std::distance(other.vec_pParticles.begin(),
				std::find(other.vec_pParticles.begin(),
					other.vec_pParticles.end(),
					pOtherConstraint->pParticleB));

			pNewConstraint->pParticleA = this->vec_pParticles[indexA];
			pNewConstraint->pParticleB = this->vec_pParticles[indexB];

			this->vec_pConstraints.push_back(pNewConstraint);
		}

		// 4. Copy model vertex info
		this->m_ModelVertexInfo = other.m_ModelVertexInfo;

		// Important: Update vertex pointers to OUR particles
		for (size_t i = 0; i < this->vec_pParticles.size(); ++i) {
			this->vec_pParticles[i]->pModelVertex = &this->m_ModelVertexInfo.pVertices[i];
		}

	
	}
	void UpdateGeometricCentrePoint(void);
	// This is for loading the original model
	// The 2nd param is the identy matrix 
	bool CreateSoftBody(sModelDrawInfo ModelDrawInfo, glm::mat4 matInitalTransform = glm::mat4(1.0f), int constIterations = 7, float restLengthMultiplier = 1.f);

	// Will update the vertex information to match the particles
	void UpdateVertexPositions(void);

	void UpdateNormals(void);

	void CreateConstraintsBetweenCloseVertices(float maxDistance);

	// Create random constraints within the object to 'brace' the shape
	// These are invisible, though]

	void RandomizeConstraintIterations();
	void CreateRandomBracing(unsigned int numberOfBraces,
		float minDistanceBetweenVertices);

	// This will update the draw info vertex information from the 
	//	soft body simulation state (at the current moment)
//	bool UpdateDrawInfo(void);
	void CalculateBaseVolume();



	float GetVolume();
	void ApplyVolumeCorrection();


	void VerletUpdate(double deltaTime);

	// This is for the ground, hitting something, etc.
	void ApplyCollision(double deltaTime, SoftBodyCollision* sbCollision, glm::vec3 worldPosition, float scale, bool cylCol);

	void SatisfyConstraints(void);

	void ApplyCylinderCollision(double deltaTime, SoftBodyCollision* sbCollision, float scale, glm::vec3 worldPosition);

	typedef sVertex_SHADER_FORMAT_xyz_rgb_N_UV sVertex;

	// This represents a vertex or point or whatever in the soft body
	struct sParticle
	{
		glm::vec3 position = glm::vec3(0.0f);
		glm::vec3 old_position = glm::vec3(0.0f);
		// Pointer back to the model vertex info
		sVertex* pModelVertex = NULL;					// See typedef above...
		//
		// If "fixed" then don't update with Verlet step
		bool bIsFixed_DontUpdate = false;
		float accelerationMultiplier = 1.f;
	};

	void cleanZeros(glm::vec3& value);

	void DebugDrawNormals(float scale = 1.0f);
	float calcDistanceBetween(sParticle* pPartA, sParticle* pPartB);

	// These are the "rods" or "springs" between vertices
	// We get these from the mesh, BUT we might need additional
	//	constraints to make sure this stays in the desired shape
	struct sConstraint
	{
		sConstraint() {}
		sParticle* pParticleA = NULL;
		sParticle* pParticleB = NULL;
		// How far away they are supposed to be 'at rest'
		float restLength = 2.0f;

		unsigned int maxIterations = 7;

		// if false, this isnt' checked
		// Like if the constraint is 'broken'
		bool bIsActive = true;

		// Maybe a maximum distance between vertices:
		// If it was stiff cloth, maybe set this to just longer than the rest length.
		// It it's really "springy" (like spandex?) then set this to a lot longer.
		float breakingDistance = FLT_MAX;
		bool bIsBreakable = false;

		// This is interesting, too
		// From: I Spent a Week Making an AI's Video Game Idea - YouTube
		// https://www.youtube.com/watch?v=PGk0rnyTa1U&ab_channel=SebastianLague
		// He's using it as to fix the end of the constraint
		bool bIsLocked = false;
	};

	// Force, like gravity or whatever
	glm::vec3 acceleration = glm::vec3(0.0f);
	float tightnessFactor = 1.f;
	// This is the "guts" of the soft body
	std::vector< sParticle* > vec_pParticles;			// All the vertices
	std::vector< sConstraint* > vec_pConstraints;		// All "springs" or whatever

	// The mesh model name that the VAO has 
	// i.e. the drawing mesh that matches this particular soft body
	std::string matching_VAO_MeshName;
	sModelDrawInfo m_ModelVertexInfo;

	// This method is public and has THE SAME critical section
	//	we have in the Update vertex and normal code
	sModelDrawInfo* pGetModelDrawInfo(void);

	// Max update time is 60Hz
	const double MAX_DELTATIME = 1.0 / 60.0;

	void LockParticlesOnX(float xPos, bool lower);
	void LockParticlesOnZ(float xPos, bool lower);
	void LockParticlesOnY(float xPos, bool lower);
	void LockParticlesOutsideRadius(float radius);

	// This just gets the average of all the points (based on location)
	glm::vec3 getGeometricCentrePoint(void);

	// This would  take into account the mass, so would "look" more accurate, maybe?
	glm::vec3 getCentreOfMass(void);

	float volume;

	bool useVolume = false;
};

