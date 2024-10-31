#include "Scene.hpp"
#include "cVAOManager/cVAOManager.h"
#include "GLCommon.h"
#include <iostream>
#include "ScarEffect.hpp"
#include <string>
#include <glm/glm.hpp>

ScarEffect* scarAction = new ScarEffect();
//const float COLLISION_THRESHOLD = 4000.0f;


// Helper function to convert glm::vec3 to string
std::string vec3ToString(const glm::vec3& vec) {
    return "{" + std::to_string(vec.x) + ", " + std::to_string(vec.y) + ", " + std::to_string(vec.z) + "}";
}

sModelDrawInfo LoadDrawInfo(cVAOManager* meshManager, sModelDrawInfo drawInfo, GLuint program)
{
    meshManager->LoadModelIntoVAO(drawInfo.meshPath,
        drawInfo, program);

    std::cout << drawInfo.meshPath << "-Loaded" << std::endl << drawInfo.numberOfVertices << " vertices loaded" << std::endl;

    return drawInfo;
}


sMesh* CreateMeshObjects(std::vector<sMesh*>& meshes, sMesh* mesh)
{

    meshes.push_back(mesh);

    return mesh;

}

void Scene::Prepare(cVAOManager* meshManager, GLuint program, std::vector<sMesh*>& meshes)
{
    for (sModelDrawInfo info : modelInfos)
    {
        LoadDrawInfo(meshManager, info, program);
    }


    for (Object* object : sceneObjects)
    {
        CreateMeshObjects(meshes, object->mesh);
        object->scene = this;
    }

    

}

// Question_2 collision detection

std::vector<Object*> Scene::GetAsteroids() {
    std::vector<Object*> asteroids;
    for (Object* obj : sceneObjects) {
        if (obj->type == "asteroid") {
            asteroids.push_back(obj);
        }
    }
    return asteroids;
}


// Question 2 collisoin detection
const glm::vec3 COLLISION_THRESHOLD = glm::vec3(4000.0f, 4000.0f, 7000.0f);

void Scene::CheckForCollisions() {
    glm::vec3 spaceStationPos = sceneObjects[1]->mesh->positionXYZ; 

    // Log the space station position
    std::cout << "Space Station Position: " << vec3ToString(spaceStationPos) << std::endl;

    for (Object* obj : GetAsteroids()) {
        if (obj->type == "asteroid") {

            glm::vec3 delta = obj->mesh->positionXYZ - spaceStationPos;

            // Log the asteroid position and each axis difference
            std::cout << "Asteroid Position: " << vec3ToString(obj->mesh->positionXYZ) << std::endl;
            std::cout << "Delta X: " << std::abs(delta.x) << " (Threshold X: " << COLLISION_THRESHOLD.x << ")" << std::endl;
            std::cout << "Delta Y: " << std::abs(delta.y) << " (Threshold Y: " << COLLISION_THRESHOLD.y << ")" << std::endl;
            std::cout << "Delta Z: " << std::abs(delta.z) << " (Threshold Z: " << COLLISION_THRESHOLD.z << ")" << std::endl;

            if (std::abs(delta.x) < COLLISION_THRESHOLD.x &&
                std::abs(delta.y) < COLLISION_THRESHOLD.y &&
                std::abs(delta.z) < COLLISION_THRESHOLD.z) {

                // Create explosion effect
                Object* explosion = CreateObject(
                    obj->mesh->positionXYZ,
                    glm::vec3(0),
                    500.f,
                    glm::vec4(1, 0, 0, 1),
                    "ExplosionSphereModel",
                    "assets/models/Sphere_radius_1_xyz_N_uv.ply",
                    "Explosion"
                );
                explosion->mesh->bDoNotLight = true;

                std::cout << "Collision detected! Creating explosion effect." << std::endl;

                ExplosionLogic* explosionAction = new ExplosionLogic();
                explosionAction->deltaTime = deltaTime;
                AddActionToObj(explosionAction, explosion);

                // Create scar effect
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

                // Remove asteroid from scene
                RemoveObject(obj);
                std::cout << "Asteroid removed from scene." << std::endl;
            }
        }
    }
}




void Scene::Update() {
    CalculateDeltaTime(); 

    // Update all actions
    for (Action* action : actions) {
        if (action->object) {
            action->deltaTime = deltaTime;
            action->Update();
        }
    }
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


