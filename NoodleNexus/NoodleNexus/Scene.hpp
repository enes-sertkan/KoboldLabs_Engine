#pragma once

#include "sObject.h"
#include <vector>
#include "cVAOManager/cVAOManager.h"
#include "GLCommon.h"
#include <iostream>
#include <GLFW/glfw3.h>
#include "sharedThings.h"
#include "Action.h"
#include "Transform.h"
#include "cBasicFlyCamera/cBasicFlyCamera.h"
#include "PhysicsManager.h"
#include <thread>
#include <chrono>

class Scene {
public:
    bool flyCamera = true;
    bool laserEnabled = false;
    float stationLength = 1000.0f; // Defined station length

    std::vector<Object*> temporaryObjects;
    PhysicsManager* physicsThings;
    std::vector<sModelDrawInfo> modelInfos;
    std::vector<Object*> sceneObjects;
    std::vector<Action*> actions;
    std::vector<Object*> GetAsteroids();

    cLightManager* lightManager;

    std::vector<std::string> modelPaths;
    std::vector<Transform*> cameraPositions;
    int currentCameraIndex = 0;

    void MoveCameraToPoint();
    void SetCameraToNextPoint();
    void SetCameraToFirstPoint();
    void NextCameraPoint();

    void RemoveObject(Object* object);
    void CheckLaserHit();
    void FireLaser(Object* asteroid, const glm::vec3& stationPos);
    void TriggerExplosionEffect(Object* asteroid);
    void ScheduleLaserCleanup();
    void Update();
    void Prepare(cVAOManager* meshManager, GLuint program, std::vector<sMesh*>& meshes);
    void CheckForCollisions();
    void AddActionToObj(Action* action, Object* object);

    double currentTime, lastTime, deltaTime;

    void CalculateDeltaTime() {
        currentTime = glfwGetTime();
        deltaTime = currentTime - lastTime;
        lastTime = currentTime;
    }

    Object* CreateObject(glm::vec3 position, glm::vec3 rotation, float scale, glm::vec4 color, const std::string& name, const std::string& modelPath, const std::string& objectType) {
        Object* newObject = new Object();
        newObject->name = name;
        newObject->mesh = new sMesh();
        newObject->mesh->modelFileName = modelPath;
        newObject->mesh->positionXYZ = position;
        newObject->mesh->rotationEulerXYZ = rotation;
        newObject->mesh->uniformScale = scale;
        newObject->mesh->objectColourRGBA = color;
        newObject->mesh->bOverrideObjectColour = true;
        newObject->isTemporary = true;
        newObject->type = objectType;
        sceneObjects.push_back(newObject);
        return newObject;
    }
};
