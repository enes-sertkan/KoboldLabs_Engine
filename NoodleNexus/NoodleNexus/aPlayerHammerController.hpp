#pragma once

#include "Action.h"
#include "Scene.hpp"

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
					object->mesh->objectColourRGBA = glm::vec4(0.3, 0.4, 0.1, 1.0f);


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
};