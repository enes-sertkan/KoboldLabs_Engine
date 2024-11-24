#pragma once

#include "Action.h"
#include <glm/vec3.hpp>
#include <vector>
#include "PhysicsManager.h"
#include "Scene.hpp"

class ClimbLadder : public Action {
public:
    float climbSpeed = 10.0f; 
    bool isClimbing = false; 
    int numberOfRays = 10; 
    float raySpacing = 1.5f;

    // Detect climbable object using raycasts
    bool DetectClimbable(Scene* scene, glm::vec3 position, glm::vec3 forward) {
        std::vector<sCollision_RayTriangleInMesh> collisions;

        // Cast multiple rays
        for (int i = 0; i < numberOfRays; ++i) {
            glm::vec3 offset = glm::vec3(0.0f, 0.0f, i * raySpacing - (numberOfRays * raySpacing) / 2.0f); 
            glm::vec3 rayStart = position + offset + forward * 0.5f; 
            glm::vec3 rayEnd = rayStart + forward * 2.0f; 

            if (scene->physicsManager->RayCast(rayStart, rayEnd, collisions, true)) {
                return true; 
            }
        }

        return false; 
    }

    void Update() override {
        glm::vec3 position = object->mesh->positionXYZ;
        glm::vec3 forward = object->scene->fCamera->getTargetRelativeToCamera();
        forward.y = 0; 
        forward = glm::normalize(forward);

        // Check for climbable objects using raycasts
        if (DetectClimbable(object->scene, position, forward)) {
            // Climbing logic
            if (glfwGetKey(object->scene->window, GLFW_KEY_W) == GLFW_PRESS) {
                object->mesh->positionXYZ.y += climbSpeed * object->scene->deltaTime; 
                isClimbing = true;
            }
            else if (glfwGetKey(object->scene->window, GLFW_KEY_S) == GLFW_PRESS) {
                object->mesh->positionXYZ.y -= climbSpeed * object->scene->deltaTime;
                isClimbing = true;
            }
            else {
                isClimbing = false; 
            }
        }
        else {
            isClimbing = false; 
        }
    }
};
