#pragma once

#include "Action.h"
#include "cVAOManager/cVAOManager.h"
#include "cSoftBodyVerlet.hpp"
#include <string>
#include "cSoftBodyCollisions.h"
#include "MazeGenerator.hpp"
#include "cVAOManager/cVAOManager.h"	// for: sModelDrawInfo	
#include <thread>
class Object;
typedef sVertex_SHADER_FORMAT_xyz_rgb_N_UV sVertex;
class SoftBody : public Action {
private:
   
public:

    cSoftBodyVerlet* m_PhysicsCopy; // Secondary buffer
    std::mutex m_PhysicsMutex;
    bool m_PhysicsUpdated = false;
    std::atomic<bool> m_KeepThreadAlive{ true };
    std::thread m_PhysicsThread;
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

    glm::vec3 initialVelocity = glm::vec3(0.0f);
    bool randPressed = false;
    SoftBodyCollision* sbCollision = new SoftBodyCollision();
    double lastUsedTime = 0.0;
    void CopyPhysicsState(cSoftBodyVerlet* src, cSoftBodyVerlet* dest) {
        // Copy particles
        for (size_t i = 0; i < src->vec_pParticles.size(); ++i) {
            dest->vec_pParticles[i]->position = src->vec_pParticles[i]->position;
            dest->vec_pParticles[i]->old_position = src->vec_pParticles[i]->old_position;
            // Copy other particle properties if needed
        }


        for (size_t i = 0; i < dest->m_InitialPositions.size(); ++i) {
            dest->m_InitialPositions[i] = src->m_InitialPositions[i];
        }


        for (size_t i = 0; i < dest->m_InitialOldPositions.size(); ++i) {
            dest->m_InitialOldPositions[i] = src->m_InitialOldPositions[i];
        }
        // Copy constraints
        for (size_t i = 0; i < src->vec_pConstraints.size(); ++i) {
            *dest->vec_pConstraints[i] = *src->vec_pConstraints[i];
        }

        // Copy other necessary state
        dest->m_geometricCentrePoint = src->m_geometricCentrePoint;
        dest->yToJump = src->yToJump;
        dest->acceleration = src->acceleration;
        dest->tightnessFactor = src->tightnessFactor;
        dest->useVolume = src->useVolume;
    }

    void SwapPhysicsState() {
        std::lock_guard<std::mutex> lock(m_PhysicsMutex);
        CopyPhysicsState(m_PhysicsCopy, softBody);  // Copy from physics buffer to main buffer
    }
    void SetMazeToSBCollision(MazeGenerator* mazeGenerator)
    {
        sbCollision->mazeGenerator = mazeGenerator;
    }

    void Start() override {
    
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
       
        softBody->SetInitialVelocity(initialVelocity);
        if (inCylynder)
        {
            sbCollision->cylinder = new Cylinder();
            sbCollision->cylinder->cylinderPos = object->mesh->positionXYZ;
            sbCollision->cylinder->cylinderPos.y -= 2.f;
            sbCollision->cylinder->cylinderRadius = cykinderRadious;
            sbCollision->cylinder->cylinderHeight = 2.5f;
        }
        m_PhysicsCopy = new cSoftBodyVerlet(*softBody);
        m_PhysicsThread = std::thread(&SoftBody::PhysicsUpdateLoop, this);
    }


    void Reset() {
        softBody->ResetToInitialState();
        if (m_PhysicsCopy) {
            m_PhysicsCopy->ResetToInitialState();
            m_PhysicsCopy->SetInitialVelocity(initialVelocity);
        }
    }

    // Move entire softbody while maintaining physics state
    void MoveTo(const glm::vec3& newWorldPosition) {
        glm::vec3 currentWorldPos = object->mesh->positionXYZ +
            softBody->getGeometricCentrePoint() *
            object->mesh->uniformScale;
        glm::vec3 delta = (newWorldPosition - currentWorldPos) /
            object->mesh->uniformScale;

        // Move both active and physics copy buffers
        softBody->MoveAllParticles(delta);
        if (m_PhysicsCopy) {
            m_PhysicsCopy->MoveAllParticles(delta);
        }

        // Update object's position to match new location
        object->mesh->positionXYZ = newWorldPosition -
            softBody->getGeometricCentrePoint() *
            object->mesh->uniformScale;
    }

    void MoveTopPart()
    {

    }

    void PhysicsUpdateLoop() {
        auto prevTime = std::chrono::high_resolution_clock::now();

        while (m_KeepThreadAlive) {
            // Calculate delta time properly
            auto now = std::chrono::high_resolution_clock::now();
            double deltaTime = std::chrono::duration<double>(now - prevTime).count();
            prevTime = now;

            // Cap delta time to prevent instability
            deltaTime = 0.1f;

            {
                std::lock_guard<std::mutex> lock(m_PhysicsMutex);
                UpdateSoftBody(deltaTime);  // Work on m_PhysicsCopy
                m_PhysicsUpdated = true;
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            bool should_swap = false;
            {
                std::lock_guard<std::mutex> lock(m_PhysicsMutex);
                should_swap = m_PhysicsUpdated;
                m_PhysicsUpdated = false;
            }

            if (should_swap) {
                SwapPhysicsState();  // This now copies data instead of swapping pointers
            }
        }
    }

    void Update() override {



        // Normal rendering using softBody pointer
        UpdateSoftBodyMeshes(object->scene->programs[0]);
   
        // Update any other soft body logic here, e.g. Verlet integration,
        // constraint satisfaction, collisions, etc.
      //  UpdateSoftBody(object->scene->deltaTime);

      

      //  DebugDrawNormals();

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
   //     std::lock_guard<std::mutex> lock(m_PhysicsMutex);

          //  if (softBody!=nullptr) return;

            // Apply Verlet integration steps
        m_PhysicsCopy->VerletUpdate(deltaTime);
        m_PhysicsCopy->SatisfyConstraints();
  
            if (inCylynder)
                m_PhysicsCopy->ApplyCylinderCollision(deltaTime, sbCollision, object->mesh->uniformScale, object->mesh->positionXYZ);
            else
                m_PhysicsCopy->ApplyCollision(deltaTime, sbCollision, object->mesh->positionXYZ, object->mesh->uniformScale,inCylynder);
         

   
          
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

        for (cSoftBodyVerlet::sParticle* particle : m_PhysicsCopy->vec_pParticles)
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
            glm::vec3 center = m_PhysicsCopy->getGeometricCentrePoint();
            float distanceToTopmost = glm::length(topmostParticle->position - center);

            // Apply force to all particles based on their distance to the topmost particle
            for (cSoftBodyVerlet::sParticle* particle : m_PhysicsCopy->vec_pParticles)
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
