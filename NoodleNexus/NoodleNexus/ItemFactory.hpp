#pragma once

#include "Scene.hpp"
#include "aRemoveAfterTime.hpp"
#include "aEquipItem.hpp"
#include "aUseItem.hpp"
#include "aPlayerHammerController.hpp"
#include "aIconNearObject.h"

#include "glm/vec3.hpp"
#include <vector>
#include <cstdlib>
#include <ctime>

class ItemFactory
{
public:
    Scene* scene;

    std::vector<glm::vec3> spawnLocations = {
        glm::vec3(0, 57, 100),
        glm::vec3(0, 87, -80),
        glm::vec3(0, 117, 55),
        glm::vec3(0, 149, -45),
    };

    ItemFactory(Scene* scene) : scene(scene) {
        std::srand(static_cast<unsigned>(std::time(0)));
    }

    void SpawnHammer(glm::vec3 position, float scale = 0.1f)
    {
        glm::vec3 rotation = glm::vec3(0, -90, 0);
        glm::vec4 color = glm::vec4(0.8f, 0.8f, 0.1f, 1.0f);

        Object* hammer = scene->GenerateMeshObjectsFromObject(
            "assets/models/dk_3d_all_obj/DonkeyKong_Level_0_Hammer.ply",
            position,
            scale,
            rotation,
            true,
            color,
            false,
            scene->sceneObjects

        );
        hammer->name = "Hammer";

        hammer->isTemporary = true;
    }

    //// Spawn a Power-Up
    //void SpawnPowerUp(glm::vec3 position, float scale = 0.3f) {
    //    glm::vec3 rotation = glm::vec3(0, 0, 0);
    //    glm::vec4 color = glm::vec4(0.1f, 0.9f, 0.1f, 1.0f); // Green

    //    Object* powerUp = scene->GenerateMeshObjectsFromObject(
    //        "assets/models/items/power_up.ply",
    //        position,
    //        scale,
    //        rotation,
    //        true,
    //        color,
    //        false,
    //        scene->sceneObjects
    //    );
    //    powerUp->name = "PowerUp";

    //    aEquipItem* equipAction = new aEquipItem();
    //    aRemoveAfterTime* removeAction = new aRemoveAfterTime();

    //    equipAction->object->name = "Mario";
    //    removeAction->timeToRemove = 20.0f;

    //    scene->AddActionToObj(equipAction, powerUp);
    //    scene->AddActionToObj(removeAction, powerUp);
    //}

    // Spawn random items at the start of the game
    void SpawnRandomItems() {
        // Shuffle and pick two unique positions from spawnLocations
        std::vector<int> indices = { 0, 1, 2, 3 };
        std::random_shuffle(indices.begin(), indices.end());

        // Spawn two items at the first two positions
        SpawnHammer(spawnLocations[indices[0]]);
        //SpawnPowerUp(spawnLocations[indices[1]]);
    }
};