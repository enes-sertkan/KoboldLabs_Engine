#pragma once

#include "Action.h"
#include "cVAOManager/cVAOManager.h"
#include "cSoftBodyVerlet.hpp"

class Object;

class SoftBody : public Action {
private:
    cSoftBodyVerlet* m_pSoftBody = nullptr;
    cVAOManager* m_pVAOManager = nullptr;
    std::string m_meshName;

public:
    SoftBody(cSoftBodyVerlet* softBody, cVAOManager* vaoManager, const std::string& meshName)
        : m_pSoftBody(softBody), m_pVAOManager(vaoManager), m_meshName(meshName) {}

    void Start() override {
        if (!m_pSoftBody || !m_pVAOManager) return;

        // Create a new dynamic mesh for the soft body
        sModelDrawInfo drawInfo;
        if (!m_pVAOManager->FindDrawInfoByModelName(m_meshName, drawInfo)) {
            return;  // Mesh not found
        }

        // Associate the soft body with the VAO mesh
        m_pSoftBody->matching_VAO_MeshName = m_meshName;
    }

    void Update() override {
        if (!m_pSoftBody || !m_pVAOManager) return;

        // Update soft body physics
        m_pSoftBody->UpdateNormals();

        // Retrieve the dynamic mesh data
        sModelDrawInfo drawInfo;
        if (!m_pVAOManager->FindDrawInfoByModelName(m_meshName, drawInfo)) {
            return;
        }

        // Update mesh vertex positions and normals
        for (unsigned int i = 0; i < drawInfo.numberOfVertices; ++i) {
            drawInfo.pVertices[i].x = m_pSoftBody->vec_pParticles[i]->position.x;
            drawInfo.pVertices[i].y = m_pSoftBody->vec_pParticles[i]->position.y;
            drawInfo.pVertices[i].z = m_pSoftBody->vec_pParticles[i]->position.z;

            drawInfo.pVertices[i].nx = m_pSoftBody->vec_pParticles[i]->pModelVertex->nx;
            drawInfo.pVertices[i].ny = m_pSoftBody->vec_pParticles[i]->pModelVertex->ny;
            drawInfo.pVertices[i].nz = m_pSoftBody->vec_pParticles[i]->pModelVertex->nz;
        }

        // Update the dynamic mesh in the VAO
        m_pVAOManager->UpdateDynamicMesh(m_meshName, drawInfo, 0);
    }
};
