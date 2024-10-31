#include "Scene.hpp"
#include "cVAOManager/cVAOManager.h"
#include "GLCommon.h"
#include <iostream>
#include "ScarEffect.hpp"
#include <string>
#include <glm/glm.hpp>
#include <chrono>

ScarEffect* scarAction = new ScarEffect();
float stationLength = 1000.0f;
//const float COLLISION_THRESHOLD = 4000.0f;


// Helper function to convert glm::vec3 to string
std::string vec3ToString(const glm::vec3& vec) {
    return "{" + std::to_string(vec.x) + ", " + std::to_string(vec.y) + ", " + std::to_string(vec.z) + "}";
}

// Helper function to load models
sModelDrawInfo LoadDrawInfo(cVAOManager* meshManager, sModelDrawInfo drawInfo, GLuint program) {
    meshManager->LoadModelIntoVAO(drawInfo.meshPath, drawInfo, program);
    std::cout << drawInfo.meshPath << " - Loaded" << std::endl;
    std::cout << drawInfo.numberOfVertices << " vertices loaded" << std::endl;
    return drawInfo;
}

// Helper function to create meshes and add to vector
sMesh* CreateMeshObjects(std::vector<sMesh*>& meshes, sMesh* mesh) {
    meshes.push_back(mesh);
    return mesh;
}

void Scene::Prepare(cVAOManager* meshManager, GLuint program, std::vector<sMesh*>& meshes) {
    for (sModelDrawInfo info : modelInfos) {
        LoadDrawInfo(meshManager, info, program);
    }
    for (Object* object : sceneObjects) {
        CreateMeshObjects(meshes, object->mesh);
        object->scene = this;
    }
}

// Question 2 collision detection
std::vector<Object*> Scene::GetAsteroids() {
    std::vector<Object*> asteroids;
    for (Object* obj : sceneObjects) {
        if (obj->type == "asteroid") {
            asteroids.push_back(obj);
        }
    }
    return asteroids;
}

// Collision threshold
const glm::vec3 COLLISION_THRESHOLD = glm::vec3(3500.0f, 4200.0f, 20000.0f);
void Scene::CheckForCollisions() {
    glm::vec3 spaceStationPos = sceneObjects[1]->mesh->positionXYZ;

    for (Object* obj : GetAsteroids()) {
        glm::vec3 delta = obj->mesh->positionXYZ - spaceStationPos;

        if (std::abs(delta.x) < COLLISION_THRESHOLD.x &&
            std::abs(delta.y) < COLLISION_THRESHOLD.y &&
            std::abs(delta.z) < COLLISION_THRESHOLD.z) {

            Object* explosion = CreateObject(
                obj->mesh->positionXYZ,
                glm::vec3(0),
                500.f,
                glm::vec4(1, 0, 0, 1),
                "ExplosionSphereModel",
                "assets/models/Sphere_radius_1_xyz_N_uv.ply",
                "Explosion"
            );

            ExplosionLogic* explosionAction = new ExplosionLogic();
            explosionAction->deltaTime = deltaTime;
            AddActionToObj(explosionAction, explosion);

            explosion->mesh->bDoNotLight = true;

            Object* scar = CreateObject(
                obj->mesh->positionXYZ,
                glm::vec3(0),
                250.f,
                glm::vec4(0.2f, 0.2f, 0.2f, 1),
                "ScarModel",
                "assets/models/Sphere_radius_1_xyz_N_uv.ply",
                "Scar"
            );

            ScarEffect* scarAction = new ScarEffect();
            AddActionToObj(scarAction, scar);
            RemoveObject(obj);
        }
    }
}

const glm::vec3 laserOffset = glm::vec3(0.0f, 0.0f, 10.0f);

void Scene::CheckLaserHit() {
    if (!laserEnabled) return;

    glm::vec3 stationPos = sceneObjects[1]->mesh->positionXYZ;  // Assuming the station is at index 1
    float laserRange = stationLength / 2.0f;  // Half the length of the space station

    for (Object* asteroid : GetAsteroids()) {
        if (asteroid->type == "asteroid") {
            float distance = glm::distance(stationPos, asteroid->mesh->positionXYZ);
            if (distance < laserRange) {
                FireLaser(asteroid, stationPos);  // Trigger the laser
                ScheduleLaserCleanup();  // Schedule the cleanup of temporary objects
                break;  // Stop after hitting one asteroid at a time
            }
        }
    }
}

void Scene::FireLaser(Object* asteroid, const glm::vec3& stationPos) {
    glm::vec3 laserStart = stationPos + laserOffset;  // Start position of the laser
    glm::vec3 direction = glm::normalize(asteroid->mesh->positionXYZ - laserStart);

    float step = 50.0f;  // Step size for creating laser segments
    for (float t = 0.0f; t < glm::distance(laserStart, asteroid->mesh->positionXYZ); t += step) {
        glm::vec3 spherePos = laserStart + direction * t;
        Object* laserPart = CreateObject(spherePos, glm::vec3(0), 5.0f, glm::vec4(0, 1, 0, 1), "LaserSphere", "", "laser");
        temporaryObjects.push_back(laserPart);  // Keep track of temporary objects
    }

    TriggerExplosionEffect(asteroid);  // Trigger explosion effect for the asteroid
}

void Scene::TriggerExplosionEffect(Object* asteroid) {
    Object* explosion = CreateObject(
        asteroid->mesh->positionXYZ,
        glm::vec3(0),
        10.0f,
        glm::vec4(1, 0, 0, 1),
        "ExplosionSphere",
        "",
        "explosion"
    );

    ExplosionLogic* explosionAction = new ExplosionLogic();
    AddActionToObj(explosionAction, explosion);
    RemoveObject(asteroid);
}

void Scene::ScheduleLaserCleanup() {
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    for (Object* obj : temporaryObjects) {
        RemoveObject(obj);
    }
    temporaryObjects.clear();
}

void Scene::Update() {
    CalculateDeltaTime();
    for (Action* action : actions) {
        if (action->object) {
            action->deltaTime = deltaTime;
            action->Update();
        }
    }

    // Check for laser hits during each update
    CheckLaserHit();
}

void Scene::AddActionToObj(Action* action, Object* obj) {
    if (action && obj) {
        action->object = obj;
        actions.push_back(action);
    }
    else {
        std::cerr << "Failed to add action. Invalid action or object!" << std::endl;
    }
}



void Scene::MoveCameraToPoint()
{
   if (cameraPositions.size() == 0) return;
   glm::vec3 newPos = moveTowards(g_pFlyCamera->getEyeLocation(), cameraPositions[currentCameraIndex]->position, 10.f);
   g_pFlyCamera->setEyeLocation(newPos);
}

void Scene::SetCameraToNextPoint()
{
    if (cameraPositions.size() == 0) return;

    if (currentCameraIndex >= cameraPositions.size() - 1) currentCameraIndex = -1;
    currentCameraIndex++;

    g_pFlyCamera->setEyeLocation(cameraPositions[currentCameraIndex]->position);
}

void Scene::SetCameraToFirstPoint()
{
    if (cameraPositions.size() == 0) return;
    g_pFlyCamera->setEyeLocation(cameraPositions[1]->position);
}

void Scene::NextCameraPoint()
{
      if (cameraPositions.size() == 0) return;

    if (currentCameraIndex >= cameraPositions.size() - 1) currentCameraIndex = -1;
    currentCameraIndex++;
}

void Scene::RemoveObject(Object* obj)
{
    // Find the object in the sceneObjects vector
    std::vector<Object*>::iterator it = std::find(sceneObjects.begin(), sceneObjects.end(), obj);
    if (it != sceneObjects.end())
    {
        // Delete the object and free memory
        delete* it; // Free the memory of the object
        sceneObjects.erase(it); // Remove from the vector
    }

    // Now go through the actions and delete actions that have this object as their object
    std::vector<Action*>::iterator actionIt = actions.begin();
    while (actionIt != actions.end())
    {
        if ((*actionIt)->object == obj)
        {
            delete* actionIt; // Free the memory of the action
            actionIt = actions.erase(actionIt); // Remove from the vector and update iterator
        }
        else
        {
            ++actionIt; // Move to the next action
        }
    }
}


