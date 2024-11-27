#pragma once

#include "aRayCastPhysics2D.hpp"
#include "aPlayerMovement2D.hpp"
#include "aRotateAction.hpp"
#include "GLCommon.h"
#include "aRemoveAfterTime.hpp"
#include "aRollDown.hpp"


//#include "linmath.h"
#include <glm/glm.hpp>
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> 
#include "aStandingBarrel.hpp"


class BarrelFactory
{
public:
	Scene* scene;

	void SpawnRegularBarrel()
	{
        glm::vec3 position = glm::vec3(0, 180, -70);
        glm::vec3 rotation = glm::vec3(0, -90, 0);
        float scale = 0.1;

        Object* barrel = scene->GenerateMeshObjectsFromObject("assets/models/dk_3d_all_obj/DonkeyKong_Level_0_Barrel_2.ply", position, scale, rotation, true, glm::vec4(0.5, 0.25, 0.1, 1.f), false, scene->sceneObjects);
        barrel->name = "Barrel";
        
        aRayCastPhysics2D* physics2Db = new aRayCastPhysics2D();
        aRotateAction* rotationBarrel = new aRotateAction();
        aRemoveAfterTime* removeBarrelAction = new aRemoveAfterTime();
        aRollDown* barrelRoll = new aRollDown();
        aModelsFramesAnimator* barrelAnimator = new aModelsFramesAnimator();


        physics2Db->gravityAcceleration = glm::vec3(0, -0.05f, 0);
        physics2Db->speed = glm::vec3(0.f, 0.f, 1.f);
        physics2Db->baseRayCastLength = 5.f;
        physics2Db->bounciness = 1.f;

        rotationBarrel->rotationSpeed = 250.0f;
        rotationBarrel->physics = physics2Db;

        barrelRoll->descentSpeed = 1;
        barrelRoll->phys = physics2Db;
        barrelRoll->animator = barrelAnimator;
        //barrelRoll->velocity = physics2Db->speed;

        removeBarrelAction->timeToRemove = 52.0f;

        barrel->isTemporary = true;

        std::vector<std::string> berrelRollAnim = { "assets/models/dk_3d_all_obj/DonkeyKong_Level_0_Barrel_2.ply" };
        std::vector<std::string> berrelClimbAnim = { "assets/models/dk_3d_all_obj/DonkeyKong_Level_0_Barrel_5.ply" };
        barrelAnimator->animations.push_back(berrelRollAnim);
        barrelAnimator->animations.push_back(berrelClimbAnim);



        scene->AddActionToObj(physics2Db, barrel);
        scene->AddActionToObj(rotationBarrel, barrel);
        scene->AddActionToObj(barrelRoll, barrel);
        scene->AddActionToObj(removeBarrelAction, barrel);
        scene->AddActionToObj(barrelAnimator, barrel);
	}

    void SpawnBlueBarrel()
    {

        glm::vec3 position = glm::vec3(0, 180, -70);
        glm::vec3 rotation = glm::vec3(0, -90, 0);
        float scale = 0.1;

        Object* barrel = scene->GenerateMeshObjectsFromObject("assets/models/dk_3d_all_obj/DonkeyKong_Level_0_Barrel_2.ply", position, scale, rotation, true, glm::vec4(0.1, 0.1, 0.5, 1.f), false, scene->sceneObjects);
        barrel->name = "BlueBarrel";

        aRayCastPhysics2D* physics2Db = new aRayCastPhysics2D();
        aRotateAction* rotationBarrel = new aRotateAction();
        aRemoveAfterTime* removeBarrelAction = new aRemoveAfterTime();
        aRollDown* barrelRoll = new aRollDown();
        aModelsFramesAnimator* barrelAnimator = new aModelsFramesAnimator();
        barrelRoll->rollDownChance = 100;

        physics2Db->gravityAcceleration = glm::vec3(0, -0.05f, 0);
        physics2Db->speed = glm::vec3(0.f, 0.f, 1.f);
        physics2Db->baseRayCastLength = 5.f;
        physics2Db->bounciness = 1.f;

        rotationBarrel->rotationSpeed = 250.0f;
        rotationBarrel->physics = physics2Db;

        barrelRoll->descentSpeed = 1;
        barrelRoll->phys = physics2Db;
        barrelRoll->animator = barrelAnimator;
        //barrelRoll->velocity = physics2Db->speed;

        removeBarrelAction->timeToRemove = 52.0f;

        barrel->isTemporary = true;

        std::vector<std::string> berrelRollAnim = { "assets/models/dk_3d_all_obj/DonkeyKong_Level_0_Barrel_2.ply" };
        std::vector<std::string> berrelClimbAnim = { "assets/models/dk_3d_all_obj/DonkeyKong_Level_0_Barrel_5.ply" };
        barrelAnimator->animations.push_back(berrelRollAnim);
        barrelAnimator->animations.push_back(berrelClimbAnim);



        scene->AddActionToObj(physics2Db, barrel);
        scene->AddActionToObj(rotationBarrel, barrel);
        scene->AddActionToObj(barrelRoll, barrel);
        scene->AddActionToObj(removeBarrelAction, barrel);
        scene->AddActionToObj(barrelAnimator, barrel);
    
    }

    void SpawnStandingBarrel(glm::vec3 position)
    {
        glm::vec3 rotation = glm::vec3(0, -90, 0);
        float scale = 0.2;

        Object* standingBarrel = scene->GenerateMeshObjectsFromObject("assets/models/dk_3d_all_obj/DonkeyKong_Level_1_Rivet.ply", position, scale, rotation, true, glm::vec4(0.1, 0.1, 0.5, 1.f), false, scene->sceneObjects);
         scale = 0.1;
        position.y += 15.f;
        Object* fire = scene->GenerateMeshObjectsFromObject("assets/models/dk_3d_all_obj/DonkeyKong_Level_0_Fire_0.ply", position, scale, rotation, true, glm::vec4(0.9, 0.3, 0.3, 1.f), false, scene->sceneObjects);
        fire->mesh->bIsVisible = false;
        aStandingBarrel* standBarrelAction = new aStandingBarrel();
        scene->AddActionToObj(standBarrelAction, standingBarrel);
        standBarrelAction->fire = fire;

        aModelsFramesAnimator* animator = new aModelsFramesAnimator();
        std::vector<std::string> fireAnim = { "assets/models/dk_3d_all_obj/DonkeyKong_Level_0_Fire_0.ply",
                                              "assets/models/dk_3d_all_obj/DonkeyKong_Level_0_Fire_1.ply",
                                              "assets/models/dk_3d_all_obj/DonkeyKong_Level_0_Fire_2.ply",
                                              "assets/models/dk_3d_all_obj/DonkeyKong_Level_0_Fire_3.ply"};
        animator->animationSpeed = 5;

        scene->AddActionToObj(animator, fire);
        standingBarrel->isTemporary = true;

        animator->animations.push_back(fireAnim);
    }

};

