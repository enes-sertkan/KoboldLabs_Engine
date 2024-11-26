#pragma once

#include "Action.h"
#include <glm/vec3.hpp>
#include <vector>
#include "PhysicsManager.h"
#include "Scene.hpp"
#include <string>

class Climb : public Action {
public:
    float climbSpeed = 10.0f;
    int forwardRays = 6;
    int verticalRays = 2;
    int horizontalRays = 5;
    float raySpacing = 2.0f;

    bool isClimbing = false;
    aRayCastPhysics2D* physics = nullptr;

    void DrawRays(Scene* scene, glm::vec3 position, glm::vec3 forward, float climbHeight, GLuint program) {
        glm::vec4 rayColor = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f); // Red for debug rays
        float rayScale = 0.1f; // Sphere size for rays

        // Increase the forward offset and climb height for the rays
        float forwardOffset = 3.0f; // Move the rays more in front of the player
        float rayLength = climbHeight * 10.0f; // Make the rays longer

        // Forward rays
        for (int i = 0; i < forwardRays; ++i) {
            glm::vec3 offset = glm::vec3(0.0f, i * raySpacing, 0.0f);
            glm::vec3 rayStart = position + offset + forward * forwardOffset;  // Start the rays further ahead
            glm::vec3 rayEnd = rayStart + forward * rayLength;  // Make the rays longer

            scene->DrawRay(rayStart, rayEnd, program, rayColor, true, rayScale);
        }

        // Vertical and horizontal rays (grid)
        for (int i = 0; i < verticalRays; ++i) {
            for (int j = 0; j < horizontalRays; ++j) {
                glm::vec3 offset = glm::vec3(j * raySpacing - (horizontalRays * raySpacing) / 2.0f,
                    i * raySpacing,
                    0.0f);
                glm::vec3 rayStart = position + offset + forward * forwardOffset;  // Start the rays further ahead
                glm::vec3 rayEnd = rayStart + forward * rayLength;  // Make the rays longer

                scene->DrawRay(rayStart, rayEnd, program, rayColor, true, rayScale);
            }
        }
    }

    bool DetectClimbable(Scene* scene, glm::vec3 position, glm::vec3 forward, float climbHeight) {
        std::vector<sCollision_RayTriangleInMesh> collisions;

        // Color for forward rays (Red)
        glm::vec4 forwardRayColor = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
        // Color for vertical and horizontal rays (Blue)
        glm::vec4 gridRayColor = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
        float rayScale = 0.1f;

        // Cast forward rays to check for climbable surfaces
        for (int i = 0; i < forwardRays; ++i) {
            glm::vec3 offset = glm::vec3(0.0f, i * raySpacing, 0.0f);
            glm::vec3 rayStart = position + offset + forward * 0.5f;
            rayStart.y -= 15.0f;
            glm::vec3 rayEnd = rayStart + forward * climbHeight * 2.0f;

            // Draw forward rays
            scene->DrawRay(rayStart, rayEnd, scene->programs[0], forwardRayColor, true, rayScale);

            // Perform raycast for forward rays
            if (scene->physicsManager->RayCast(rayStart, rayEnd, collisions, true)) {
                return true; 
            }
        }

        /* Draw vertical and horizontal rays in a grid pattern*/
        float forwardOffset = 2.0f;
        float rayLength = 10.0f;
        glm::vec3 right = glm::vec3(0, 0, 1);

        glm::vec3 rayStart = position + forward * forwardOffset - right * rayLength / 2.0f;
        glm::vec3 rayEnd = rayStart + right * rayLength;

        scene->DrawRay(rayStart, rayEnd, scene->programs[0], gridRayColor, true, rayScale);

        if (scene->physicsManager->RayCast(rayStart, rayEnd, collisions, true)) {
            return true;  
        }


        return false;  // No climbable surfaces detected
    }


    void Update() override {
        glm::vec3 position = object->mesh->positionXYZ;
        glm::vec3 forward = glm::vec3(1, 0, 0);
        forward.y = 0; // Restrict forward direction to horizontal plane
        forward = glm::normalize(forward);

        float climbHeight = 2.0f; // Default climb height multiplier

        // Check for climbable objects using raycasts
        if (DetectClimbable(object->scene, position, forward, climbHeight)) {

        std::cout << "trying to climb" << std::endl;

            // Climbing logic
            if (glfwGetKey(object->scene->window, GLFW_KEY_W) == GLFW_PRESS) {
                object->mesh->positionXYZ.y += climbSpeed * object->scene->deltaTime;  // Move up
                isClimbing = true;

            }
            else if (glfwGetKey(object->scene->window, GLFW_KEY_S) == GLFW_PRESS) {
                object->mesh->positionXYZ.y -= climbSpeed * 2 * object->scene->deltaTime;  // Move down
                isClimbing = true;

            }
            else {
                //isClimbing = false;  // Stop climbing if no key is pressed
                
            }
        }
        else {
            if (isClimbing)
            {
                isClimbing = false;  // Not climbing if no climbable surface detected
                physics->speed = glm::vec3(0.0f, 0.0f, 0.0f);

            }




        }

        if (isClimbing) 
        {
            object->mesh->positionXYZ -= physics->speed + physics->gravityAcceleration;
            
        }

    }

};
