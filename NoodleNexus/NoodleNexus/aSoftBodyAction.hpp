#pragma once

#include "Action.h"
#include "cVAOManager/cVAOManager.h"
#include "cSoftBodyVerlet.hpp"
#include <string>
#include "cSoftBodyCollisions.h"
#include "MazeGenerator.hpp"
#include "cVAOManager/cVAOManager.h"	// for: sModelDrawInfo	
class Object;
typedef sVertex_SHADER_FORMAT_xyz_rgb_N_UV sVertex;
class SoftBody : public Action {
private:
   
public:
    bool easyControl = false;
    cSoftBodyVerlet* softBody = nullptr;
    std::string originalMeshName;
    std::string SBMeshName;
    glm::vec3 acceleration;
    bool isLockOnY = false;
    float zLockPos = 0.f;
    bool checkGreaterZLock = true;
    bool wind = true;
    bool useVolume = false;
    int constIterations = 3;
    float restLengthMultiplier = 1.f;
    float tighness = 1.f;

    float yPosToLock;
    
    bool isLockOutsideRadius = false;
    float lockRadius = 2.5f;
    float yToJump = -6;
    bool inCylynder = false;
    float cykinderRadious = 0.4f;


    bool randPressed = false;
    SoftBodyCollision* sbCollision = new SoftBodyCollision();

    void SetMazeToSBCollision(MazeGenerator* mazeGenerator)
    {
        sbCollision->mazeGenerator = mazeGenerator;
    }

    void Start() override {
      //  return;
        softBody = new cSoftBodyVerlet();
        originalMeshName = object->mesh->modelFileName;
        sModelDrawInfo drawInfo;
        object->scene->vaoManager->FindDrawInfoByModelName(originalMeshName, drawInfo);
        float randID = rand();
        SBMeshName = originalMeshName + "SB" + std::to_string(randID);
        object->scene->vaoManager->CloneMeshToDynamicVAO(SBMeshName, drawInfo, object->scene->programs[0]);
    
        softBody->CreateSoftBody(drawInfo);   
      //  if (!isLockOnZ)//BAD CODE BAD CODE
        //softBody->CreateRandomBracing(300, 0.8f);
       
        object->mesh->modelFileName = SBMeshName;
        softBody->acceleration = acceleration;

        softBody->CreateConstraintsBetweenCloseVertices(0.01f);

        softBody->UpdateGeometricCentrePoint();

        if (isLockOnY)
            softBody->LockParticlesOnY(zLockPos, checkGreaterZLock);

        if (isLockOutsideRadius)
            softBody->LockParticlesOutsideRadius(lockRadius);
      
        
        softBody->useVolume = useVolume;
        
        //softBody->LockParticlesOnY(yPosToLock, true);
        softBody->CalculateBaseVolume();
        softBody->tightnessFactor = tighness;
        softBody->yToJump = yToJump;
      

        if (inCylynder)
        {
            sbCollision->cylinder = new Cylinder();
            sbCollision->cylinder->cylinderPos = object->mesh->positionXYZ;
            sbCollision->cylinder->cylinderPos.y -= 2.f;
            sbCollision->cylinder->cylinderRadius = cykinderRadious;
            sbCollision->cylinder->cylinderHeight = 2.5f;
        }
    }

    void MoveTopPart()
    {

    }

    
    void Update() override {
      //  return;
        // Update any other soft body logic here, e.g. Verlet integration,
        // constraint satisfaction, collisions, etc.
        UpdateSoftBody(object->scene->deltaTime);
        UpdateSoftBodyMeshes(object->scene->programs[0]);

        DebugDrawNormals();

        if (!easyControl) return;
        // Check each arrow key independently:
        if (glfwGetKey(object->scene->window, GLFW_KEY_DOWN) == GLFW_PRESS) {
            // Apply a leftward force (negative X) only to particles above the center.
            glm::vec3 force = glm::vec3(-6.f * object->scene->deltaTime, 0.0f, 0.0f);
            ApplyForceAboveCenter(force);
        }
        if (glfwGetKey(object->scene->window, GLFW_KEY_UP) == GLFW_PRESS) {
            // Apply a rightward force (positive X).
            glm::vec3 force = glm::vec3(6.f * object->scene->deltaTime, 0.0f, 0.0f);
            ApplyForceAboveCenter(force);
        }
        if (glfwGetKey(object->scene->window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
            // Apply a forward force (positive Z).
            glm::vec3 force = glm::vec3(0.0f, 0.0f, 6.f * object->scene->deltaTime);
            ApplyForceAboveCenter(force);
        }
        if (glfwGetKey(object->scene->window, GLFW_KEY_LEFT) == GLFW_PRESS) {
            // Apply a backward force (negative Z).
            glm::vec3 force = glm::vec3(0.0f, 0.0f, -6.f * object->scene->deltaTime);
            ApplyForceAboveCenter(force);
        }

        if (glfwGetKey(object->scene->window, GLFW_KEY_LEFT) == GLFW_PRESS) {
            // Apply a backward force (negative Z).
            glm::vec3 force = glm::vec3(0.0f, 0.0f, -6.f * object->scene->deltaTime);
            ApplyForceAboveCenter(force);
        }

        if (glfwGetKey(object->scene->window, GLFW_KEY_R) == GLFW_PRESS) {
            if (!randPressed)
            {
                softBody->RandomizeConstraintIterations();
                randPressed = true;
            }
        }
        else
        {
            randPressed = false;
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
  
            if (inCylynder)
                softBody->ApplyCylinderCollision(deltaTime, sbCollision, object->mesh->uniformScale, object->mesh->positionXYZ);
            else
            softBody->ApplyCollision(deltaTime, sbCollision, object->mesh->positionXYZ, object->mesh->uniformScale,inCylynder);
         

   
          
          //  glm::vec3 center = softBody->getGeometricCentrePoint() + object->mesh->positionXYZ;
            //std::cout << center.x << " " << center.y << " " << center.z << std::endl;
           
        
    }

    void UpdateSoftBodyMeshes(unsigned int shaderProgramID) {
      
            if (!softBody) return;

            // Update normals
            softBody->UpdateNormals();

            // Find mesh in VAO
            sModelDrawInfo softBodyDrawMeshLocalCopy;

            if (!object->scene->vaoManager->FindDrawInfoByModelName(SBMeshName, softBodyDrawMeshLocalCopy))
                return;
            //std::cout << softBodyDrawMeshLocalCopy.pVertices[1].nx << std::endl;
            // Update mesh vertex positions and normals
            for (unsigned int i = 0; i < softBodyDrawMeshLocalCopy.numberOfVertices; ++i) {
                softBodyDrawMeshLocalCopy.pVertices[i].x = softBody->vec_pParticles[i]->position.x;
                softBodyDrawMeshLocalCopy.pVertices[i].y = softBody->vec_pParticles[i]->position.y;
                softBodyDrawMeshLocalCopy.pVertices[i].z = softBody->vec_pParticles[i]->position.z;

                softBodyDrawMeshLocalCopy.pVertices[i].nx = softBody->m_ModelVertexInfo.pVertices[i].nx;
                softBodyDrawMeshLocalCopy.pVertices[i].ny = softBody->m_ModelVertexInfo.pVertices[i].ny;
                softBodyDrawMeshLocalCopy.pVertices[i].nz = softBody->m_ModelVertexInfo.pVertices[i].nz;
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
        force *= 0.1f;

        // Find the most top particle (the one with the highest y-position)
        cSoftBodyVerlet::sParticle* topmostParticle = nullptr;
        float maxY = -FLT_MAX;  // Initialize to a very low value to find the maximum

        for (cSoftBodyVerlet::sParticle* particle : softBody->vec_pParticles)
        {
            if (particle->position.y > maxY)
            {
                maxY = particle->position.y;
                topmostParticle = particle;
            }
        }

        if (topmostParticle)
        {
            // Calculate the distance from the center to the topmost particle
            glm::vec3 center = softBody->getGeometricCentrePoint();
            float distanceToTopmost = glm::length(topmostParticle->position - center);

            // Apply force to all particles based on their distance to the topmost particle
            for (cSoftBodyVerlet::sParticle* particle : softBody->vec_pParticles)
            {
                // Calculate the distance from this particle to the center
                float distanceToParticle = glm::length(particle->position - center);

                // Apply the force divided by the distance to the topmost particle
                float forceFactor = (distanceToTopmost != 0) ? (distanceToTopmost / distanceToParticle) : 1.0f;
                glm::vec3 forceApplied = force / forceFactor;

                // Apply the force depending on whether the particle is above or below the center
                if (particle->position.y > center.y)
                {
                    particle->position += forceApplied;
                }
                else
                {
                   // particle->position += forceApplied / 2.f;  // Apply less force for particles below the center
                }
            }
        }
    }



    void DebugDrawNormals(float scale = 1.0f)
    {
        for (unsigned int i = 0; i < softBody->m_ModelVertexInfo.numberOfVertices; ++i)
        {
            sVertex v =softBody->m_ModelVertexInfo.pVertices[i];
            glm::vec3 start(v.x, v.y, v.z);
            glm::vec3 end = start + glm::vec3(v.nx, v.ny, v.nz) * scale;

            // Use your debug drawing system here, e.g.:
           object->scene->DrawRay(start, end,object->scene->programs[0],glm::vec4(1),false,1);
        }
    }
};
