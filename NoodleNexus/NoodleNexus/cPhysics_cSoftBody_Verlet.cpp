//
//#include "cPhysics.h"
//#include "cSoftBodyVerlet.hpp"
////	#include "../sharedThings.h"
//
//
//cSoftBodyVerlet* cPhysics::createSoftBodyFromMesh(std::string meshFriendlyName, glm::mat4 matModel, std::string& error)
//{
//    sModelDrawInfo softBodyMeshDrawingInfo;
//
//    if (!this->m_pVAOManager->FindDrawInfoByModelName(meshFriendlyName, softBodyMeshDrawingInfo))
//    {
//        error = "Can't find >" + meshFriendlyName + "< mesh";
//        return NULL;
//    }
//
//
//    cSoftBodyVerlet* pNewSoftBody = new cSoftBodyVerlet();
//
//    pNewSoftBody->matching_VAO_MeshName = meshFriendlyName;
//
//    pNewSoftBody->CreateSoftBody(softBodyMeshDrawingInfo, matModel);
//
//    //    pNewSoftBody->CreateRandomBracing(20, 1.0f);
//
//        // Add this soft body to the map of soft bodies
//    this->m_MapSoftBodiesByName[meshFriendlyName] = pNewSoftBody;
//
//    return pNewSoftBody;
//}
//
//
//
//
//
//cSoftBodyVerlet* cPhysics::pFindSoftBodyByFriendlyName(std::string friendlyName)
//{
//    std::map< std::string /*freindly name*/, cSoftBodyVerlet* >::iterator itSB
//        = this->m_MapSoftBodiesByName.find(friendlyName);
//
//    // Found it? 
//    if (itSB == this->m_MapSoftBodiesByName.end())
//    {
//        // NO, we didn't
//        return NULL;
//    }
//
//    // return pointer to soft body
//    return itSB->second;
//
//}
//
//
//// 
////	std::map< std::string /*freindly name*/, cSoftBody_Verlet* > m_MapSoftBodiesByName;
//
//
//void cPhysics::updateSoftBodyMeshes(unsigned int shaderProgramID)
//{
//    for (std::map< std::string, cSoftBodyVerlet* >::iterator itSB = this->m_MapSoftBodiesByName.begin();
//        itSB != this->m_MapSoftBodiesByName.end(); itSB++)
//    {
//        cSoftBodyVerlet* pCurrentSB = itSB->second;
//
//        // Update normals
//        pCurrentSB->UpdateNormals();
//
//        // Find the corresponding mesh in the VAO
//        sModelDrawInfo softBodyDrawMeshLocalCopy;
//        this->m_pVAOManager->FindDrawInfoByModelName(pCurrentSB->matching_VAO_MeshName, softBodyDrawMeshLocalCopy);
//
//        // Update the copy of the ModelDrawInfo to reflect the current soft body particle (vertex) info
//        for (unsigned int vertexIndex = 0; vertexIndex != softBodyDrawMeshLocalCopy.numberOfVertices; vertexIndex++)
//        {
//            softBodyDrawMeshLocalCopy.pVertices[vertexIndex].x = pCurrentSB->vec_pParticles[vertexIndex]->position.x;
//            softBodyDrawMeshLocalCopy.pVertices[vertexIndex].y = pCurrentSB->vec_pParticles[vertexIndex]->position.y;
//            softBodyDrawMeshLocalCopy.pVertices[vertexIndex].z = pCurrentSB->vec_pParticles[vertexIndex]->position.z;
//
//            // Also the normals...
//            softBodyDrawMeshLocalCopy.pVertices[vertexIndex].nx = pCurrentSB->vec_pParticles[vertexIndex]->pModelVertex->nx;
//            softBodyDrawMeshLocalCopy.pVertices[vertexIndex].ny = pCurrentSB->vec_pParticles[vertexIndex]->pModelVertex->ny;
//            softBodyDrawMeshLocalCopy.pVertices[vertexIndex].nz = pCurrentSB->vec_pParticles[vertexIndex]->pModelVertex->nz;
//
//            // Also anything else (colours? UVs?)
//
//        }//for (unsigned int vertexIndex
//
//        // Copy this back to the VAO (overwriting the current VAO information)
//        this->m_pVAOManager->UpdateDynamicMesh(pCurrentSB->matching_VAO_MeshName, softBodyDrawMeshLocalCopy, shaderProgramID);
//
//    }
//    return;
//}
//
//
//
//
//
//// Moved to cPhysics_cSoftBody_Verlet_Threaded.cpp
//// 
////    void cPhysics::updateSoftBodies(double deltaTime)
////    {
////        for (std::map< std::string, cSoftBodyVerlet* >::iterator itSB = this->m_MapSoftBodiesByName.begin();
////            itSB != this->m_MapSoftBodiesByName.end(); itSB++)
////        {
////            cSoftBodyVerlet* pCurrentSB = itSB->second;
////
////            // Like the Euler step, but we are using Verlet
////            // (i.e. calculate the next (or current) location based on gravity, etc.
////            pCurrentSB->VerletUpdate(deltaTime);
////
////            // Update for collision constraints
////            pCurrentSB->ApplyCollision(deltaTime);
////
////
////            pCurrentSB->SatisfyConstraints();
////
////            //        pCurrentSB->UpdateDrawInfo();
////        }
////        return;
////    }


//___________
//prev push


//
//
//
//#include "cPhysics.h"
////	#include "../sharedThings.h"
//
//cSoftBodyVerlet* cPhysics::createSoftBodyFromMesh(std::string meshFriendlyName, glm::mat4 matModel, std::string& error)
//{
//    sModelDrawInfo softBodyMeshDrawingInfo;
//
//    if (!this->m_pVAOManager->FindDrawInfoByModelName(meshFriendlyName, softBodyMeshDrawingInfo))
//    {
//        error = "Can't find >" + meshFriendlyName + "< mesh";
//        return NULL;
//    }
//
//
//    cSoftBodyVerlet* pNewSoftBody = new cSoftBodyVerlet();
//
//    pNewSoftBody->matching_VAO_MeshName = meshFriendlyName;
//
//    pNewSoftBody->CreateSoftBody(softBodyMeshDrawingInfo, matModel);
//
//    //    pNewSoftBody->CreateRandomBracing(20, 1.0f);
//
//        // Add this soft body to the map of soft bodies
//    this->m_MapSoftBodiesByName[meshFriendlyName] = pNewSoftBody;
//
//    return pNewSoftBody;
//}
//
//void cPhysics::updateSoftBodies(double deltaTime)
//{
//    for (std::map< std::string, cSoftBodyVerlet* >::iterator itSB = this->m_MapSoftBodiesByName.begin();
//        itSB != this->m_MapSoftBodiesByName.end(); itSB++)
//    {
//        cSoftBodyVerlet* pCurrentSB = itSB->second;
//
//        // Like the Euler step, but we are using Verlet
//        // (i.e. calculate the next (or current) location based on gravity, etc.
//        pCurrentSB->VerletUpdate(deltaTime);
//
//        // Update for collision constraints
//        pCurrentSB->ApplyCollision(deltaTime);
//
//
//        pCurrentSB->SatisfyConstraints();
//
//        //        pCurrentSB->UpdateDrawInfo();
//    }
//    return;
//}
//
//
//void cPhysics::updateSoftBodyMeshes(unsigned int shaderProgramID)
//{
//    for (std::map< std::string, cSoftBodyVerlet* >::iterator itSB = this->m_MapSoftBodiesByName.begin();
//        itSB != this->m_MapSoftBodiesByName.end(); itSB++)
//    {
//        cSoftBodyVerlet* pCurrentSB = itSB->second;
//
//        // Update normals
////        pCurrentSB->UpdateNormals();
//
//        // Find the corresponding mesh in the VAO
//        sModelDrawInfo softBodyDrawMeshLocalCopy;
//        this->m_pVAOManager->FindDrawInfoByModelName(pCurrentSB->matching_VAO_MeshName, softBodyDrawMeshLocalCopy);
//
//        // Update the copy of the ModelDrawInfo to reflect the current soft body particle (vertex) info
//        for (unsigned int vertexIndex = 0; vertexIndex != softBodyDrawMeshLocalCopy.numberOfVertices; vertexIndex++)
//        {
//            softBodyDrawMeshLocalCopy.pVertices[vertexIndex].x = pCurrentSB->vec_pParticles[vertexIndex]->position.x;
//            softBodyDrawMeshLocalCopy.pVertices[vertexIndex].y = pCurrentSB->vec_pParticles[vertexIndex]->position.y;
//            softBodyDrawMeshLocalCopy.pVertices[vertexIndex].z = pCurrentSB->vec_pParticles[vertexIndex]->position.z;
//
//            // Also the normals...
//            softBodyDrawMeshLocalCopy.pVertices[vertexIndex].nx = pCurrentSB->vec_pParticles[vertexIndex]->pModelVertex->nx;
//            softBodyDrawMeshLocalCopy.pVertices[vertexIndex].ny = pCurrentSB->vec_pParticles[vertexIndex]->pModelVertex->ny;
//            softBodyDrawMeshLocalCopy.pVertices[vertexIndex].nz = pCurrentSB->vec_pParticles[vertexIndex]->pModelVertex->nz;
//
//            // Also anything else (colours? UVs?)
//
//        }//for (unsigned int vertexIndex
//
//        // Copy this back to the VAO (overwriting the current VAO information)
//        this->m_pVAOManager->UpdateDynamicMesh(pCurrentSB->matching_VAO_MeshName, softBodyDrawMeshLocalCopy, shaderProgramID);
//
//    }
//    return;
//}
//
//
//
//
////
////	//struct sPoint: 
////	//  glm::vec3 position;
////	//struct sEdge : 
////	//	sPoint* pointA;
////	//	sPoint* pointB;
////	//	float restLength;
////	//
////	//std::vector< sPoint > vecPoints;
////	//std::vector< sEdge > vecEdges;
////
////	bool cPhysics::createSoftBodyFromMesh(std::string friendlyName, sModelDrawInfo* pTheMesh)
////	{
////		cSoftBody_Verlet* pSoftBody = new cSoftBody_Verlet();
////		pSoftBody->friendlyName = friendlyName;
////
////		// Go through the vertices and add them as points to the vector of points
////		for (unsigned int index = 0; index != pTheMesh->numberOfVertices; index++)
////		{
////			cSoftBody_Verlet::sPoint newPoint;
////			newPoint.position.x = pTheMesh->pVertices[index].x;
////			newPoint.position.y = pTheMesh->pVertices[index].y;
////			newPoint.position.z = pTheMesh->pVertices[index].z;
////			// Init the old positions from this position
////			newPoint.oldPosition = newPoint.position;
////
////			pSoftBody->vecPoints.push_back(newPoint);
////		}
////
////		for (unsigned int index = 0; index != pTheMesh->numberOfIndices; index += 3)
////		{
////			// The indices are in multiples of 3:
////			// In the file: 3 950 2582 2583 
////			// But they are "unrolled" into a 1D array
////
////			// There are 3 edges to this triangle
////			unsigned int vert0index = pTheMesh->pIndices[index + 0];
////			unsigned int vert1index = pTheMesh->pIndices[index + 1];
////			unsigned int vert2index = pTheMesh->pIndices[index + 2];
////
////			// Edge 0 --> vert0 to vert1
////			cSoftBody_Verlet::sEdgeConstraint edge0;
////			edge0.pPointA = &(pSoftBody->vecPoints[vert0index]);
////			edge0.vertAindex = vert0index;
////
////			edge0.pPointB = &(pSoftBody->vecPoints[vert1index]);
////			edge0.vertBindex = vert1index;
////
////			// Calcualte rest length between these vertices
////			edge0.restLength = glm::distance(edge0.pPointA->position, edge0.pPointB->position);
////
////			// Add this edge to list of edges
////			pSoftBody->vecEdgeConstraints.push_back(edge0);
////
////
////			// Edge 1 --> vert1 to vert2
////			cSoftBody_Verlet::sEdgeConstraint edge1;
////			edge1.pPointA = &(pSoftBody->vecPoints[vert1index]);
////			edge1.vertAindex = vert1index;
////			edge1.pPointB = &(pSoftBody->vecPoints[vert2index]);
////			edge1.vertBindex = vert2index;
////
////			// Calcualte rest length between these vertices
////			edge1.restLength = glm::distance(edge1.pPointA->position, edge1.pPointB->position);
////
////			// Add this edge to list of edges
////			pSoftBody->vecEdgeConstraints.push_back(edge1);
////
////
////			// Edge 2 --> vert2 to vert0
////			cSoftBody_Verlet::sEdgeConstraint edge2;
////			edge2.pPointA = &(pSoftBody->vecPoints[vert2index]);
////			edge2.vertAindex = vert2index;
////			edge2.pPointB = &(pSoftBody->vecPoints[vert0index]);
////			edge2.vertBindex = vert0index;
////
////			edge2.restLength = glm::distance(edge2.pPointA->position, edge2.pPointB->position);
////
////			// Add this edge to list of edges
////			pSoftBody->vecEdgeConstraints.push_back(edge2);
////		}
////
////		// Add this soft body to the map of soft bodies
////		this->m_MapSoftBodiesByName[friendlyName] = pSoftBody;
////
////		return true;
////	}
////
////	bool cPhysics::setSoftBodyAcceleration(std::string friendlyName, glm::vec3 acceleration)
////	{
////		std::map< 
////			std::string /*freindly name*/,
////			cSoftBody_Verlet* >::iterator itSB = this->m_MapSoftBodiesByName.find(friendlyName);
////
////		// 
////		if (itSB == this->m_MapSoftBodiesByName.end())
////		{
////			// Nope, not found
////			return false;
////		}
////
////		for (cSoftBody_Verlet::sPoint& curPoint : itSB->second->vecPoints)
////		{
////			curPoint.acceleration = acceleration;
////		}
////
////		return true;
////	}
////
////
////
////	cPhysics::cSoftBody_Verlet* cPhysics::pFindSoftBodyByFriendlyName(std::string friendlyName)
////	{
////		std::map< 
////			std::string /*freindly name*/,
////			cSoftBody_Verlet* >::iterator itSB = this->m_MapSoftBodiesByName.find(friendlyName);
////
////		// 
////		if (itSB == this->m_MapSoftBodiesByName.end())
////		{
////			// Nope, not found
////			return NULL;
////		}
////
////		// Return pointer to the soft body
////		return itSB->second; 
////	}
////
////	void cPhysics::cSoftBody_Verlet::integrationStep(double deltaTime)
////	{
////		if (deltaTime > this->maxTimeStep)
////		{
////			deltaTime = this->maxTimeStep;
////		}
////
////		// Perform the Verlet integration step
////		for (cSoftBody_Verlet::sPoint& curPoint : this->vecPoints)
////		{
////			// 
////			// This is the Euler integration step from the StepTick() method
////			//
////			//			//Update the velocity based on the acceleration
////			//			glm::vec3 deltaVelocity = curPoint.acceleration * (float)deltaTime;
////			//			curPoint.velocity += deltaVelocity;
////			//
////			//			// Update the position based on the velocity
////			//			glm::vec3 deltaPosition = curPoint.velocity * (float)deltaTime;
////			//			curPoint.position += deltaPosition;
////
////
////			// But we are using Verlet integration (not using velocity)
////
////			// From: http://jet.ro/files/Gentle_Introduction_to_Physics_Asm05_20050726.pdf
////			// (Page 13)
////			// x_new = 2x - (x_old * a) + ( a * deltaTime^2 )
////			// 
////			glm::vec3 current_pos = curPoint.position;
////			glm::vec3 old_pos = curPoint.oldPosition;
////
////			// From Michael's ancient (2016) code:
////			//		CVector& x = this->m_vec_x[index];
////			//		CVector temp = x;
////			//		CVector& oldx = this->m_vec_oldx[index];
////			//
////			//		CVector& a = this->m_vec_a[index];
////			//		x += x - oldx + (a * fTimeStep * fTimeStep);
////			//
////			//		oldx = temp;
////
////			// This is the actual Verlet integration step (notice there isn't a velocity)
////			curPoint.position += (current_pos - old_pos) + (curPoint.acceleration * (float)(deltaTime * deltaTime));
////
////			curPoint.oldPosition = current_pos;
////
////			// Check if there is a LARGE different between old and new positions
////
////
////			this->cleanZeros(curPoint.position);
////			this->cleanZeros(curPoint.oldPosition);
////
////		}// for (cSoftBody_Verlet::sPoint& curPoint
////	}
////
////	// Michael's original 2016 code from the original Hitman article on gamasutra:
////	// from the satisfyConstraints() method
////	//for (int itCount = 0; itCount != 10; itCount++)
////	////for ( int itCount = 0; itCount != this->numIterations; itCount++ )
////	//{
////	//	int numConstraints = this->m_vecConstraints.size();
////	//	for (int index = 0; index != numConstraints; index++)
////	//	{
////	//
////	//		// Apply constraints...
////	//		if (index > static_cast<int>(this->m_vecConstraints.size()))
////	//		{
////	//			int x = 0;
////	//		}
////	//		CConstraint& c = this->m_vecConstraints[index];
////	//		// 
////	//		unsigned int indexA = c.particleA;
////	//		unsigned int indexB = c.particleB;
////	//
////	//		if (indexA > this->m_numParticles)
////	//		{
////	//			int stop = 0;
////	//		}
////	//		if (indexB > this->m_numParticles)
////	//		{
////	//			int stop = 0;
////	//		}
////	//
////	//		CVector& x1 = this->m_vec_x[indexA];
////	//		CVector& x2 = this->m_vec_x[indexB];
////	//		CVector delta = x2 - x1;
////	//		float deltaMag = delta.Magnitude();
////	//		float deltalength = sqrt(deltaMag * deltaMag);
////	//
////	//		// Speed up by placing this on separate lines... why? who knows?
////	//		// (Actually, I think it's because of some wonkly bug in the CVector3f...)
////	//		//float diff=( deltalength - c.restlength ) / deltalength;
////	//
////	//		float diffA = (deltalength - c.restlength);
////	//		float diff = diffA / deltalength;
////	//
////	//		// Making this non-one, will change how quickly the objects move together
////	//		// For example, making this < 1.0 will make it "bouncier"
////	//		float tightnessFactor = 1.0f;
////	//
////	//		x1 += delta * 0.5 * diff * tightnessFactor;
////	//		x2 -= delta * 0.5 * diff * tightnessFactor;
////	//
////	//		x1.CleanZero();
////	//		x2.CleanZero();
////	//	}
////	//}// for ( int itCount = 0; itCount != this->numIterations; 
////
////	void cPhysics::cSoftBody_Verlet::satisfyConstraints(void)
////	{
////		for (unsigned int iteration = 0; iteration != this->numberOfConstraintIterationSteps; iteration++)
////		{
////			// This is ONE pass of the constraint resolution
////			for (cSoftBody_Verlet::sEdgeConstraint &currentEdgeConstraint : this->vecEdgeConstraints)
////			{
////				if (currentEdgeConstraint.bIsActive)
////				{
////
////					cSoftBody_Verlet::sPoint* pXA = currentEdgeConstraint.pointA;
////					cSoftBody_Verlet::sPoint* pXB = currentEdgeConstraint.pointB;
////
////					glm::vec3 delta = pXA->position - pXB->position;
////
////					float deltaLength = glm::length(delta);
////
////
////					float diff = (deltaLength - currentEdgeConstraint.restLength) / deltaLength;
////
////					int hey = 0;
////
////					if (diff > 0.1f)
////					{
////						hey = 1;
////					}
////
////					// If we were having this 'tear' or break apart, 
////					//	you could check some maximum length and if it's 'too long'
////					//	then the constraint 'breaks'
////					// Handle this by:
////					// - Setting a bool (where it doesn't check the constraint any more)
////					// - Remove the constraint (but removing from a vector is sketchy...)
////
////	//				if ( diff > 0.1f )
////	//				{
////	//					pCurConstraint->bIsActive = false;
////	//				}
////
////					// Making this non-one, will change how quickly the objects move together
////					// For example, making this < 1.0 will make it "bouncier"
////					float tightnessFactor = 1.0f;
////
////					pXA->position += delta * 0.5f * diff * tightnessFactor;
////					pXB->position -= delta * 0.5f * diff * tightnessFactor;
////
////					this->cleanZeros(pXA->position);
////					this->cleanZeros(pXB->position);
////
////				}//if (pCurConstraint->bIsActive)
////
////			}//for (sConstraint* pCurConstraint...
////		}//for ( unsigned int iteration
////
////		return;
////	}
////
////
////	void cPhysics::updateSoftBodies(double deltaTime)
////	{
////		if (deltaTime > this->maxTimeStep)
////		{
////			deltaTime = this->maxTimeStep;
////		}
////
////		// Go through all the soft bodies and update them
////
////		for (std::map<std::string, cSoftBody_Verlet* >::iterator itSB = this->m_MapSoftBodiesByName.begin();
////			 itSB != this->m_MapSoftBodiesByName.end(); itSB++)
////		{
////			// For clarity...
////			cSoftBody_Verlet* pCurrentSB = itSB->second;
////
////	// HACK: Randomly perturbing the vertices to see that they are really being updated
////	//		for (cSoftBody_Verlet::sPoint& curPoint : pCurrentSB->vecPoints)
////	//		{
////	//			curPoint.position.x += ::g_getRandomFloat(-0.01f, 0.01f);
////	//			curPoint.position.y += ::g_getRandomFloat(-0.01f, 0.01f);
////	//			curPoint.position.z += ::g_getRandomFloat(-0.01f, 0.01f);
////	//		}
////
////			pCurrentSB->maxTimeStep = this->maxTimeStep;
////
////			pCurrentSB->integrationStep(deltaTime);
////
////
////			// Text for collisions
////			// 
////			// You would want to do the same point - object collisoin check as before.
////			// For now, we will just test a couple hard coded things
////
////			float groundPlaneY = -50.0f;
////
////			for (cSoftBody_Verlet::sPoint& curPoint : pCurrentSB->vecPoints)
////			{
////				// Did it hit the "ground"?
////
////				if (curPoint.position.y <= groundPlaneY)
////				{
////					curPoint.position.y = groundPlaneY;
////				}
////			}//for (cSoftBody_Verlet::sPoint& curPoint : pCurrentSB->vecPoints)
////
////
////			pCurrentSB->satisfyConstraints();
////
////		}// for (std::map<std::string, cSoftBody_Verlet* >::iterator itSB
////
////
////		return;
////	}
////
////
////
////	void cPhysics::cSoftBody_Verlet::cleanZeros(glm::vec3& value)
////	{
////		// 1.192092896e–07F 
////		const float minFloat = 1.192092896e-07f;
////		if ((value.x < minFloat) && (value.x > -minFloat))
////		{
////			value.x = 0.0f;
////		}
////		if ((value.y < minFloat) && (value.y > -minFloat))
////		{
////			value.y = 0.0f;
////		}
////		if ((value.z < minFloat) && (value.z > -minFloat))
////		{
////			value.z = 0.0f;
////		}
////	}
////
