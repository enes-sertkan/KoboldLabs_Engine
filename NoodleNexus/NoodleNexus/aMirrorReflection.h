#pragma once
#include "sObject.h"
#include "Action.h"
#include "Scene.hpp"
#include <glm/glm.hpp>
#include "Camera.hpp"
#include "aCameraToTexture.h"

class cCamera;

class aMirrorReflection : public Action {
public:
    glm::vec3 mirrorCenter = glm::vec3(0.0f, 0.0f, 0.0f);
    Camera* mainCamera = nullptr;
    CameraToTexture* cameraController = nullptr;
    float fovAdjustmentFactor = 0.1f;

    Object* mirrorObject = nullptr;

    virtual void Update() {
        mainCamera = object->scene->fCamera->getCameraData();
        if (!mainCamera || !cameraController) return;

        // Reflect position across mirror plane (Y-axis)
        glm::vec3 mainPos = mainCamera->position;
        glm::vec3 reflectedPos = glm::vec3(
            mainPos.x,
            2.0f * mirrorCenter.y - mainPos.y, // Proper reflection formula
            mainPos.z
        );

        // Apply reflected position to reflection camera
        object->mesh->positionXYZ = reflectedPos;

        // Reflect main camera's rotation (invert pitch)
        glm::vec3 mainRot = mainCamera->rotation;
        object->mesh->rotationEulerXYZ = CalculateLookAtRotation(reflectedPos, mirrorCenter);
        
        object->mesh->rotationEulerXYZ.z += 180;

         float distance = glm::length(mainPos - reflectedPos);
        float adjustedFOV = 60.f * (1.0f + fovAdjustmentFactor / (distance + 0.001f));
        cameraController->FOV = 60.f;// adjustedFOV;
      //  RotateMeshTowardsPlayer(mirrorObject->mesh->positionXYZ, mainCamera->position);
    }

    void RotateMeshTowardsPlayer(const glm::vec3 reflectedPos, const glm::vec3 playerPos) {
        // Calculate the direction from the reflected position to the player
        glm::vec3 direction = glm::normalize(playerPos - reflectedPos);

        // Convert direction to Euler angles
        float pitch = glm::degrees(asin(-direction.y)); // Vertical angle (inverted)
        float yaw = glm::degrees(atan2(direction.x, direction.z)); // Horizontal angle

        // Apply rotation to the cameraController's mesh
        mirrorObject->mesh->rotationEulerXYZ = glm::vec3(mirrorObject->mesh->rotationEulerXYZ.x, yaw, mirrorObject->mesh->rotationEulerXYZ.z);
    }
    glm::vec3 CalculateLookAtRotation( glm::vec3 fromPos,  glm::vec3 targetPos) {
        // Calculate direction to mirror center
        glm::vec3 direction = glm::normalize(targetPos - fromPos);

        // Calculate pitch (X-axis rotation)
        float pitch = glm::degrees(asin(-direction.y)); // Negative because in GLM, positive Y is up

        // Calculate yaw (Y-axis rotation)
        float yaw = glm::degrees(atan2(direction.x, direction.z));

        // Roll is 0 by default for a flat mirror
        float roll = 0.0f;

        return glm::vec3(pitch, yaw, roll);
    }

};