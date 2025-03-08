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
    bool wind = true;
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

       
        object->mesh->modelFileName = SBMeshName;
        softBody->acceleration = acceleration;

        //softBody->LockParticlesOnY(yPosToLock, true);
    }

    void Update() override {
    
        if (glfwGetKey(object->scene->window, GLFW_KEY_T) == GLFW_PRESS)
        {
            wind = true;
        }

        if (glfwGetKey(object->scene->window, GLFW_KEY_Y) == GLFW_PRESS)
        {
            wind = false;
        }


        //if (wind)
        //{
        //    softBody->acceleration.y = 0.1;
        //    softBody->acceleration.x = -0.5;
        //    softBody->acceleration.z = -0.05;
        //}
        //else

        //{
        //    softBody->acceleration.y = 0;
        //    softBody->acceleration.x = 0;
        //    softBody->acceleration.z = -0.5;
        //}

        UpdateSoftBody(object->scene->deltaTime);
        UpdateSoftBodyMeshes(object->scene->programs[0]);
    }



    void UpdateSoftBody(double deltaTime) {
      

          //  if (softBody!=nullptr) return;

            // Apply Verlet integration steps
            softBody->VerletUpdate(deltaTime);
            softBody->ApplyCollision(deltaTime, sbCollision);
            softBody->SatisfyConstraints();
        
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



};
