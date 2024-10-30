#pragma once
#include "Action.h"
#include <random>

class aAsteroidRotation : public Action {
public:
    glm::vec3 rotationSpeedo;
    bool isRotationing = true;

    aAsteroidRotation() {
        // Set random initial rotation speed for x, y, and z
        rotationSpeedo = glm::vec3(randomFloat(-50.0f, 50.0f), randomFloat(-50.0f, 50.0f), randomFloat(-50.0f, 50.0f));
    }

    void Update() override {
        if (isRotationing) {
            // Apply rotation speed multiplied by deltaTime
            object->mesh->rotationEulerXYZ.x += rotationSpeedo.x * deltaTime;
            object->mesh->rotationEulerXYZ.y += rotationSpeedo.y * deltaTime;
            object->mesh->rotationEulerXYZ.z += rotationSpeedo.z * deltaTime;
        }
    }

private:
    // Helper function to generate random float within a specified range
    float randomFloat(float min, float max) {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        std::uniform_real_distribution<float> dist(min, max);
        return dist(gen);
    }
};
