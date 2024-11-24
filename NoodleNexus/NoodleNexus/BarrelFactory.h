#pragma once

#include "aRayCastPhysics2D.hpp"
#include "aPlayerMovement2D.hpp"
#include "aRotateAction.hpp"
#include "GLCommon.h"
#include "aRemoveAfterTime.hpp"


//#include "linmath.h"
#include <glm/glm.hpp>
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> 

class BarrelFactory
{
public:
	Scene* scene;

	void SpawnRegularBarrel()
	{
        glm::vec3 position = glm::vec3(0, 190, -37);
        glm::vec3 rotation = glm::vec3(0, -90, 0);
        float scale = 0.1;

        Object* barrel = scene->GenerateMeshObjectsFromObject("assets/models/dk_3d_all_obj/DonkeyKong_Level_0_Barrel_2.ply", position, scale, rotation, true, glm::vec4(0.5, 0.25, 0.1, 1.f), false, scene->sceneObjects);
        aRayCastPhysics2D* physics2Db = new aRayCastPhysics2D();
        aRotateAction* rotationBarrel = new aRotateAction();
        aRemoveAfterTime* removeBarrelAction = new aRemoveAfterTime();


        physics2Db->gravityAcceleration = glm::vec3(0, -0.05f, 0);
        physics2Db->speed = glm::vec3(0.f, 0.f, 1.f);
        physics2Db->baseRayCastLength = 5.f;
        physics2Db->bounciness = 1.f;

        rotationBarrel->rotationSpeed = 250.0f;
        rotationBarrel->physics = physics2Db;

        removeBarrelAction->timeToRemove = 52.0f;

        scene->AddActionToObj(physics2Db, barrel);
        scene->AddActionToObj(rotationBarrel, barrel);
        scene->AddActionToObj(removeBarrelAction, barrel);

	}


};

