#pragma once

#include "Action.h"
#include "sObject.h"
#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <Lua5.4.7/lua.hpp>
#include "cBasicFlyCamera/cBasicFlyCamera.h"
#include <iostream>
#include <glm/gtx/compatibility.hpp>
#include "Scene.hpp"
#include "cPhysics.h"
#include <glm/glm.hpp>

struct PhysicsData
{
    glm::vec3 position;
    glm::vec3 velocity = glm::vec3(0.0f);
    float mass = 1000.0f; // Default airplane mass in kg
};

class aPlanePhysics : public Action
{
public:
    cPhysics* physMan = nullptr;
    PhysicsData physData;
    glm::quat orientation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f); // Default orientation
    GLuint program;

    float maxSpeed = 200.0f;        // Maximum speed for the plane
    float liftCoefficient = 1.5f;  // Lift coefficient
    float dragCoefficient = 0.3f;  // Drag coefficient
    float wingArea = 50.0f;        // Wing area in m^2
    float thrust = 5000.0f;        // Engine thrust in N
    float sensitivity = 0.01f;     // Mouse sensitivity for yaw

    double prevMouseX = 0.0, prevMouseY = 0.0;

    void Start() override
    {
        physData.position = object->mesh->positionXYZ;
        glfwGetCursorPos(object->scene->window, &prevMouseX, &prevMouseY);
    }

    void Update() override
    {

        if (object->scene->isFlyCamera) return;

        // Handle inputs
        HandleInputs();

        // Apply 3DOF motion equations
        Apply3DOFMotion();

        // Update object mesh position and orientation
        object->mesh->positionXYZ = physData.position;
        object->mesh->orientation = orientation;

        // Debug: Log position and orientation
        std::cout << "Position: " << physData.position.x << ", " << physData.position.y << ", " << physData.position.z << std::endl;
        std::cout << "Orientation: " << glm::degrees(glm::eulerAngles(orientation)).x << ", "
            << glm::degrees(glm::eulerAngles(orientation)).y << ", "
            << glm::degrees(glm::eulerAngles(orientation)).z << std::endl;

        printf("Velocity X : ", physData.velocity.x);
        printf("Velocity Y : ", physData.velocity.y);
        printf("Velocity Z : ", physData.velocity.z);

    }

private:
    void HandleInputs()
    {
        float rotationSpeed = glm::radians(45.0f); // Default rotation speed (radians)
        float pitchUp = glm::radians(3.0f);
        float pitchDown = glm::radians(30.0f);

        if (glfwGetKey(object->scene->window, GLFW_KEY_W) == GLFW_PRESS)
        {
            Accelerate();
        }

        if (glfwGetKey(object->scene->window, GLFW_KEY_A) == GLFW_PRESS)
        {
            // Rotate left by 45 degrees (yaw)
            orientation = glm::rotate(orientation, -rotationSpeed, glm::vec3(0, 1, 0));
        }

        if (glfwGetKey(object->scene->window, GLFW_KEY_D) == GLFW_PRESS)
        {
            // Rotate right by 45 degrees (yaw)
            orientation = glm::rotate(orientation, rotationSpeed, glm::vec3(0, 1, 0));
        }

        if (glfwGetKey(object->scene->window, GLFW_KEY_S) == GLFW_PRESS)
        {
            // Rotate down by 30 degrees (pitch)
            orientation = glm::rotate(orientation, -pitchDown, glm::vec3(1, 0, 0));
        }

        if (glfwGetKey(object->scene->window, GLFW_KEY_SPACE) == GLFW_PRESS)
        {
            // Rotate up by 3 degrees (pitch)
            orientation = glm::rotate(orientation, pitchUp, glm::vec3(1, 0, 0));
        }

        // Mouse input for yaw
        double mouseX, mouseY;
        glfwGetCursorPos(object->scene->window, &mouseX, &mouseY);
        float mouseYaw = glm::radians((float)(mouseX - prevMouseX) * sensitivity);
        orientation = glm::rotate(orientation, mouseYaw, glm::vec3(0, 1, 0));

        prevMouseX = mouseX;
        prevMouseY = mouseY;
    }

    void Accelerate()
    {
        glm::vec3 forward = glm::normalize(orientation * glm::vec3(0, 0, -1));
        physData.velocity += forward * (thrust / physData.mass) * object->scene->deltaTime;

        // Clamp velocity to maxSpeed
        if (glm::length(physData.velocity) > maxSpeed)
        {
            physData.velocity = glm::normalize(physData.velocity) * maxSpeed;
        }
    }

    void Apply3DOFMotion()
    {
        float g = 9.81f; // Gravity
        glm::vec3 gravity(0.0f, -g, 0.0f);

        // Compute forces
        float L = ComputeLift();     // Lift
        float D = ComputeDrag();     // Drag
        glm::vec3 forward = glm::normalize(physData.velocity);
        glm::vec3 lift(0.0f, L, 0.0f);
        glm::vec3 drag = -D * forward;

        // Dynamics equations
        glm::vec3 totalForce = thrust * forward + lift + drag + physData.mass * gravity;
        glm::vec3 acceleration = totalForce / physData.mass;
        physData.velocity += acceleration * object->scene->deltaTime;

        // Kinematic equations
        physData.position += physData.velocity * object->scene->deltaTime;
    }

    float ComputeLift()
    {
        float rho = 1.225f; // Air density (kg/m^3)
        float q = 0.5f * rho * glm::length(physData.velocity) * glm::length(physData.velocity);
        return liftCoefficient * q * wingArea;
    }

    float ComputeDrag()
    {
        float rho = 1.225f; // Air density (kg/m^3)
        float q = 0.5f * rho * glm::length(physData.velocity) * glm::length(physData.velocity);
        return dragCoefficient * q * wingArea;
    }
};
