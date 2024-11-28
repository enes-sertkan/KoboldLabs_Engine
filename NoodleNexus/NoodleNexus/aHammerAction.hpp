#pragma once

#include "Action.h"
#include "Scene.hpp"
#include "aScoreManager.h"


class aHammerAction : public Action
{
public:
    float effectRange = 7.5f;
    bool isOn = true;
    aScoreManager* scoreManager;

    void Update() override {
        if (!isOn) return;

        for (Object* target : object->scene->sceneObjects) {
            if (target->name == "Barrel" || target->name == "Flame_Enemy" || target->name == "BlueBarrel") {
                if (glm::distance(object->mesh->positionXYZ, target->mesh->positionXYZ) <= effectRange) {

                    aModelsFramesAnimator* explosionAnimator = new aModelsFramesAnimator();
                    explosionAnimator->animationSpeed = 5;
                    std::vector<std::string> explosionFrames0 = { "assets/models/dk_3d_all_obj/DonkeyKong_Level_0_Explosion_0.ply",
                                                                "assets/models/dk_3d_all_obj/DonkeyKong_Level_0_Explosion_1.ply",
                                                                "assets/models/dk_3d_all_obj/DonkeyKong_Level_0_Explosion_2.ply",
                                                                "assets/models/dk_3d_all_obj/DonkeyKong_Level_0_Explosion_3.ply" };

                    explosionAnimator->animations.push_back(explosionFrames0);

                    Object* explosion = object->scene->GenerateMeshObjectsFromObject(
                        "assets/models/dk_3d_all_obj/DonkeyKong_Level_0_Explosion_0.ply",
                        target->mesh->positionXYZ,
                        0.2f,
                        glm::vec3(-90.0f, 0.0f, 0.0f),
                        true,
                        glm::vec4(1.0f, 0.1f, 0.1f, 1.0f),
                        false,
                        object->scene->sceneObjects
                        );
                    


                    object->scene->AddActionToObj(explosionAnimator, explosion);

                    target->Destroy();

                    glm::vec3 offset = glm::vec3(0.0f, 6.0f, 0.0f);
                    if (scoreManager)
                        scoreManager->DrawScore(target->mesh->positionXYZ + offset);

                    aRemoveAfterTime* removeAction = new aRemoveAfterTime();
                    removeAction->timeToRemove = 5.0f; 
                    object->scene->AddActionToObj(removeAction, explosion);

                }
            }
        }
    }

};