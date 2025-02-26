#ifndef VERLET_PHYSICS_HPP
#define VERLET_PHYSICS_HPP

#include "cSoftBodyVerlet.hpp"
#include "cVAOManager/cVAOManager.h"
#include "cPhysics.h"
#include <map>
#include <string>

class VerletPhysics {
private:
    cVAOManager* m_pVAOManager = nullptr;

public:
    std::map<std::string, cSoftBodyVerlet*> m_MapSoftBodiesByName;

    void updateSoftBodyMeshes(unsigned int shaderProgramID) {
        std::map<std::string, cSoftBodyVerlet*>::iterator itSB;
        for (itSB = m_MapSoftBodiesByName.begin(); itSB != m_MapSoftBodiesByName.end(); ++itSB) {
            cSoftBodyVerlet* pCurrentSB = itSB->second;
            if (!pCurrentSB) continue;

            // Update normals
            pCurrentSB->UpdateNormals();

            // Find mesh in VAO
            sModelDrawInfo softBodyDrawMeshLocalCopy;
            if (!m_pVAOManager->FindDrawInfoByModelName(pCurrentSB->matching_VAO_MeshName, softBodyDrawMeshLocalCopy))
                continue;

            // Update mesh vertex positions and normals
            for (unsigned int i = 0; i < softBodyDrawMeshLocalCopy.numberOfVertices; ++i) {
                softBodyDrawMeshLocalCopy.pVertices[i].x = pCurrentSB->vec_pParticles[i]->position.x;
                softBodyDrawMeshLocalCopy.pVertices[i].y = pCurrentSB->vec_pParticles[i]->position.y;
                softBodyDrawMeshLocalCopy.pVertices[i].z = pCurrentSB->vec_pParticles[i]->position.z;

                softBodyDrawMeshLocalCopy.pVertices[i].nx = pCurrentSB->vec_pParticles[i]->pModelVertex->nx;
                softBodyDrawMeshLocalCopy.pVertices[i].ny = pCurrentSB->vec_pParticles[i]->pModelVertex->ny;
                softBodyDrawMeshLocalCopy.pVertices[i].nz = pCurrentSB->vec_pParticles[i]->pModelVertex->nz;
            }

            // Update VAO mesh
            m_pVAOManager->UpdateDynamicMesh(pCurrentSB->matching_VAO_MeshName, softBodyDrawMeshLocalCopy, shaderProgramID);
        }
    }

    void updateSoftBodies(double deltaTime) {
        std::map<std::string, cSoftBodyVerlet*>::iterator itSB;
        for (itSB = m_MapSoftBodiesByName.begin(); itSB != m_MapSoftBodiesByName.end(); ++itSB) {
            cSoftBodyVerlet* pCurrentSB = itSB->second;
            if (!pCurrentSB) continue;

            // Apply Verlet integration steps
            pCurrentSB->VerletUpdate(deltaTime);
            pCurrentSB->ApplyCollision(deltaTime);
            pCurrentSB->SatisfyConstraints();
        }
    }
};

#endif // VERLET_PHYSICS_HPP
