#pragma once
#pragma once
#include "Action.h"
#include "glm/glm.hpp"
#include "sObject.h"
#include "Scene.hpp"
#include <cstdlib>

class aLaunchAction : public Action {
private:
    glm::vec3 velocity;
    glm::vec3 angularVelocity;
    float floorY;

public:
    // Constructor: initialize with the object to launch and the desired floor height.
    aLaunchAction(Object* obj, float floorHeight)
        : floorY(floorHeight)
    {
        object = obj;
        float dt = object->scene->deltaTime; // just in case we need delta time later

        // Launch in a random direction: random X/Z with a fixed upward boost.
        float randomX = (static_cast<float>(rand()) / RAND_MAX) * 2.0f - 1.0f;
        float randomZ = (static_cast<float>(rand()) / RAND_MAX) * 2.0f - 1.0f;
        glm::vec3 direction = glm::normalize(glm::vec3(randomX, 1.0f, randomZ));

        // Set a random speed between 5 and 10 units per second.
        float speed = 15.0f + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / 5.0f));
        velocity = direction * speed;

        // Give the object a random angular velocity (degrees per second).
        angularVelocity = glm::vec3(
            (static_cast<float>(rand()) / RAND_MAX) * 720.0f,
            (static_cast<float>(rand()) / RAND_MAX) * 720.0f,
            (static_cast<float>(rand()) / RAND_MAX) * 720.0f
        );
    }

    virtual void Update() override {
        if (!object || !object->scene) return;

        // Simulate gravity.
        const float gravity = -9.81f;
        float dt = object->scene->deltaTime;
        velocity.y += gravity * dt;

        // Update position based on velocity.
        object->mesh->positionXYZ += velocity * dt;

        // Update rotation based on angular velocity.
        object->mesh->rotationEulerXYZ += angularVelocity * dt;

        // When the object falls below the floor, destroy it.
        if (object->mesh->positionXYZ.y < floorY-2) {
            object->Destroy();
        }
    }
};
