#pragma once

#include "Action.h"
#include "Scene.hpp"
#include "aIconNearObject.h"


class aPlayerHammerController : public Action
{
public:
	std::string equipTarget;
	bool isEquipped = false;
	float effectRange = 5.0f;



	void Update() override
	{
		for (Object* obj : object->scene->sceneObjects)
		{
			if (obj->name == "Hammer")
			{
				if (object->scene->IsNearby(equipTarget, object->mesh->positionXYZ))
				{
					//object->mesh->bIsVisible = false;  // Hide the item

					isEquipped = true;
					//object->mesh->objectColourRGBA = glm::vec4(0.3, 0.4, 0.1, 1.0f);


					obj->Destroy();
				}
			}
		}

		if (!isEquipped) return;

		for (Object* target : object->scene->sceneObjects) {
			if (target->name == "Barrel" || target->name == "FlameEnemy" || target->name == "BlueBarrel") {
				if (glm::distance(object->mesh->positionXYZ, target->mesh->positionXYZ) <= effectRange) {
					target->Destroy();
				}
			}
		}
	}

	Object* CreateHammerIcon(glm::vec3 position, float scale = 0.1f)
	{
		glm::vec3 rotation = glm::vec3(0, -90, 0);
		glm::vec4 color = glm::vec4(0.8f, 0.8f, 0.1f, 1.0f);
		//create new object with hummer model
		Object* hammer = object->scene->GenerateMeshObjectsFromObject(
			"assets/models/dk_3d_all_obj/DonkeyKong_Level_0_Hammer.ply",
			position,
			scale,
			rotation,
			true,
			color,
			false,
			object->scene->sceneObjects

		);
		hammer->name = "Hammer";

		//use the iconaction, give model name, and everything.

		aIconNearObject* iconAction = new aIconNearObject();
		Object* player = object->scene->sceneObjects[2];


		iconAction->isOn = true;
		iconAction->modelName = "assets/models/dk_3d_all_obj/DonkeyKong_Level_0_Hammer.ply";
		iconAction->offset = glm::vec3(0.0f, 1.0f, 0.5f);
		iconAction->objectToFollow = player;

		object->scene->AddActionToObj(iconAction, hammer);

		hammer->isTemporary = true;

		return hammer;

	}
};