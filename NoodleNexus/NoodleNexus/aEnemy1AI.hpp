#include "aEnemyAI.hpp"
#include <glm/gtc/matrix_transform.hpp>  // For glm::lookAt, glm::yawPitchRoll
#include <cmath>                        // For atan2 and M_PI

class Enemy1AI : public aEnemyAI {
private:
    float seekSpeed = 3.0f;
    float fleeSpeed = 4.0f;

public:
    void Update() override {
        if (!player) return;

        glm::vec3 toPlayer = GetPlayerPosition() - object->mesh->positionXYZ;

        bool shouldFlee = IsPlayerFacingEnemy();

        if (shouldFlee) {
            Flee(toPlayer);
        }
        else {
            Seek(toPlayer);
        }
    }

    bool IsPlayerFacingEnemy() {
        if (!player || !player->mesh) return false;

        glm::vec3 playerForward = player->mesh->rotationEulerXYZ;
        if (playerForward == glm::vec3(0)) {
            playerForward = glm::vec3(0, 0, 1); // Default forward direction
        }

        glm::vec3 toEnemy = glm::normalize(object->mesh->positionXYZ - GetPlayerPosition());
        float dotProduct = glm::dot(glm::normalize(playerForward), glm::normalize(toEnemy));

        return dotProduct > 0.2f; // Adjusted threshold
    }

    void Seek(const glm::vec3& direction) {
        if (!object || !object->mesh) return;

        glm::vec3 normalizedDirection = glm::normalize(direction);
        object->mesh->positionXYZ += normalizedDirection * seekSpeed * object->scene->deltaTime;

        // Rotate the AI to face the direction it is moving
        RotateTowards(normalizedDirection);
    }

    void Flee(const glm::vec3& direction) {
        if (!object || !object->mesh) return;

        glm::vec3 normalizedDirection = glm::normalize(direction);
        object->mesh->positionXYZ -= normalizedDirection * fleeSpeed * object->scene->deltaTime;

        // Rotate the AI to face the opposite direction of fleeing
        RotateTowards(-normalizedDirection);
    }

private:
    void RotateTowards(const glm::vec3& direction) {
        // Calculate the angle in radians
        float angle = atan2(direction.x, direction.z);

        // Convert radians to degrees (if your rotation system uses degrees)
        float angleDegrees = glm::degrees(angle);

        // Update the rotation Euler angles (yaw is around the Y-axis)
        object->mesh->rotationEulerXYZ.y = angleDegrees;
    }
};