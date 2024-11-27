#pragma once

#include "Action.h"
#include "Scene.hpp"

class aEquipItem : public Action
{
	std::string equipTarget;


	void Update() override
	{
		for (Object* obj : object->scene->sceneObjects)
		{
			if (obj->name == "mario1")
			{
				if (object->scene->IsNearby(equipTarget, object->mesh->positionXYZ))
				{
					object->mesh->bIsVisible = false;
				}
			}
		}
	}
};