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
    float cameraHeight = 0.1f;
    Object* mirrorObject = nullptr;

    virtual void Update() {
        // Ensure we have a valid reflection camera and mirror object.
        if (!mirrorObject)
            return;

        // Calculate the position for the reflection camera.
        // Place it directly above the mirror center at a fixed height.
        glm::vec3 camPos = mirrorCenter + glm::vec3(0.0f, cameraHeight, 0.0f);

        // Update the reflection camera's position and orientation.
        // Here, we want it to look straight down.
        Camera* reflCamData = object->scene->fCamera->getCameraData();
        if (!reflCamData) return;

        reflCamData->position = camPos;
        // Set rotation so it looks directly downward:
        // pitch = -90 degrees, yaw = 0, roll = 0.
        object->mesh->rotationEulerXYZ = glm::vec3(-90.0f, 0.0f, 0.0f);
        //reflCamData->FOV = fov;

        // Update the reflection camera's view matrix (or equivalent).
       
        float distance = glm::length(camPos - mirrorCenter);
        float adjustedFOV = 100.f * (1.0f + fovAdjustmentFactor / (distance + 0.001f));
        cameraController->FOV = 100.f;// adjustedFOV;;

        //std::cout <<"Camera Position: "<< camPos.x << " " << camPos.y << " " << camPos.z << std::endl;
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