#pragma once

#include "Action.h";
#include <glm/vec3.hpp> 
#include <glm/vec4.hpp> 
#include "sObject.h"
#include "Scene.hpp"
#include <glm/glm.hpp>

#include "aFlameEnemyController.hpp"

class aStandingBarrel : public Action
{
public:
	Object* fire = nullptr;
	bool isON = false;
    float timer = 0;
    float timeToSpawnEnemy = 5.f;



    void SpawnFlameEnemy(glm::vec3 position)
    {

        glm::vec3 rotation = glm::vec3(0, -90, 0);
        float scale = 0.1;

        Object* barrel = object->scene->GenerateMeshObjectsFromObject("assets/models/dk_3d_all_obj/DonkeyKong_Level_0_FlameEnemy_0.ply", position, scale, rotation, true, glm::vec4(0.7, 0.4, 0.4, 1.f), false, object->scene->sceneObjects);
        barrel->name = "FlamwEnemy";

        aRayCastPhysics2D* physics2Db = new aRayCastPhysics2D();
      
        aRemoveAfterTime* removeBarrelAction = new aRemoveAfterTime();
        aRollDown* barrelRoll = new aRollDown();
        aModelsFramesAnimator* flameAnimator = new aModelsFramesAnimator();
        aFlameEnemyController* enemyController = new aFlameEnemyController();

        aRollDown* climbAction = new aRollDown();

        barrelRoll->rollDownChance = 100;

        physics2Db->gravityAcceleration = glm::vec3(0, -0.05f, 0);
        physics2Db->speed = glm::vec3(0.f, 0.f, 1.f);
        physics2Db->baseRayCastLength = 5.f;
        physics2Db->bounciness = 1.f;

     

        //barrelRoll->descentSpeed = 1;
        //barrelRoll->phys = physics2Db;
        //barrelRoll->animator = barrelAnimator;
        //barrelRoll->velocity = physics2Db->speed;

        removeBarrelAction->timeToRemove = 52.0f;

        barrel->isTemporary = true;

        std::vector<std::string> flameEnemyAnim = { "assets/models/dk_3d_all_obj/DonkeyKong_Level_0_FlameEnemy_0.ply" ,
                                                    "assets/models/dk_3d_all_obj/DonkeyKong_Level_0_FlameEnemy_1.ply" ,
                                                    "assets/models/dk_3d_all_obj/DonkeyKong_Level_0_FlameEnemy_2.ply" ,
                                                    "assets/models/dk_3d_all_obj/DonkeyKong_Level_0_FlameEnemy_3.ply" 
                                                };
        flameAnimator->animations.push_back(flameEnemyAnim);
        flameAnimator->animationSpeed = 4;



        enemyController->animator = flameAnimator;
        enemyController->physics = physics2Db;


        climbAction->rayHeight = 12;
        climbAction->raySpacing = -5;
        climbAction->rayAmmont = 4;
        climbAction->descentSpeed = -3.f;
        climbAction->rollDownChance = 1000;
        climbAction->rayLength = 20.f;
        climbAction->phys = physics2Db;

        object->scene->AddActionToObj(physics2Db, barrel);
        object->scene->AddActionToObj(climbAction, barrel);
        object->scene->AddActionToObj(removeBarrelAction, barrel);
        object->scene->AddActionToObj(flameAnimator, barrel);
        object->scene->AddActionToObj(enemyController, barrel);

    }



	void Update() override
	{


    


		for (Object* obj:object->scene->sceneObjects)
		{
			if (obj->name == "BlueBarrel")
			{
				if (glm::distance(obj->mesh->positionXYZ, object->mesh->positionXYZ) < 5)
				{
					object->mesh->objectColourRGBA = glm::vec4(1.f, 1.f, 0.f, 1.f);

					if (fire) fire->mesh->bIsVisible = true;

					isON = true;

				}
			}

		}

        if (!isON) return;


        timer += object->scene->deltaTime;
        if (timer > timeToSpawnEnemy)
        {
            timer = 0;
            SpawnFlameEnemy(object->mesh->positionXYZ);
        }
		

	}
};