#pragma once
#include "Action.h"
#include "sObject.h"
#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <iostream>
#include "Scene.hpp"
#include "aExplosion.hpp"
#include "cPhysics.h"
#include "PhysicsManager.h"

struct PhysicsDataMovement {
    glm::vec3 position = glm::vec3(-43, 9, 102);
    glm::vec3 velocity = glm::vec3(0.0f);
    float mass = 1000.0f;
};

class aPlaneMovement : public Action {
public:
    
    glm::quat orientation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
    float maxSpeed = 100.0f;
    float acceleration = 15.0f;
    float dragCoefficient = 0.001f;
    cPhysics::cBroad_Cube* pTheAABB_Cube = nullptr;
    cPhysics* phys  = nullptr;

    // Constants for rotational limits
    const float maxPitchUpAngle = 25.0f;  // Maximum pitch up in degrees
    const float maxPitchDownAngle = -15.0f;  // Maximum pitch down in degrees
    const float maxRollAngle = 60.0f;  // Maximum roll angle in degrees

    float fixedDeltaTime = 1.0f / 60.0f; // Simulation step (1/60th of a second)
    float accumulator = 0.0f;
    bool isGrav = false;
    const glm::vec3 gravity = glm::vec3(0.0f, -9.8f, 0.0f);

    glm::vec3 prevPosition;
    int prevPosTimer = 0;

    // Speed constants
    const float sideMoveSpeed = 15.0f;  // Speed for lateral movement (adjust as needed)

    PhysicsDataMovement* physData = new PhysicsDataMovement();
    GLuint program;

    void Start() override {
        physData->position = glm::vec3(-43, 9, 102); // Ensure position is initialized
        physData->velocity = glm::vec3(0.0f, 0.0f, 0.0f); // Ensure velocity is initialized
    }

    void Update() override {
        float deltaTime = object->scene->deltaTime;
        accumulator += deltaTime;  // Accumulate delta time

        if (prevPosTimer > 3)
        {
            prevPosition = object->mesh->positionXYZ;
            prevPosTimer = 0;
        }
        else
        {
            prevPosTimer++;

        }
        HandleInputs();  // Process input actions
        ApplyPhysics();  // Update physics
        UpdateObjectTransform();  // Update object state
        // Apply physics updates in fixed time steps
        while (accumulator >= fixedDeltaTime) {
      
       

            accumulator -= fixedDeltaTime;  // Reduce accumulated time by fixedDeltaTime
        }
    }
    void HitSmt(glm::vec3 pos)
    {
        physData->velocity = 0.f * physData->velocity;
        Object* explosionSphere = object->scene->GenerateMeshObjectsFromObject("assets/models/Sphere_radius_1_xyz_N_uv.ply", pos, 10, glm::vec3(0), false, glm::vec4(1), true, object->sceneObjects);
        explosionSphere->mesh->textures[0] = "yellow.bmp";

        aExplosion* explodeOnImpact = new aExplosion();
        explodeOnImpact->expansionRate = 5.0f;
        explodeOnImpact->maxScale = 100.0f;
        object->scene->AddActionToObj(explodeOnImpact, explosionSphere);

        explodeOnImpact->objectToStick = object;
        explodeOnImpact->Start();

        physData->position = prevPosition;
        prevPosTimer = 0;
    }
private:
    void HandleInputs()
    {


        // Require LEFT_SHIFT to be pressed for any other input to work
        if (glfwGetKey(object->scene->window, GLFW_KEY_LEFT_SHIFT) != GLFW_PRESS)
        {
            return; // Do nothing if SHIFT is not pressed
        }

        // Forward movement (W key) along X axis
        if (glfwGetKey(object->scene->window, GLFW_KEY_W) == GLFW_PRESS)
        {

            glm::vec3 start = object->mesh->positionXYZ;
            glm::vec3 end = start + glm::vec3(35, 0, 0);
            std::vector <cPhysics::sCollision_RayTriangleInMesh> triangeMesh;
            if (!pTheAABB_Cube)
            {
                physData->velocity += glm::vec3(acceleration * fixedDeltaTime, 0.0f, 0.0f); // Move forward on X axis
            }
            else
            if (!pTheAABB_Cube->vec_pTriangles.empty())
            if (!phys->rayCastCustom(start, end, pTheAABB_Cube->vec_pTriangles, triangeMesh, false))
            {
                physData->velocity += glm::vec3(acceleration * fixedDeltaTime, 0.0f, 0.0f); // Move forward on X axis

            }
        }

        // Backward movement (S key) along X axis
        if (glfwGetKey(object->scene->window, GLFW_KEY_S) == GLFW_PRESS)
        {
            physData->velocity += glm::vec3(-acceleration * fixedDeltaTime, 0.0f, 0.0f); // Move backward on X axis
        }

        // Downward movement (X key) along Y axis (faster than other movements)
        if (glfwGetKey(object->scene->window, GLFW_KEY_X) == GLFW_PRESS)
        {
            physData->velocity += glm::vec3(0.0f, -2.0f * acceleration * fixedDeltaTime, 0.0f); // Move down on Y axis
        }

        // Upward movement (SPACE key) along Y axis (faster than other movements)
        if (glfwGetKey(object->scene->window, GLFW_KEY_SPACE) == GLFW_PRESS)
        {
            physData->velocity += glm::vec3(0.0f, 2.0f * acceleration * fixedDeltaTime, 0.0f); // Move up on Y axis
        }

        // Right movement (D key) along Z axis
        if (glfwGetKey(object->scene->window, GLFW_KEY_D) == GLFW_PRESS)
        {
            physData->velocity += glm::vec3(0.0f, 0.0f, acceleration * fixedDeltaTime); // Move right on Z axis
        }

        // Left movement (A key) along Z axis
        if (glfwGetKey(object->scene->window, GLFW_KEY_A) == GLFW_PRESS)
        {
            physData->velocity += glm::vec3(0.0f, 0.0f, -acceleration * fixedDeltaTime); // Move left on Z axis
        }

        if (glfwGetKey(object->scene->window, GLFW_KEY_R) == GLFW_PRESS)
        {
            isGrav = true;
        }
        
        if (glfwGetKey(object->scene->window, GLFW_KEY_F) == GLFW_PRESS)
        {
            isGrav = false;
        }

        // Deceleration (if no keys are pressed)
        if (glfwGetKey(object->scene->window, GLFW_KEY_W) == GLFW_RELEASE &&
            glfwGetKey(object->scene->window, GLFW_KEY_A) == GLFW_RELEASE &&
            glfwGetKey(object->scene->window, GLFW_KEY_D) == GLFW_RELEASE &&
            glfwGetKey(object->scene->window, GLFW_KEY_S) == GLFW_RELEASE &&
            glfwGetKey(object->scene->window, GLFW_KEY_SPACE) == GLFW_RELEASE &&
            glfwGetKey(object->scene->window, GLFW_KEY_X) == GLFW_RELEASE)
        {
            Deaccelerate();
        }



        if( isGrav)
        // Apply gravity: constantly pull down along the Y-axis
        physData->velocity += gravity * object->scene->deltaTime;

        // Clamp speed
        physData->velocity = ClampSpeed();
    }




    void Accelerate(const glm::vec3& direction) {
        glm::vec3 forward = RotateVectorWithQuaternion(orientation, direction);
        physData->velocity += forward * acceleration * fixedDeltaTime;
        ClampSpeed();
    }

    void Deaccelerate()
    {
        float decelerationFactor = 0.98f;  // A value slightly less than 1 for gradual slowing
        physData->velocity *= decelerationFactor;  // Reduce velocity
        if (glm::length(physData->velocity) < 0.1f) // Stop if velocity is very small
        {
            physData->velocity = glm::vec3(0.0f);
        }
    }


    void Rotate(float angle, const glm::vec3& axis) {
        orientation = glm::rotate(orientation, angle, axis);
    }

    void ApplyPhysics() {
        ApplyDrag();
        physData->position += physData->velocity * fixedDeltaTime;
    }

    void ApplyDrag() {
        physData->velocity *= 1.0f - (dragCoefficient * fixedDeltaTime);
    }

    glm::vec3 ClampSpeed()
    {
        // Check if the velocity exceeds the maximum speed
        float currentSpeed = glm::length(physData->velocity);
        if (currentSpeed > maxSpeed)
        {
            // Normalize the velocity and scale it to maxSpeed
            return glm::normalize(physData->velocity) * maxSpeed;
        }

        // If within limits, return the original velocity
        return physData->velocity;
    }

    void UpdateObjectTransform() {
        object->mesh->positionXYZ = physData->position;
        object->mesh->rotationEulerXYZ = glm::degrees(glm::eulerAngles(orientation));
    }

    glm::vec3 RotateVectorWithQuaternion(const glm::quat& quaternion, const glm::vec3& vector) {
        return glm::mat3_cast(quaternion) * vector;
    }

    bool IsKeyPressed(int key) {
        return glfwGetKey(object->scene->window, key) == GLFW_PRESS;
    }


    
};










































//#pragma once
//#define NOMINMAX
//#include "Action.h"
//#include "sObject.h"
//#include <glm/vec3.hpp>
//#include <glm/gtc/quaternion.hpp>
//#include <glm/gtx/euler_angles.hpp>
//#include <glm/gtx/rotate_vector.hpp>
//#include <Lua5.4.7/lua.hpp>
//#include "cBasicFlyCamera/cBasicFlyCamera.h"
//#include <iostream>
//#include "Scene.hpp"
//#include "cPhysics.h"
//#include <glm/glm.hpp>
//#include <algorithm> // For std::min and std::max
//
//struct PhysicsData
//{
//    glm::vec3 position = glm::vec3(0.0f);
//    glm::vec3 velocity = glm::vec3(0.0f);
//    float mass = 1000.0f; // Default airplane mass in kg
//};
//
//class aPlaneMovement : public Action
//{
//public:
//    cPhysics* physMan = nullptr;
//    PhysicsData* physData = new PhysicsData();
//    glm::quat orientation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f); // Default orientation
//    GLuint program;
//
//    float maxSpeed = 50.0f;        // Maximum speed for the plane
//    float acceleration = 5.0f;
//    float deacceleration = 5.0f;
//    float sideMoveSpeed = 10.0f;
//    float liftCoefficient = 1.5f;  // Lift coefficient
//    float dragCoefficient = 0.3f;  // Drag coefficient
//    float wingArea = 50.0f;        // Wing area in m^2
//    float thrust = 5000.0f;        // Engine thrust in N
//
//    float maxRollAngle = glm::radians(60.0f); // Max roll rotation in X-axis for A/D keys
//    float maxPitchUpAngle = glm::radians(25.0f); // Max pitch rotation for SPACE (up)
//    float maxPitchDownAngle = glm::radians(-15.0f); // Max pitch rotation for S (down)
//
//    void Start() override
//    {
//        physData->position = glm::vec3(0.0f, 0.0f, 0.0f); // Ensure position is initialized
//        physData->velocity = glm::vec3(0.0f, 0.0f, 0.0f); // Ensure velocity is initialized
//    }
//
//    void Update() override
//    {
//        if (glfwGetKey(object->scene->window, GLFW_KEY_LEFT_SHIFT) != GLFW_PRESS)
//        {
//            return; // Do nothing if SHIFT is not pressed
//        }
//
//        // Handle inputs
//        HandleInputs();
//
//        // Apply forces and update motion (3DOF)
//        ApplyForces();
//
//        // Rotate the forward vector using the orientation quaternion
//        glm::vec3 forward = RotateVectorWithQuaternion(orientation, glm::vec3(0, 0, -1)); // Get rotated forward direction
//
//        // Update the velocity and movement (you can apply this in other logic as needed)
//        physData->velocity += forward * acceleration * object->scene->deltaTime;
//
//        // Update object mesh position and orientation
//        object->mesh->positionXYZ = physData->position;
//        object->mesh->rotationEulerXYZ = glm::degrees(glm::eulerAngles(orientation));
//
//        // Debugging output
//        std::cout << "Position: " << physData->position.x << ", " << physData->position.y << ", " << physData->position.z << std::endl;
//        std::cout << "Orientation: " << glm::degrees(glm::eulerAngles(orientation)).x << ", "
//            << glm::degrees(glm::eulerAngles(orientation)).y << ", "
//            << glm::degrees(glm::eulerAngles(orientation)).z << std::endl;
//    }
//
//private:
//    void HandleInputs()
//    {
//        float rotationSpeed = glm::radians(5.0f); // Rotation speed for yaw
//        float pitchUp = glm::radians(3.0f); // Pitch up for SPACE
//        float pitchSide = glm::radians(2.0f); // Side rotation for A/D
//
//        // Move forward (W)
//        if (glfwGetKey(object->scene->window, GLFW_KEY_W) == GLFW_PRESS)
//        {
//            AccelerateForward();
//        }
//
//        // Move left and rotate left (A)
//        if (glfwGetKey(object->scene->window, GLFW_KEY_A) == GLFW_PRESS)
//        {
//            // Rotate left (yaw)
//            orientation = glm::rotate(orientation, rotationSpeed, glm::vec3(0, 1, 0));
//
//            // Move left (side vector)
//            glm::vec3 left = glm::normalize(RotateVectorWithQuaternion(orientation, glm::vec3(-1, 0, 0))); // Rotate left vector
//            physData->velocity += left * sideMoveSpeed * object->scene->deltaTime;
//
//            // Slight roll left (rotate on x-axis)
//            if (pitchSide < maxRollAngle) {
//                orientation = glm::rotate(orientation, pitchSide, glm::vec3(1, 0, 0)); // Apply pitch side without exceeding max roll angle
//            }
//            else {
//                orientation = glm::rotate(orientation, maxRollAngle, glm::vec3(1, 0, 0)); // Apply max roll angle if exceeded
//            }
//        }
//
//        // Move right and rotate right (D)
//        if (glfwGetKey(object->scene->window, GLFW_KEY_D) == GLFW_PRESS)
//        {
//            // Rotate right (yaw)
//            orientation = glm::rotate(orientation, -rotationSpeed, glm::vec3(0, 1, 0));
//
//            // Move right (side vector)
//            glm::vec3 right = glm::normalize(RotateVectorWithQuaternion(orientation, glm::vec3(1, 0, 0))); // Rotate right vector
//            physData->velocity += right * sideMoveSpeed * object->scene->deltaTime;
//
//            // Slight roll right (rotate on x-axis)
//            if (-pitchSide > -maxRollAngle) {
//                orientation = glm::rotate(orientation, -pitchSide, glm::vec3(1, 0, 0)); // Apply negative pitch side without exceeding negative max roll angle
//            }
//            else {
//                orientation = glm::rotate(orientation, -maxRollAngle, glm::vec3(1, 0, 0)); // Apply negative max roll angle if exceeded
//            }
//        }
//
//        // Move up (SPACE)
//        if (glfwGetKey(object->scene->window, GLFW_KEY_SPACE) == GLFW_PRESS)
//        {
//            // Upward acceleration (lift)
//            physData->velocity += glm::vec3(0, 1, 0) * acceleration * object->scene->deltaTime; // Modify only y
//
//            // Slight pitch upwards (rotate on z-axis)
//            float pitchAngleZ = glm::eulerAngles(orientation).z;
//            if (pitchAngleZ < maxPitchUpAngle)
//            {
//                // Rotate up on the z-axis, making sure we don't exceed the max pitch angle
//                orientation = glm::rotate(orientation, pitchUp, glm::vec3(0, 0, 1));
//            }
//        }
//
//        // Move down (S)
//        if (glfwGetKey(object->scene->window, GLFW_KEY_S) == GLFW_PRESS)
//        {
//            // Downward acceleration (decelerate or reverse thrust)
//            physData->velocity += glm::vec3(0, -1, 0) * acceleration * object->scene->deltaTime; // Modify only y
//
//            // Slight pitch downwards (rotate on z-axis)
//            float pitchAngleZ = glm::eulerAngles(orientation).z;
//            if (pitchAngleZ > maxPitchDownAngle)
//            {
//                // Rotate down on the z-axis, ensuring we don't exceed the max pitch down angle
//                orientation = glm::rotate(orientation, -pitchUp, glm::vec3(0, 0, 1));
//            }
//        }
//
//        // Decelerate in all directions if no input is given
//        if (glfwGetKey(object->scene->window, GLFW_KEY_W) == GLFW_RELEASE)
//        {
//            Deaccelerate();
//        }
//
//        // Clamp velocity to maxSpeed
//        physData->velocity = ClampSpeed();
//    }
//
//
//
//    void AccelerateForward()
//    {
//        glm::vec3 forward = RotateVectorWithQuaternion(orientation, glm::vec3(0, 0, -1)); // Get forward direction
//
//        // Ensure forward vector is normalized
//        if (glm::length(forward) > 0.0f)
//            forward = glm::normalize(forward);
//        else
//            forward = glm::vec3(0.0f);
//
//        // Update velocity: Accelerate forward
//        physData->velocity += forward * acceleration * object->scene->deltaTime;
//
//        // Debugging output
//        std::cout << "Accelerating: " << physData->velocity.x << ", "
//            << physData->velocity.y << ", "
//            << physData->velocity.z << std::endl;
//    }
//
//    void Deaccelerate()
//    {
//        // Apply negative acceleration for deceleration (friction-like effect)
//        physData->velocity -= physData->velocity * deacceleration * object->scene->deltaTime;
//    }
//
//    void ApplyForces()
//    {
//        float g = 9.81f; // Gravity
//        glm::vec3 gravity(0.0f, -g, 0.0f);
//
//        // Compute forces
//        float L = ComputeLift();     // Lift
//        float D = ComputeDrag();     // Drag
//        glm::vec3 forward = glm::length(physData->velocity) > 0.0f ? glm::normalize(physData->velocity) : glm::vec3(0.0f, 0.0f, 0.0f);
//        glm::vec3 lift(0.0f, L, 0.0f);
//        glm::vec3 drag = glm::vec3(0); //* -D* forward;
//
//        // Dynamics equations
//        glm::vec3 totalForce = drag + physData->mass * gravity;
//        glm::vec3 acceleration = totalForce / physData->mass;
//        physData->velocity += acceleration * object->scene->deltaTime;
//
//        // Kinematic equations
//        physData->position += physData->velocity * object->scene->deltaTime;
//
//        std::cout << "Total Force : " << totalForce.x << ", " <<  totalForce.y << ", " << totalForce.z << std::endl;
//
//    }
//
//    float ComputeLift()
//    {
//        if (glm::length(physData->velocity) <= 0.0f) return 0.0f; // Avoid invalid values
//        float rho = 1.225f; // Air density (kg/m^3)
//        float q = 0.5f * rho * glm::length(physData->velocity) * glm::length(physData->velocity);
//        return liftCoefficient * q * wingArea;
//    }
//
//    float ComputeDrag()
//    {
//        if (glm::length(physData->velocity) <= 0.0f) return 0.0f; // Avoid invalid values
//        float rho = 1.225f; // Air density (kg/m^3)
//        float q = 0.5f * rho * glm::length(physData->velocity) * glm::length(physData->velocity);
//        return dragCoefficient * q * wingArea;
//    }
//
//    glm::vec3 ClampSpeed()
//    {
//        // Check if the velocity exceeds the maximum speed
//        float currentSpeed = glm::length(physData->velocity);
//        if (currentSpeed > maxSpeed)
//        {
//            // Normalize the velocity and scale it to maxSpeed
//            return glm::normalize(physData->velocity) * maxSpeed;
//        }
//
//        // If within limits, return the original velocity
//        return physData->velocity;
//    }
//
//    glm::vec3 RotateVectorWithQuaternion(const glm::quat& quaternion, const glm::vec3& vector)
//    {
//        return glm::mat3_cast(quaternion) * vector;
//    }
//};
