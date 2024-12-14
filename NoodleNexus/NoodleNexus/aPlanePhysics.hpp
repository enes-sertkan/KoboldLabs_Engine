#pragma once

#include "Action.h";
#include "sObject.h"
#include <glm/vec3.hpp> 
#include <Lua5.4.7/lua.hpp>
#include "cBasicFlyCamera/cBasicFlyCamera.h"
#include <iostream>
#include "Scene.hpp"
#include "cPhysics.h"
#include <glm/glm.hpp>

struct PhysicsData
{
    glm::vec3 position;
    glm::vec3 acceleration = glm::vec3(0, 0, 0);
    glm::vec3 velocity = glm::vec3(0, 0, 0);
    bool isStatic = false;
};

class aPlanePhysics : public Action
{
public:
    cPhysics* physMan = nullptr;
    unsigned int numberOfNarrowPhaseTrianglesInAABB_BroadPhaseThing = 0;

    PhysicsData physData;
    float maxSpeed = 200.0f;  // Maximum speed for the plane
    float accelerationRate = 50.0f;  // Rate of acceleration
    float decelerationRate = 30.0f;  // Rate of deceleration
    bool isMoving = true;
    glm::vec3 up = glm::vec3(0, 1, 0);  // Common up vector in 3D
    GLuint program;

    void Start() override
    {
        physData.position = object->mesh->positionXYZ;
    }

    void Update() override
    {
        if (!physMan) return;

        if (object->scene->isFlyCamera && !glfwGetKey(object->scene->window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) return; //IF not fly camera and not pressing shoft, then return


        if (glfwGetKey(object->scene->window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
            maxSpeed = accelerationRate;
        else
            maxSpeed = decelerationRate;

        // Handle input for the plane
        if (glfwGetKey(object->scene->window, GLFW_KEY_W) == GLFW_PRESS)
        {
            Accelerate(FORWARD);
        }
        if (glfwGetKey(object->scene->window, GLFW_KEY_S) == GLFW_PRESS)
        {
            Accelerate(BACK);
        }
        if (glfwGetKey(object->scene->window, GLFW_KEY_A) == GLFW_PRESS)
        {
            Accelerate(LEFT);
        }
        if (glfwGetKey(object->scene->window, GLFW_KEY_D) == GLFW_PRESS)
        {
            Accelerate(RIGHT);
        }
        if (glfwGetKey(object->scene->window, GLFW_KEY_SPACE) == GLFW_PRESS)
        {
            Accelerate(UP);
        }
        if (glfwGetKey(object->scene->window, GLFW_KEY_BACKSPACE) == GLFW_PRESS)
        {
            Accelerate(DOWN);
        }
        else
            StopAcceleration();

        // Update velocity based on acceleration
        physData.velocity += physData.acceleration * object->scene->deltaTime;

        // Clamp velocity to maxSpeed
        if (glm::length(physData.velocity) > maxSpeed)
        {
            physData.velocity = glm::normalize(physData.velocity) * maxSpeed;
        }

        // Update position based on velocity
        physData.position += physData.velocity * object->scene->deltaTime;

        // Apply deceleration when no acceleration input is provided
        if (physData.acceleration == glm::vec3(0.0f))
        {
            physData.velocity -= physData.velocity * decelerationRate * object->scene->deltaTime;
            if (glm::length(physData.velocity) < 0.1f)
                physData.velocity = glm::vec3(0.0f);  // Stop completely when velocity is very small
        }

        // Update object mesh position
        object->mesh->positionXYZ = physData.position;

        // Debug: Output current velocity and position
        std::cout << "Position: " << physData.position.x << ", " << physData.position.y << ", " << physData.position.z << std::endl;
        std::cout << "Velocity: " << physData.velocity.x << ", " << physData.velocity.y << ", " << physData.velocity.z << std::endl;
    }


    enum Direction
    {
        FORWARD,
        BACK,
        LEFT,
        RIGHT,
        UP,
        DOWN,
        NONE
    };

    void Accelerate(Direction direction)
    {
        glm::vec3 forward = object->scene->fCamera->getTargetRelativeToCamera();
        forward.y = 0;

        forward = glm::normalize(forward);
        glm::vec3 left = glm::cross(up, forward);

        std::vector<sCollision_RayTriangleInMesh> collisions;

        // Debug ray visualization for directions (optional)
        // DrawRayS(position, position + forward * accelerationRate * 2.f, program);
        // DrawRayS(position, position - forward * accelerationRate * 2.f, program);
        // DrawRayS(position, position + left * accelerationRate * 2.f, program);
        // DrawRayS(position, position - left * accelerationRate * 2.f, program);

        switch (direction)
        {
        case aPlanePhysics::FORWARD:
            if (object->scene->physicsManager->RayCast(object->mesh->positionXYZ, object->mesh->positionXYZ + forward * accelerationRate * 1.5f * object->scene->deltaTime, collisions, false))
            {
                // Handle collision if needed
                break;
            }
            physData.acceleration = forward * accelerationRate;
            break;

        case aPlanePhysics::BACK:
            if (object->scene->physicsManager->RayCast(object->mesh->positionXYZ, object->mesh->positionXYZ - forward * accelerationRate * 1.5f * object->scene->deltaTime, collisions, false))
            {
                // Handle collision if needed
                break;
            }
            physData.acceleration = -forward * accelerationRate;
            break;

        case aPlanePhysics::LEFT:
            if (object->scene->physicsManager->RayCast(object->mesh->positionXYZ, object->mesh->positionXYZ + left * accelerationRate * 1.5f * object->scene->deltaTime, collisions, false))
            {
                // Handle collision if needed
                break;
            }
            physData.acceleration = left * accelerationRate;
            break;

        case aPlanePhysics::RIGHT:
            if (object->scene->physicsManager->RayCast(object->mesh->positionXYZ, object->mesh->positionXYZ - left * accelerationRate * 1.5f * object->scene->deltaTime, collisions, false))
            {
                // Handle collision if needed
                break;
            }
            physData.acceleration = -left * accelerationRate;
            break;

        case aPlanePhysics::UP:
            physData.acceleration = up * accelerationRate;
            break;

        case aPlanePhysics::DOWN:
            physData.acceleration = -up * accelerationRate;
            break;

        default:
            physData.acceleration = glm::vec3(0.0f);
            break;
        }
    }


    // Stop acceleration
    void StopAcceleration()
    {
        physData.acceleration = glm::vec3(0.0f);
    }
};
