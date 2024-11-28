#pragma once

#include "Action.h"
#include <glm/vec3.hpp>
#include <vector>
#include "PhysicsManager.h"
#include "Scene.hpp"
#include <string>

class Climb : public Action {
public:
    float climbSpeed = 3.0f;
    int forwardRays = 6;
    int verticalRays = 2;
    int horizontalRays = 3;
    float raySpacing = -8.0f;
    float rayHeight = 12.f;
    int rayAmmont = 4;
    float rayLength = 10.0f;
    bool isClimbing = false;
    aRayCastPhysics2D* physics = nullptr;


    bool DetectLadder(Scene* scene, glm::vec3 position) {


        std::vector<sCollision_RayTriangleInMesh> collisions;
        glm::vec4 gridRayColor = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f); // Blue for horizontal rays
        float rayScale = 0.1f;



        for (int i = 0; i < rayAmmont; i++)
        {
            // Horizontal grid ray detection
            glm::vec3 right = glm::vec3(0, 0, 1); // Direction along Z-axis
            glm::vec3 rayStart = position + glm::vec3(1.8f, rayHeight, 0.0f) - right * rayLength / 2.0f;
            glm::vec3 rayEnd = rayStart + right * rayLength;


            // Draw the ray for debugging
            scene->DrawRay(rayStart, rayEnd, scene->programs[0], gridRayColor, true, rayScale);

            // Perform raycast
            if (scene->physicsManager->RayCast(rayStart, rayEnd, collisions, false)) {
                return true;
            }

            position.y += raySpacing;
        }



        return false; // No ladder detected
    }


    bool DetectLadderDown(Scene* scene, glm::vec3 position) {


        std::vector<sCollision_RayTriangleInMesh> collisions;
        glm::vec4 gridRayColor = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f); // Blue for horizontal rays
        float rayScale = 0.1f;


            position.y += raySpacing* rayAmmont;

     
            glm::vec3 right = glm::vec3(0, 0, 1); // Direction along Z-axis
            glm::vec3 rayStart = position + glm::vec3(1.8f, rayHeight, 0.0f) - right * rayLength / 2.0f;
            glm::vec3 rayEnd = rayStart + right * rayLength;


            // Draw the ray for debugging
            scene->DrawRay(rayStart, rayEnd, scene->programs[0], gridRayColor, true, rayScale);

            // Perform raycast
            if (scene->physicsManager->RayCast(rayStart, rayEnd, collisions, false)) {
                return true;
            }

          
        



        return false; // No ladder detected
    }



    void Update() override {
        glm::vec3 position = object->mesh->positionXYZ;
        glm::vec3 forward = glm::vec3(1, 0, 0);
        forward.y = 0; // Restrict forward direction to horizontal plane
        forward = glm::normalize(forward);

        glm::vec3 up = glm::vec3(0.f, 1.f, 0.f);
      

        std::vector<sCollision_RayTriangleInMesh> collisions;



        float climbHeight = 2.0f; // Default climb height multiplier

        // Check for climbable objects using raycasts
        if (DetectLadder(object->scene, position)) {

        std::cout << "trying to climb" << std::endl;

            // Climbing logic
            if (glfwGetKey(object->scene->window, GLFW_KEY_W) == GLFW_PRESS) {
                object->mesh->positionXYZ.y += climbSpeed * object->scene->deltaTime;  // Move up
                isClimbing = true;

            }
            if (glfwGetKey(object->scene->window, GLFW_KEY_S) == GLFW_PRESS) {
                if (!object->scene->physicsManager->RayCast(position, position - up * climbSpeed, collisions, false) || DetectLadderDown(object->scene, position))
                {
                    object->mesh->positionXYZ.y -= climbSpeed * 2 * object->scene->deltaTime;  // Move down
                    isClimbing = true;
                }

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
