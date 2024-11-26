#pragma once

#include "Action.h"
#include <glm/vec3.hpp>
#include <vector>
#include "PhysicsManager.h"
#include "Scene.hpp"
#include <cstdlib> // For random number generation
#include <ctime>   // To seed the random number generator

class aRollDown : public Action {
public:
    float descentSpeed = 10.0f;  // Speed for descending a ladder
    glm::vec3 velocity = glm::vec3(0.0f); // Tracks current speed
    bool isDescending = false;  // Whether the barrel is currently descending
    bool canUseLadder = false;  // Whether this barrel can detect ladders
    bool usedFirstLadder = false; // Tracks if the first ladder was used
    float raySpacing = 2.0f; // Ray spacing for side collision detection
    bool isOnGround = false;  // Whether the barrel has landed on the ground
    bool isMovingDown = true;  // Direction flag for movement after hitting the ground

    aRollDown() {
        std::srand(static_cast<unsigned>(std::time(0))); // Seed the RNG

        // Assign 10% chance for this barrel to use ladders
        canUseLadder = (std::rand() % 100) < 10;
    }

    // Ladder detection using horizontal rays
    bool DetectLadder(Scene* scene, glm::vec3 position, glm::vec3& ladderPosition) {
        if (!canUseLadder) return false; // Skip detection for barrels without ladder ability

        std::vector<sCollision_RayTriangleInMesh> collisions;
        glm::vec4 gridRayColor = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f); // Blue for horizontal rays
        float rayScale = 0.1f;
        float rayLength = 10.0f;
        float lowerYOffset = -17.0f;

        // Horizontal grid ray detection
        glm::vec3 right = glm::vec3(0, 0, 1); // Direction along Z-axis
        glm::vec3 rayStart = position + glm::vec3(1.8f, lowerYOffset, 0.0f) - right * rayLength / 2.0f;
        glm::vec3 rayEnd = rayStart + right * rayLength;

        // Draw the ray for debugging
        scene->DrawRay(rayStart, rayEnd, scene->programs[0], gridRayColor, true, rayScale);

        // Perform raycast
        if (scene->physicsManager->RayCast(rayStart, rayEnd, collisions, true)) {
            if (!collisions.empty() && collisions[0].object && collisions[0].object->name == "ladder") {
                glm::vec3 rayDirection = glm::normalize(rayEnd - rayStart);
                ladderPosition = rayStart + rayDirection * static_cast<float>(collisions[0].timeOfCollision);
                return true; // Ladder detected
            }
        }
        return false; // No ladder detected
    }

    void Update() override {
        glm::vec3 position = object->mesh->positionXYZ;
        glm::vec3 ladderPosition;

        // Ladder detection and descent logic
        if (DetectLadder(object->scene, position, ladderPosition)) {
            if (!usedFirstLadder) {
                // Always descend the first ladder
                object->mesh->positionXYZ = glm::mix(position, ladderPosition, 0.1f);
                object->mesh->positionXYZ.y -= descentSpeed * object->scene->deltaTime;
                isDescending = true;
                usedFirstLadder = true;
            }
            else {
                // 50% chance to descend additional ladders
                if ((std::rand() % 100) < 50) {
                    object->mesh->positionXYZ = glm::mix(position, ladderPosition, 0.1f);
                    object->mesh->positionXYZ.y -= descentSpeed * object->scene->deltaTime;
                    isDescending = true;
                }
                else {
                    isDescending = false; // Skip this ladder
                }
            }
        }
        else {
            isDescending = false; // Not descending
        }

        // Disable physics (no gravity) while on the ladder
        if (isDescending) {
            velocity = glm::vec3(0.0f); // Stop moving in any direction other than Y
            // Only update Y-axis (vertical movement) for descent
            object->mesh->positionXYZ.y -= descentSpeed * object->scene->deltaTime;
        }
        else {
            // Ground collision handling when barrel is not descending
            glm::vec3 groundRayStart = position;
            glm::vec3 groundRayEnd = position + glm::vec3(0.0f, -1.0f, 0.0f);
            std::vector<sCollision_RayTriangleInMesh> groundCollisions;

            if (object->scene->physicsManager->RayCast(groundRayStart, groundRayEnd, groundCollisions, true)) {
                glm::vec3 rayDirection = glm::normalize(groundRayEnd - groundRayStart);
                glm::vec3 collisionPoint = groundRayStart + rayDirection * static_cast<float>(groundCollisions[0].timeOfCollision);

                object->mesh->positionXYZ.y = collisionPoint.y;
                velocity.y = 0.0f; // Stop falling after hitting the ground

                if (!isOnGround) {
                    // Reverse movement direction once the barrel hits the ground
                    isMovingDown = !isMovingDown;
                    isOnGround = true;
                }
            }
        }

        // After hitting the ground, the barrel will move in the opposite direction
        if (isOnGround) {
            if (isMovingDown) {
                velocity.y = -descentSpeed;  // Move downwards
            }
            else {
                velocity.y = descentSpeed;   // Move upwards (or stop moving)
            }
        }

        // Update barrel position with velocity
        object->mesh->positionXYZ += velocity * object->scene->deltaTime;
    }

    void DrawRays(Scene* scene, glm::vec3 position, glm::vec3 forward, float climbHeight, GLuint program) {
        glm::vec4 rayColor = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
        float rayScale = 0.1f;

        // Forward ray for ladder detection
        glm::vec3 rayStart = position + 0.5f;
        glm::vec3 rayEnd = rayStart + forward * 6.0f;
        //scene->DrawRay(rayStart, rayEnd, program, rayColor, true, rayScale);

        // Ground ray for collision detection
        rayStart = position;
        rayEnd = position + glm::vec3(0.0f, -3.3f, 0.0f) * raySpacing;
        //scene->DrawRay(rayStart, rayEnd, program, rayColor, true, rayScale);

        // Side rays for wall detection
        glm::vec3 sideRayEndLeft = position + glm::vec3(0.0f, 0.0f, -3.0f) * raySpacing;
        glm::vec3 sideRayEndRight = position + glm::vec3(0.0f, 0.0f, 3.0f) * raySpacing;
        scene->DrawRay(position, sideRayEndLeft, program, rayColor, true, rayScale);
        scene->DrawRay(position, sideRayEndRight, program, rayColor, true, rayScale);
    }
};
