#pragma once
#include "Action.h"
#include "sObject.h"
#include <glm/vec3.hpp> 
#include "cBasicFlyCamera/cBasicFlyCamera.h"
#include "Scene.hpp"
#include <glm/glm.hpp>
#include "Gun.hpp"
//#include "EnemyBullet.hpp"

class aPlayer : public Action {
private:
    float walkSpeed = 30.0f;
    float runSpeed = 80.0f;
    float currentSpeed = walkSpeed;
    glm::vec3 up = glm::vec3(0, 1, 0); // Common up vector in 3D

    // Player collision radius
    float playerRadius = 1.0f;

    // Respawn position (center of the play area)
    glm::vec3 respawnPosition = glm::vec3(0.0f, 0.0f, 0.0f);

    // Helper function to calculate movement direction and rotation
    void CalculateMovementAndRotation(bool& shouldMove, glm::vec3& moveDirection, float& targetYaw) {
        bool w = glfwGetKey(object->scene->window, GLFW_KEY_UP) == GLFW_PRESS;
        bool s = glfwGetKey(object->scene->window, GLFW_KEY_DOWN) == GLFW_PRESS;
        bool a = glfwGetKey(object->scene->window, GLFW_KEY_LEFT) == GLFW_PRESS;
        bool d = glfwGetKey(object->scene->window, GLFW_KEY_RIGHT) == GLFW_PRESS;

        shouldMove = false;
        targetYaw = object->mesh->rotationEulerXYZ.y;

        // Handle primary directions with rotation
        if (w || s) {
            shouldMove = true;

            // Base rotation for forward/backward
            if (w) targetYaw = 0.0f; // Forward
            if (s) targetYaw = 180.0f; // Backward

            // Handle diagonal combinations
            if (d) targetYaw += (w ? -45.0f : 45.0f); // Forward + Right or Backward + Right
            if (a) targetYaw += (w ? 45.0f : -45.0f); // Forward + Left or Backward + Left
        }
        // Handle pure side movements
        else if (d) { targetYaw = -90.0f; shouldMove = true; } // Right
        else if (a) { targetYaw = 90.0f;  shouldMove = true; } // Left

        // Calculate forward direction based on final rotation
        float yawRad = glm::radians(targetYaw);
        moveDirection = glm::vec3(sin(yawRad), 0.0f, cos(yawRad));
    }

    // Helper function to check for sphere collisions
    bool CheckSphereCollision(const glm::vec3& pos1, float radius1, const glm::vec3& pos2, float radius2) {
        float distance = glm::length(pos1 - pos2);
        return distance < (radius1 + radius2);
    }

    // Helper function to handle player respawn
    void Respawn() {
        object->mesh->positionXYZ = respawnPosition; // Move player to respawn position
    }

public:
    void Start() override {
        // Initialization logic (if needed)
    }

    void Update() override {
        // Update camera position and rotation to follow the player
        object->scene->fCamera->setEyeLocation(
            object->mesh->positionXYZ.x - 50,
            object->mesh->positionXYZ.y + 200,
            object->mesh->positionXYZ.z - 50
        );
        object->scene->fCamera->setEyeRotation(
            object->mesh->rotationEulerXYZ.z + 90,
            object->mesh->rotationEulerXYZ.y + 90
        );

        // Speed adjustment
        if (glfwGetKey(object->scene->window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
            currentSpeed = runSpeed;
        }
        else {
            currentSpeed = walkSpeed;
        }

        // Calculate movement and rotation
        bool shouldMove = false;
        glm::vec3 moveDirection(0.0f);
        float targetYaw = 0.0f;
        CalculateMovementAndRotation(shouldMove, moveDirection, targetYaw);

        // Apply movement and rotation
        if (shouldMove) {
            // Calculate potential new position
            glm::vec3 movement = moveDirection * currentSpeed * object->scene->deltaTime;
            glm::vec3 newPosition = object->mesh->positionXYZ + movement;

            // Collision check with 1.5x safety margin
            std::vector<sCollision_RayTriangleInMesh> collisions;
            glm::vec3 rayEnd = object->mesh->positionXYZ + moveDirection * currentSpeed * 1.5f * object->scene->deltaTime;

            if (!object->scene->physicsManager->RayCast(
                object->mesh->positionXYZ,
                rayEnd,
                collisions,
                false))
            {
                object->mesh->positionXYZ = newPosition;
                object->mesh->rotationEulerXYZ.y = targetYaw; // Update rotation
            }
        }

        // Check for collisions with enemies and enemy bullets
        //for (auto obj : object->scene->sceneObjects) {
        //    if (obj->GetActionOfType<aEnemyAI>() != nullptr) { // Check if object is an enemy
        //        if (CheckSphereCollision(object->mesh->positionXYZ, playerRadius, obj->mesh->positionXYZ, 1.0f)) {
        //            // Destroy the enemy
        //            obj->Destroy();

        //            // Respawn the player at the center
        //            Respawn();
        //        }
        //    }

            // Check for collisions with enemy bullets (if applicable)
            //if (obj->GetActionOfType<aEnemyBullet>() != nullptr) { // Check if object is an enemy bullet
            //    if (CheckSphereCollision(object->mesh->positionXYZ, playerRadius, obj->mesh->positionXYZ, 0.5f)) {
            //        // Destroy the bullet
            //        obj->Destroy();

            //        // Respawn the player at the center
            //        Respawn();
            //    }
            //}
        //}
    }
};