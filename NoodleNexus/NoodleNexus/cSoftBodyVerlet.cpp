﻿#pragma once
#include "cSoftBodyVerlet.hpp"


#include <iostream>


cSoftBodyVerlet::cSoftBodyVerlet()
{

}

cSoftBodyVerlet::~cSoftBodyVerlet()
{

}


void cSoftBodyVerlet::CreateConstraintsBetweenCloseVertices(float maxDistance)
{
	size_t numParticles = vec_pParticles.size();
	for (size_t i = 0; i < numParticles; ++i)
	{
		for (size_t j = i + 1; j < numParticles; ++j)
		{
			// Compute distance between the two particles
			float distance = glm::distance(vec_pParticles[i]->position, vec_pParticles[j]->position);

			// If the distance is less than the threshold, create a constraint
			if (distance < maxDistance)
			{
				sConstraint* newConstraint = new sConstraint();
				newConstraint->pParticleA = vec_pParticles[i];
				newConstraint->pParticleB = vec_pParticles[j];
				newConstraint->restLength = 0.f; // Set the rest length to the current distance
				newConstraint->bIsActive = true;  // Make sure the constraint is active
				// You might also want to set other properties (like maxIterations, bIsBreakable, etc.)

				vec_pConstraints.push_back(newConstraint);
			}
			
		}
	}
	//vec_pConstraints[vec_pConstraints.size() - 1]->bIsActive = true;
	//vec_pConstraints[vec_pConstraints.size() - 1]->pParticleA->bIsFixed_DontUpdate = true;
	//vec_pConstraints[vec_pConstraints.size() - 1]->pParticleB->bIsFixed_DontUpdate = true;
}

// This is for loading the original model
bool cSoftBodyVerlet::CreateSoftBody(sModelDrawInfo ModelInfo, glm::mat4 matInitalTransform, int constIterations, float restLengthMultiplier)
{
	// Copy the model info
	this->m_ModelVertexInfo = ModelInfo;

	this->matching_VAO_MeshName = ModelInfo.meshPath;

	// Copy the vertices and indices as well
	// The cModelDrawInfo has pointers to the originally loaded vertices.
	sVertex* pVerticesLocalCopy = new sVertex[ModelInfo.numberOfVertices];

	for (unsigned int index = 0; index != ModelInfo.numberOfVertices; index++)
	{
		// TODO: If we are taking these vertices, apply some soft boy tranform, 
		//	then updating the original VAO buffers (which we ARE going to do),
		//	then we have to be aware that this will CHANGE the original VAO.
		// In other words, if we have TWO bouncy soft bunnies, we will
		//	have to load TWO meshes into the VAO...because the soft body
		//	physics will update each one. 
		glm::vec4 theVertex = glm::vec4(ModelInfo.pVertices[index].x,
			ModelInfo.pVertices[index].y,
			ModelInfo.pVertices[index].z, 1.0f);
		// Apply transformation
		theVertex = matInitalTransform * theVertex;

		pVerticesLocalCopy[index].x = theVertex.x;
		pVerticesLocalCopy[index].y = theVertex.y;
		pVerticesLocalCopy[index].z = theVertex.z;


		pVerticesLocalCopy[index].r = ModelInfo.pVertices[index].r;
		pVerticesLocalCopy[index].g = ModelInfo.pVertices[index].g;
		pVerticesLocalCopy[index].b = ModelInfo.pVertices[index].b;

		pVerticesLocalCopy[index].nx = ModelInfo.pVertices[index].nx;
		pVerticesLocalCopy[index].ny = ModelInfo.pVertices[index].ny;
		pVerticesLocalCopy[index].nz = ModelInfo.pVertices[index].nz;

		pVerticesLocalCopy[index].u = ModelInfo.pVertices[index].u;
		pVerticesLocalCopy[index].v = ModelInfo.pVertices[index].v;
	}//for (unsigned int index



	unsigned int* pIndicesLocalCopy = new unsigned int[ModelInfo.numberOfIndices];
	for (unsigned int index = 0; index != ModelInfo.numberOfIndices; index++)
	{
		pIndicesLocalCopy[index] = ModelInfo.pIndices[index];
	}//for (unsigned int index

	this->m_ModelVertexInfo.pVertices = pVerticesLocalCopy;
	this->m_ModelVertexInfo.pIndices = pIndicesLocalCopy;



	// From this point on, we are referencing the copy of the original vertices

	// Go through the model info and add the particles from the vertex locations
	this->vec_pParticles.reserve(this->m_ModelVertexInfo.numberOfVertices);

	for (unsigned int index = 0; index != this->m_ModelVertexInfo.numberOfVertices; index++)
	{
		sParticle* pParticle = new sParticle();
		pParticle->position.x = this->m_ModelVertexInfo.pVertices[index].x;
		pParticle->position.y = this->m_ModelVertexInfo.pVertices[index].y;
		pParticle->position.z = this->m_ModelVertexInfo.pVertices[index].z;

		pParticle->old_position = pParticle->position;

		// Save the pointer address.
		// Note the "&" (address of)
		pParticle->pModelVertex = &(this->m_ModelVertexInfo.pVertices[index]);

		this->vec_pParticles.push_back(pParticle);
	}//for ( unsigned int index =...

	// Constraints
	// Note: we are stepping through the array by 3s (3 vertices per triangle)
	for (unsigned int index = 0; index < this->m_ModelVertexInfo.numberOfIndices; index += 3)
	{
		// Example: ply file has “3 733 1026 736” for this triangle.
		// pIndices would have:
		// 
		//	 733	this->m_ModelVertexInfo.pIndices[ index + 0 ]
		//  1026	this->m_ModelVertexInfo.pIndices[ index + 1 ]
		//   736	this->m_ModelVertexInfo.pIndices[ index + 2 ]
		//
		sParticle* pParticle1 = this->vec_pParticles[this->m_ModelVertexInfo.pIndices[index /* +0 */]];
		sParticle* pParticle2 = this->vec_pParticles[this->m_ModelVertexInfo.pIndices[index + 1]];
		sParticle* pParticle3 = this->vec_pParticles[this->m_ModelVertexInfo.pIndices[index + 2]];

		sConstraint* pEdge1 = new sConstraint();
		pEdge1->pParticleA = pParticle1;
		pEdge1->pParticleB = pParticle2;
		pEdge1->restLength = this->calcDistanceBetween(pEdge1->pParticleA, pEdge1->pParticleB)* restLengthMultiplier;
		pEdge1->maxIterations = constIterations;

		sConstraint* pEdge2 = new sConstraint();
		pEdge2->pParticleA = pParticle2;
		pEdge2->pParticleB = pParticle3;
		pEdge2->restLength = this->calcDistanceBetween(pEdge2->pParticleA, pEdge2->pParticleB)* restLengthMultiplier;
		pEdge2->maxIterations = constIterations;

		sConstraint* pEdge3 = new sConstraint();
		pEdge3->pParticleA = pParticle3;
		pEdge3->pParticleB = pParticle1;
		pEdge3->restLength = this->calcDistanceBetween(pEdge3->pParticleA, pEdge3->pParticleB)* restLengthMultiplier;
		pEdge3->maxIterations = constIterations;

		this->vec_pConstraints.push_back(pEdge1);
		this->vec_pConstraints.push_back(pEdge2);
		this->vec_pConstraints.push_back(pEdge3);
	}//for ( unsigned int index

	return true;
}

void cSoftBodyVerlet::UpdateVertexPositions(void)
{
	// Copy the current particle positions to the local vertex locations

	// ENTER CRITICAL SECTION
	for (sParticle* curParticle : this->vec_pParticles)
	{
		curParticle->pModelVertex->x = curParticle->position.x;
		curParticle->pModelVertex->y = curParticle->position.y;
		curParticle->pModelVertex->z = curParticle->position.z;
	}
	// LEAVE CRITICAL SECTION

	// At this point, our local sModelDrawInfo pVertices array has the 
	//	current locations of the particles (i.e. the mesh is updated)

	// TODO: Update normals, etc.

	return;
}
void cSoftBodyVerlet::UpdateNormals(void)
{
	// Clear existing normals
	for (unsigned int vertIndex = 0; vertIndex < m_ModelVertexInfo.numberOfVertices; vertIndex++)
	{
		m_ModelVertexInfo.pVertices[vertIndex].nx = 0.0f;
		m_ModelVertexInfo.pVertices[vertIndex].ny = 0.0f;
		m_ModelVertexInfo.pVertices[vertIndex].nz = 0.0f;
	}

	// Accumulate normals from triangles
	for (unsigned int triIndex = 0; triIndex < m_ModelVertexInfo.numberOfIndices; triIndex += 3)
	{
		unsigned int vertAIndex = m_ModelVertexInfo.pIndices[triIndex + 0];
		unsigned int vertBIndex = m_ModelVertexInfo.pIndices[triIndex + 1];
		unsigned int vertCIndex = m_ModelVertexInfo.pIndices[triIndex + 2];

		// Use particle positions instead of vertex positions
		glm::vec3 posA = vec_pParticles[vertAIndex]->position;
		glm::vec3 posB = vec_pParticles[vertBIndex]->position;
		glm::vec3 posC = vec_pParticles[vertCIndex]->position;

		// Calculate triangle edges
		glm::vec3 edgeAB = posB - posA;
		glm::vec3 edgeAC = posC - posA;

		// Skip degenerate triangles
		if (glm::length(edgeAB) < 0.0001f || glm::length(edgeAC) < 0.0001f) {
			continue;
		}

		// Calculate face normal (cross product of edges)
		glm::vec3 faceNormal = glm::cross(edgeAB, edgeAC);
		faceNormal = glm::normalize(faceNormal);

		// Accumulate face normal to each vertex
		m_ModelVertexInfo.pVertices[vertAIndex].nx += faceNormal.x;
		m_ModelVertexInfo.pVertices[vertAIndex].ny += faceNormal.y;
		m_ModelVertexInfo.pVertices[vertAIndex].nz += faceNormal.z;

		m_ModelVertexInfo.pVertices[vertBIndex].nx += faceNormal.x;
		m_ModelVertexInfo.pVertices[vertBIndex].ny += faceNormal.y;
		m_ModelVertexInfo.pVertices[vertBIndex].nz += faceNormal.z;

		m_ModelVertexInfo.pVertices[vertCIndex].nx += faceNormal.x;
		m_ModelVertexInfo.pVertices[vertCIndex].ny += faceNormal.y;
		m_ModelVertexInfo.pVertices[vertCIndex].nz += faceNormal.z;
	}

	// Normalize accumulated normals
	for (unsigned int vertIndex = 0; vertIndex < m_ModelVertexInfo.numberOfVertices; vertIndex++)
	{
		glm::vec3 normal = glm::vec3(
			m_ModelVertexInfo.pVertices[vertIndex].nx,
			m_ModelVertexInfo.pVertices[vertIndex].ny,
			m_ModelVertexInfo.pVertices[vertIndex].nz
		);

		normal = glm::normalize(normal);

		m_ModelVertexInfo.pVertices[vertIndex].nx = normal.x;
		m_ModelVertexInfo.pVertices[vertIndex].ny = normal.y;
		m_ModelVertexInfo.pVertices[vertIndex].nz = normal.z;
	}
}

float cSoftBodyVerlet::calcDistanceBetween(sParticle* pPartA, sParticle* pPartB)
{
	return glm::distance(pPartA->position, pPartB->position);
}

void cSoftBodyVerlet::LockParticlesOnY(float yPos, bool lower)
{
	for (sParticle* particle : vec_pParticles)
	{
		if (lower)
		{
			if (particle->position.y < yPos)
			{
				std::cout << "Particle Locked at Y: " << particle->position.y << std::endl;
				particle->bIsFixed_DontUpdate = true;
			}
		}
		else
		{
			if (particle->position.y > yPos)
			{
				std::cout << "Particle Locked at Y: " << particle->position.y << std::endl;
				particle->bIsFixed_DontUpdate = true;
			}
		}
	}

}

void cSoftBodyVerlet::LockParticlesOnZ(float yPos, bool lower)
{
	for (sParticle* particle : vec_pParticles)
	{
		if (lower)
		{
			if (particle->position.z < yPos)
			{
				std::cout << "Particle Locked at Z: " << particle->position.z << std::endl;
				particle->bIsFixed_DontUpdate = true;
			}
		}
		else
		{
			if (particle->position.z > yPos)
			{
				std::cout << "Particle Locked at Z: " << particle->position.z << std::endl;
				particle->bIsFixed_DontUpdate = true;
			}
		}
	}

}

void cSoftBodyVerlet::CalculateBaseVolume()
{
	// Store the base "volume" as the average radius at the start
	volume = GetVolume()*3;
}



float cSoftBodyVerlet::GetVolume()
{
	// Here, "volume" is really the average distance from the geometric center.
	if (vec_pParticles.empty())
		return 0.0f;

	glm::vec3 geoCenter = getGeometricCentrePoint();
	float sum = 0.0f;

	for (sParticle* particle : vec_pParticles)
	{
		if (glm::any(glm::isnan(particle->position)))
		{
			std::cout << "Particle has NaN position!" << std::endl;
		}
		sum += glm::distance(particle->position, geoCenter);
	}
	return sum / static_cast<float>(vec_pParticles.size());
}

void cSoftBodyVerlet::ApplyVolumeCorrection()
{
	// Compute the current average radius
	float currentAvgRadius = GetVolume();

	// "Error" is the difference between current and base average radius
	float error = currentAvgRadius - volume;

	// Define a tolerance so that very small differences are ignored (here, 0.1% of base)
	float tolerance = 0.001f * volume;
	if (fabs(error) < tolerance)
		return;  // Skip minor corrections

	glm::vec3 geoCenter = getGeometricCentrePoint();

	// Compute corrections for each particle
	std::vector<glm::vec3> corrections;
	corrections.reserve(vec_pParticles.size());
	glm::vec3 totalCorrection(0.0f);

	for (sParticle* particle : vec_pParticles)
	{
		
		glm::vec3 dir = particle->position - geoCenter;
		float len = glm::length(dir);
		glm::vec3 correction(0.0f);

		// Avoid division by zero if particle is exactly at the center
		if (len > 0.0001f)
		{
			dir.y *= 1.2;
			//dir.y *= 2;
			dir = glm::normalize(dir);
			// If currentAvgRadius is greater than base (error > 0), we need to pull particles inward.
			// If error is negative, we need to push particles outward.
			correction = -dir * error;
		}

		corrections.push_back(correction);
		totalCorrection += correction;
	}

	// Compute average correction to cancel net translation
	glm::vec3 avgCorrection = totalCorrection / static_cast<float>(vec_pParticles.size());

	// Use a damping factor to apply corrections gradually
	float dampingFactor = 1.f;

	for (size_t i = 0; i < vec_pParticles.size(); i++)
	{
		// Subtract the average correction so that the center remains fixed
		glm::vec3 finalCorrection = (corrections[i] - avgCorrection) * dampingFactor;
		vec_pParticles[i]->position += finalCorrection;
	}
}

void cSoftBodyVerlet::VerletUpdate(double deltaTime)
{
	if (deltaTime > MAX_DELTATIME)
	{
		deltaTime = MAX_DELTATIME;
	}


	for (sParticle* pCurrentParticle : vec_pParticles)
	{
		glm::vec3 current_pos = pCurrentParticle->position;
		glm::vec3 old_pos = pCurrentParticle->old_position;

		// From Michael's ancient (2016) code:
		//		CVector& x = this->m_vec_x[index];
		//		CVector temp = x;
		//		CVector& oldx = this->m_vec_oldx[index];
		//
		//		CVector& a = this->m_vec_a[index];
		//		x += x - oldx + (a * fTimeStep * fTimeStep);
		//
		//		oldx = temp;

		if (!pCurrentParticle->bIsFixed_DontUpdate)
		{

			// This is the actual Verlet integration step (notice there isn't a velocity)
			const float dampingFactor = 0.95f; // Experiment with values between 0.9 and 1.0
			glm::vec3 velocity = (current_pos - old_pos) * dampingFactor; 
			pCurrentParticle->position += velocity + (this->acceleration * (float)(deltaTime * deltaTime));

			pCurrentParticle->old_position = current_pos;

			// Check if there is a LARGE different between old and new positions


			this->cleanZeros(pCurrentParticle->position);
			this->cleanZeros(pCurrentParticle->old_position);
		}
	}

	return;
}

void cSoftBodyVerlet::ApplyCollision(double deltaTime, SoftBodyCollision* sbCollision, glm::vec3 worldPosition, float scale)
{
	// HACK: Stop any particles that go below the "ground"
	for (sParticle* pCurrentParticle : vec_pParticles)
	{
		glm::vec3 particleWorldPosition = scale * pCurrentParticle->position + worldPosition;
		glm::vec3 posChange = sbCollision->ProcessMazeCollision(particleWorldPosition)/ scale; 

		pCurrentParticle->position += posChange;
		particleWorldPosition += posChange;

		
		posChange = sbCollision->ProcessCollisionToOtherSoftBodies(particleWorldPosition);
		pCurrentParticle->position += posChange;

	/*	if (posChange!=glm::vec3(0.f))
		{
			pCurrentParticle->old_position = pCurrentParticle->position;

		}*/
	}

	//	this->vec_pParticles[5'000]->position = glm::vec3(0.0f, 30.0f, 0.0f);

		// Collide with a sphere at 20 units above the origin
		//	with a radius of 5 units.
		// Check to see if this particle is inside this sphere...
//	for (sParticle* pCurrentParticle : vec_pParticles)
	//{
	//	// Slightly off centre... 
	//	glm::vec3 sphereCentre = glm::vec3(-1.0f, -30.0f, 1.0f);
	//	float sphereRadius = 15.0f;

	//	float distanceToSphere = glm::distance(pCurrentParticle->position,
	//		sphereCentre);
	//	if (distanceToSphere < sphereRadius)
	//	{
	//		// it's 'inside' the sphere
	//		// Shift or slide the point along the ray from the centre of the sphere
	//		glm::vec3 particleToCentreRay = pCurrentParticle->position - sphereCentre;
	//		// Normalize to get the direction
	//		particleToCentreRay = glm::normalize(particleToCentreRay);
	//		// 
	//		pCurrentParticle->position = (particleToCentreRay * sphereRadius) + sphereCentre;

	//	}
	//}//for (sParticle* pCurrentParticle



	return;
}


void cSoftBodyVerlet::SatisfyConstraints(void)
{
	const unsigned int MAX_GLOBAL_ITERATIONS = 5;
	const unsigned int VOLUME_CORRECTIONITERATIONS = 50;



	if (useVolume)
		for (unsigned int iteration = 0; iteration != VOLUME_CORRECTIONITERATIONS; iteration++)
		{
			ApplyVolumeCorrection();
		}


	for (unsigned int iteration = 0; iteration != MAX_GLOBAL_ITERATIONS; iteration++)
	{
		
		// This is ONE pass of the constraint resolution
		for (sConstraint* pCurConstraint : this->vec_pConstraints)
		{
			// Ripped or broken
			if (!pCurConstraint->bIsActive)
			{
				continue;
			}
			// How many iterations for THIS constraint
			if (iteration > pCurConstraint->maxIterations)
			{
				continue;
			}

			// Constraint (edge) IS active, etc. 
			cSoftBodyVerlet::sParticle* pX1 = pCurConstraint->pParticleA;
			cSoftBodyVerlet::sParticle* pX2 = pCurConstraint->pParticleB;

			glm::vec3 delta = pX2->position - pX1->position;

			float deltaLength = glm::length(delta);

			// TODO: 
			// Do we break this when it's "too far apart"
			if (pCurConstraint->bIsBreakable)
			{
				if (deltaLength >= pCurConstraint->breakingDistance)
				{
					// Disable this contraint the next time
					// i.e. "it's ripped" or broken
					pCurConstraint->bIsActive;
					continue;
				}
			}


			if (pCurConstraint->restLength == 0)
			{
				glm::vec3 averagePos = (pX1->position + pX2->position) * 0.5f;
				pX1->position = averagePos;
				pX2->position = averagePos;
				this->cleanZeros(pX1->position);
				this->cleanZeros(pX2->position);
				continue;
			}

			float diff = (deltaLength - pCurConstraint->restLength) / deltaLength;
			float exponent = 1.f; // Tune this value; >1 makes small deviations even smaller, and large deviations larger.
			float nonLinearDiff = (diff >= 0.0f ? pow(diff, exponent) : -pow(-diff, exponent));

			


			// Making this non-one, will change how quickly the objects move together
			// For example, making this < 1.0 will make it "bouncier"
			float tightnessFactor = 1.f;
			if (!pX1->bIsFixed_DontUpdate)
			pX1->position += delta * 0.5f * nonLinearDiff * tightnessFactor;

			if (!pX2->bIsFixed_DontUpdate)
			pX2->position -= delta * 0.5f * nonLinearDiff * tightnessFactor;

			this->cleanZeros(pX1->position);
			this->cleanZeros(pX2->position);

		}//for (sConstraint* pCurConstraint.
		
	
	
	}//for ( unsigned int iteration
	
	return;
}

void cSoftBodyVerlet::cleanZeros(glm::vec3& value)
{
	// 1.192092896e–07F 
	const float minFloat = 1.192092896e-07f;
	if ((value.x < minFloat) && (value.x > -minFloat))
	{
		value.x = 0.0f;
	}
	if ((value.y < minFloat) && (value.y > -minFloat))
	{
		value.y = 0.0f;
	}
	if ((value.z < minFloat) && (value.z > -minFloat))
	{
		value.z = 0.0f;
	}
}

// Michael's original 2016 code from the original Hitman article on gamasutra:
// from the satisfyConstraints() method
//for (int itCount = 0; itCount != 10; itCount++)
////for ( int itCount = 0; itCount != this->numIterations; itCount++ )
//{
//	int numConstraints = this->m_vecConstraints.size();
//	for (int index = 0; index != numConstraints; index++)
//	{
//
//		// Apply constraints...
//		if (index > static_cast<int>(this->m_vecConstraints.size()))
//		{
//			int x = 0;
//		}
//		CConstraint& c = this->m_vecConstraints[index];
//		// 
//		unsigned int indexA = c.particleA;
//		unsigned int indexB = c.particleB;
//
//		if (indexA > this->m_numParticles)
//		{
//			int stop = 0;
//		}
//		if (indexB > this->m_numParticles)
//		{
//			int stop = 0;
//		}
//z
//		CVector& x1 = this->m_vec_x[indexA];
//		CVector& x2 = this->m_vec_x[indexB];
//		CVector delta = x2 - x1;
//		float deltaMag = delta.Magnitude();
//		float deltalength = sqrt(deltaMag * deltaMag);
//
//		// Speed up by placing this on separate lines... why? who knows?
//		// (Actually, I think it's because of some wonkly bug in the CVector3f...)
//		//float diff=( deltalength - c.restlength ) / deltalength;
//
//		float diffA = (deltalength - c.restlength);
//		float diff = diffA / deltalength;
//
//		// Making this non-one, will change how quickly the objects move together
//		// For example, making this < 1.0 will make it "bouncier"
//		float tightnessFactor = 1.0f;
//
//		x1 += delta * 0.5 * diff * tightnessFactor;
//		x2 -= delta * 0.5 * diff * tightnessFactor;
//
//		x1.CleanZero();
//		x2.CleanZero();
//	}
//}// for ( int itCount = 0; itCount != this->numIterations; 


glm::vec3 cSoftBodyVerlet::getGeometricCentrePoint(void)
{
	glm::vec3 centreXYZ = glm::vec3(0.0f);

	for (sParticle* pCurrentParticle : this->vec_pParticles)
	{
		centreXYZ += pCurrentParticle->position;
	}
	// Get average
	centreXYZ /= (float)this->vec_pParticles.size();

	return centreXYZ;
}









void cSoftBodyVerlet::CreateRandomBracing(unsigned int numberOfBraces,
	float minDistanceBetweenVertices)
{
	for (unsigned int count = 0; count != numberOfBraces; count++)
	{
		// Assume the distance is OK
		bool bKeepLookingForParticles = false;

		do
		{
			// Assume the distance is OK
			bKeepLookingForParticles = false;

			// Pick two random vertices
			// NOTE: Here, rand() might not be great because there's usually
			//	onlly about 32,000 possible integer values...
			// Meaning that if your are chosing from something LARGER than
			//	around 32,000, you'll miss a bunch of values. 
			// HOWEVER, you could also multiply rand() by itself
			unsigned int particleIndex1 = rand() % this->vec_pParticles.size();
			unsigned int particleIndex2 = rand() % this->vec_pParticles.size();

			sParticle* pParticle1 = this->vec_pParticles[particleIndex1];
			sParticle* pParticle2 = this->vec_pParticles[particleIndex2];
			float distBetween = this->calcDistanceBetween(pParticle1, pParticle2);

			// Distance OK?
			if (distBetween < minDistanceBetweenVertices)
			{
				// No
				bKeepLookingForParticles = true;
			}
			else
			{
				// Distance is OK, so make a constraint
				sConstraint* pBracingConstraint = new sConstraint();
				pBracingConstraint->pParticleA = pParticle1;
				pBracingConstraint->pParticleB = pParticle2;
				pBracingConstraint->restLength = this->calcDistanceBetween(pBracingConstraint->pParticleA, pBracingConstraint->pParticleB);

				this->vec_pConstraints.push_back(pBracingConstraint);
			}

		} while (bKeepLookingForParticles);

	}

	return;
}


