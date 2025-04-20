#pragma once

#include "Action.h"
#include "LabAttackFactory.h"
#include <glm/vec3.hpp>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include "SlimeController.h"

class aPlayerShooting : public Action {
public:
    // Pool settings
    const int MAX_POOL_SIZE = 50;
    std::vector<SoftBody*> slimePool;
    std::vector<SlimeController*> slimeControllers;

    LabAttackFactory* factory = nullptr;
    float fireRate = 0.15f;
    float spawnRate = 0.8f;
    float timeSinceLastShot = 0.0f;
    float timeSinceLastSpawn = 0.0f;

    virtual void Start() override {
        timeSinceLastShot = fireRate;
    }

    virtual void Update() override {
        timeSinceLastShot += object->scene->deltaTime;
        timeSinceLastSpawn += object->scene->deltaTime;
        if (glfwGetMouseButton(object->scene->window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
            if (timeSinceLastShot >= fireRate && factory) {
                glm::vec3 playerPos = object->mesh->positionXYZ;
                playerPos.y += 4.0f;
                glm::vec2 eyeRot = object->scene->fCamera->getEyeRotation();
                float yaw = glm::radians(eyeRot.y) - glm::radians(90.0f);
                float pitch = glm::radians(eyeRot.x);

                glm::vec3 shootDir(
                    sin(yaw) * cos(pitch),
                    -sin(pitch),
                    cos(yaw) * cos(pitch)
                );
                shootDir = glm::normalize(shootDir);

                glm::vec3 bulletSpeed = shootDir * 40.0f;
                glm::vec3 spawnPos = playerPos + shootDir * 1.0f;
                spawnPos.y -= 4.5f;


             
                factory->SpawnPlayerBullet(spawnPos, bulletSpeed);
                timeSinceLastShot = 0.0f;
            }
        }      
        
        if (glfwGetKey(object->scene->window, GLFW_KEY_F) == GLFW_PRESS) {
            if (timeSinceLastSpawn >= spawnRate && factory) {
                glm::vec3 playerPos = object->mesh->positionXYZ;
                playerPos.y += 4.0f;
                glm::vec2 eyeRot = object->scene->fCamera->getEyeRotation();
                float yaw = glm::radians(eyeRot.y) - glm::radians(90.0f);
                float pitch = glm::radians(eyeRot.x);

                glm::vec3 shootDir(
                    sin(yaw) * cos(pitch),
                    -sin(pitch),
                    cos(yaw) * cos(pitch)
                );
                shootDir = glm::normalize(shootDir);

                glm::vec3 bulletSpeed = shootDir * 100.0f;
                glm::vec3 spawnPos = playerPos + shootDir * 1.0f;
                spawnPos.y -= 4.5f;


             
                SpawnSlime(object->scene, spawnPos, factory->maze, bulletSpeed);
                timeSinceLastSpawn = 0.0f;
            }
        }
    }

    void SpawnSlime(Scene* scene, glm::vec3 pos, MazeGenerator* maze, glm::vec3 velocity) {
        SoftBody* slimeBody = nullptr;
        Object* slimeObj = nullptr;
        SlimeController* slimeController = nullptr;

        float randomScale = 0.2f + static_cast<float>(rand()) / RAND_MAX * (0.25f);
        glm::vec4 randomColor(
            0.5f + static_cast<float>(rand()) / RAND_MAX * 0.5f,
            0.1f + static_cast<float>(rand()) / RAND_MAX * 0.3f,
            0.2f + static_cast<float>(rand()) / RAND_MAX * 0.8f,
            1.0f
        );

        if (slimePool.size() >= MAX_POOL_SIZE) {
            auto it = std::min_element(slimePool.begin(), slimePool.end(),
                [](SoftBody* a, SoftBody* b) { return a->lastUsedTime < b->lastUsedTime; });
            size_t idx = std::distance(slimePool.begin(), it);
            slimeBody = *it;
            slimeObj = slimeBody->object;
            slimeController = slimeControllers[idx];

            slimeBody->Reset();
            slimeBody->MoveTo(pos);
            slimeBody->initialVelocity = velocity;
            slimeObj->mesh->uniformScale = randomScale;
            slimeObj->mesh->objectColourRGBA = randomColor;
        }
        else {
            slimeObj = scene->GenerateMeshObjectsFromObject(
                "assets/models/Sphere_radius_1_xyz_N_uv.ply",
                pos, randomScale, glm::vec3(0.0f), true, randomColor,
                true, scene->sceneObjects
            );

            slimeBody = new SoftBody();
            slimeBody->acceleration.y = -16.0f;
            slimeBody->constIterations = 2;
            slimeBody->tighness = 1.9f;
            slimeBody->useVolume = true;
            slimeBody->easyControl = true;
            slimeBody->SetMazeToSBCollision(maze);
            slimeBody->initialVelocity = velocity;

            scene->AddActionToObj(slimeBody, slimeObj);
            slimeBody->Start();

            slimeController = new SlimeController();
            slimeController->factory = factory;
            slimeController->softBody = slimeBody;

            // Connect to existing slimes
            for (size_t i = 0; i < slimePool.size(); ++i) {
                SlimeController* otherCtrl = slimeControllers[i];
                SoftBody* otherBody = slimePool[i];
                otherCtrl->AddOtherSlime(slimeBody);
                slimeController->AddOtherSlime(otherBody);
            }

            scene->AddActionToObj(slimeController, slimeObj);
            slimePool.push_back(slimeBody);
            slimeControllers.push_back(slimeController);

            if (factory->grass) {
                aGrassCollider* grassColl = new aGrassCollider();
                grassColl->SetGrass(factory->grass);
                grassColl->colliderRadius = 1.4f;
                grassColl->colliderBlendRadius = 1.5f;
                grassColl->softBody = slimeBody;
                scene->AddActionToObj(grassColl, slimeObj);
            }
        }

        slimeObj->mesh->positionXYZ = pos;
        slimeObj->isTemporary = true;
        slimeObj->mesh->metal = 0.8f;
        slimeObj->mesh->smoothness = 0.9f;
        slimeObj->mesh->drawBothFaces = true;

        slimeBody->lastUsedTime = glfwGetTime();
    }
};
