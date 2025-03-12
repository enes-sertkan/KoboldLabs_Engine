#pragma once

#include "Action.h"
#include "cVAOManager/cVAOManager.h"
#include "cSoftBodyVerlet.hpp"
#include <string>
#include "cSoftBodyCollisions.h"
#include "MazeGenerator.hpp"
class Object;

class SoftBody : public Action {
private:
   
public:
    cSoftBodyVerlet* softBody = nullptr;
    std::string originalMeshName;
    std::string SBMeshName;
    glm::vec3 acceleration;
    bool isLockOnZ = false;
    float zLockPos = 0.f;
    bool checkGreaterZLock = true;
    bool wind = true;
    bool useVolume = false;

    float yPosToLock;
    SoftBodyCollision* sbCollision = new SoftBodyCollision();

    void SetMazeToSBCollision(MazeGenerator* mazeGenerator)
    {
        sbCollision->mazeGenerator = mazeGenerator;
    }

    void Start() override {
        softBody = new cSoftBodyVerlet();
        originalMeshName = object->mesh->modelFileName;
        sModelDrawInfo drawInfo;
        object->scene->vaoManager->FindDrawInfoByModelName(originalMeshName, drawInfo);
        SBMeshName = originalMeshName + "SB";
        object->scene->vaoManager->CloneMeshToDynamicVAO(SBMeshName, drawInfo, object->scene->programs[0]);
    
        softBody->CreateSoftBody(drawInfo);   
        if (!isLockOnZ)//BAD CODE BAD CODE
        //softBody->CreateRandomBracing(300, 0.8f);
       
        object->mesh->modelFileName = SBMeshName;
        softBody->acceleration = acceleration;

        softBody->CreateConstraintsBetweenCloseVertices(0.01f);

        if (isLockOnZ)
            softBody->LockParticlesOnY(zLockPos, checkGreaterZLock);
      
        
        softBody->useVolume = useVolume;
        
        //softBody->LockParticlesOnY(yPosToLock, true);
        softBody->CalculateBaseVolume();
    }

    void MoveTopPart()
    {

    }

    
    void Update() override {
        // Update any other soft body logic here, e.g. Verlet integration,
        // constraint satisfaction, collisions, etc.
        UpdateSoftBody(object->scene->deltaTime);
        UpdateSoftBodyMeshes(object->scene->programs[0]);


        // Check each arrow key independently:
        if (glfwGetKey(object->scene->window, GLFW_KEY_LEFT) == GLFW_PRESS) {
            // Apply a leftward force (negative X) only to particles above the center.
            glm::vec3 force = glm::vec3(-1.f * object->scene->deltaTime, 0.0f, 0.0f);
            ApplyForceAboveCenter(force);
        }
        if (glfwGetKey(object->scene->window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
            // Apply a rightward force (positive X).
            glm::vec3 force = glm::vec3(1.f * object->scene->deltaTime, 0.0f, 0.0f);
            ApplyForceAboveCenter(force);
        }
        if (glfwGetKey(object->scene->window, GLFW_KEY_UP) == GLFW_PRESS) {
            // Apply a forward force (positive Z).
            glm::vec3 force = glm::vec3(0.0f, 0.0f, 1.f * object->scene->deltaTime);
            ApplyForceAboveCenter(force);
        }
        if (glfwGetKey(object->scene->window, GLFW_KEY_DOWN) == GLFW_PRESS) {
            // Apply a backward force (negative Z).
            glm::vec3 force = glm::vec3(0.0f, 0.0f, -1.f * object->scene->deltaTime);
            ApplyForceAboveCenter(force);
        }
    }

    void AddSoftBodyToCollisions(SoftBody* softBody)
    {
        sbCollision->otherSoftBodies.push_back(softBody);
    }


    void UpdateSoftBody(double deltaTime) {
      

          //  if (softBody!=nullptr) return;

            // Apply Verlet integration steps
            softBody->VerletUpdate(deltaTime);
    
            softBody->SatisfyConstraints();

   
            softBody->ApplyCollision(deltaTime, sbCollision, object->mesh->positionXYZ, object->mesh->uniformScale);
            glm::vec3 center = softBody->getGeometricCentrePoint() + object->mesh->positionXYZ;
            std::cout << center.x << " " << center.y << " " << center.z << std::endl;
           
        
    }

    void UpdateSoftBodyMeshes(unsigned int shaderProgramID) {
      
            if (!softBody) return;

            // Update normals
            softBody->UpdateNormals();

            // Find mesh in VAO
            sModelDrawInfo softBodyDrawMeshLocalCopy;

            if (!object->scene->vaoManager->FindDrawInfoByModelName(SBMeshName, softBodyDrawMeshLocalCopy))
                return;

            // Update mesh vertex positions and normals
            for (unsigned int i = 0; i < softBodyDrawMeshLocalCopy.numberOfVertices; ++i) {
                softBodyDrawMeshLocalCopy.pVertices[i].x = softBody->vec_pParticles[i]->position.x;
                softBodyDrawMeshLocalCopy.pVertices[i].y = softBody->vec_pParticles[i]->position.y;
                softBodyDrawMeshLocalCopy.pVertices[i].z = softBody->vec_pParticles[i]->position.z;

                softBodyDrawMeshLocalCopy.pVertices[i].nx = softBody->vec_pParticles[i]->pModelVertex->nx;
                softBodyDrawMeshLocalCopy.pVertices[i].ny = softBody->vec_pParticles[i]->pModelVertex->ny;
                softBodyDrawMeshLocalCopy.pVertices[i].nz = softBody->vec_pParticles[i]->pModelVertex->nz;
            }

            // Update VAO mesh
            object->scene->vaoManager->UpdateDynamicMesh(softBody->matching_VAO_MeshName, softBodyDrawMeshLocalCopy, shaderProgramID);
        
    }

    glm::vec3 GetParticlePosition(int particleID)
    {
        if (softBody->vec_pParticles.size() < particleID)
        {
            std::cout << "There is no particle with id of " << particleID << std::endl;
            return glm::vec3(0);

     }

        glm::vec3 worldPosition = object->mesh->uniformScale * softBody->vec_pParticles[particleID]->position + object->mesh->positionXYZ;
        return worldPosition;

    }

    void ApplyForceAboveCenter(glm::vec3 force)
    {
        force*=0.1f;
        for (cSoftBodyVerlet::sParticle* particle : softBody->vec_pParticles)
        {
            glm::vec3 center = softBody->getGeometricCentrePoint();
            // Check if the particle is above the given center point (based on y-axis)
            if (particle->position.y > center.y)
            {
                // Apply the force to this particle (you might also multiply by deltaTime if needed)
                particle->position += force;
            }
        }
    }

};
