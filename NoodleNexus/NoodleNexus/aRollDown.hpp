#pragma once

#include "Action.h"
#include <glm/vec3.hpp>
#include <vector>
#include "PhysicsManager.h"
#include "Scene.hpp"
#include <string>
#include "aRayCastPhysics2D.hpp"
#include <cstdlib> // For random number generation
#include <ctime>   // To seed the random number generator

class aRollDown : public Action {
public:
    // Barrel-specific variables
    float climbSpeed = 10.0f;
    float fallSpeed = -9.8f;
    glm::vec3 velocity = glm::vec3(0.0f);  // Tracks current speed
    float raySpacing = 2.0f;

    bool isClimbing = false;
    aRayCastPhysics2D* physics = nullptr;

    aRollDown() {
        // Seed the random number generator
        /*std::srand(static_cast<unsigned>(std::time(0)));*/
    }

    // Ladder detection and interaction
    bool DetectLadder(Scene* scene, glm::vec3 position, glm::vec3 forward, glm::vec3& ladderPosition) {
        std::vector<sCollision_RayTriangleInMesh> collisions;

        // Ray for ladder detection
        glm::vec3 rayStart = position;
        glm::vec3 rayEnd = rayStart + forward * 5.0f;

        // Perform raycast for ladders
        if (scene->physicsManager->RayCast(rayStart, rayEnd, collisions, true)) {
            // Calculate the collision point using timeOfCollision
            glm::vec3 rayDirection = glm::normalize(rayEnd - rayStart);
            ladderPosition = rayStart + rayDirection * static_cast<float>(collisions[0].timeOfCollision);

            return true;
        }
        return false;
    }

    // Barrel update logic
    void Update() override {
        glm::vec3 position = object->mesh->positionXYZ;
        glm::vec3 forward = glm::vec3(1, 0, 0);
        forward = glm::normalize(forward);

        glm::vec3 ladderPosition;

        // Gravity logic
        if (!isClimbing) {
            velocity.y += fallSpeed * object->scene->deltaTime;  // Apply gravity
        }

        // Ladder detection with 20% chance to descend
        if (DetectLadder(object->scene, position, forward, ladderPosition)) {
            std::cout << "Ladder detected!" << std::endl;

            if ((std::rand() % 100) < 20) { // 20% chance
                // Snap to ladder and begin descent
                object->mesh->positionXYZ = glm::mix(position, ladderPosition, 0.1f); // Smooth snapping
                velocity = glm::vec3(0.0f);  // Stop other movements
                isClimbing = true;

                // Move downward
                object->mesh->positionXYZ.y -= climbSpeed * object->scene->deltaTime;
            }
            else {
                isClimbing = false; // Ignore ladder
            }
        }
        else {
            isClimbing = false;  // No ladder detected
        }

        // Update barrel position with velocity
        object->mesh->positionXYZ += velocity * object->scene->deltaTime;

        glm::vec3 groundRayStart = position;
        glm::vec3 groundRayEnd = position + glm::vec3(0.0f, -1.0f, 0.0f) * raySpacing;

        std::vector<sCollision_RayTriangleInMesh> collisions;
        if (object->scene->physicsManager->RayCast(groundRayStart, groundRayEnd, collisions, true)) {
            // Calculate the collision point using the ray and timeOfCollision
            glm::vec3 rayDirection = glm::normalize(groundRayEnd - groundRayStart);
            glm::vec3 collisionPoint = groundRayStart + rayDirection * static_cast<float>(collisions[0].timeOfCollision);

            // Snap the barrel's position to the collision point
            object->mesh->positionXYZ.y = collisionPoint.y;
            velocity.y = 0.0f;  // Stop falling
        }

        // Debug information
        DrawRays(object->scene, position, forward, 2.0f, object->scene->programs[0]);
    }

    // Debugging function: Draw debug rays
    void DrawRays(Scene* scene, glm::vec3 position, glm::vec3 forward, float climbHeight, GLuint program) {
        glm::vec4 rayColor = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);  // Red for debug rays
        float rayScale = 0.1f; // Sphere size for rays

        // Forward ray for ladder detection
        glm::vec3 rayStart = position;
        glm::vec3 rayEnd = rayStart + forward * 5.0f;
        scene->DrawRay(rayStart, rayEnd, program, rayColor, true, rayScale);

        // Ground ray for collision detection
        rayStart = position;
        rayEnd = position + glm::vec3(0.0f, -1.0f, 0.0f) * raySpacing;
        scene->DrawRay(rayStart, rayEnd, program, rayColor, true, rayScale);
    }
};
