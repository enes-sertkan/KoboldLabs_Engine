#pragma once

#include "Action.h";
#include <glm/vec3.hpp> 
#include <glm/vec4.hpp> 
#include "sObject.h"
#include "Scene.hpp"
#include <glm/glm.hpp>

class aStandingBarrel : public Action
{
public:
	Object* fire = nullptr;
	bool isON = false;

	void Update() override
	{

		for (Object* obj:object->scene->sceneObjects)
		{
			if (obj->name == "Barrel")
			{
				if (glm::distance(obj->mesh->positionXYZ, object->mesh->positionXYZ) < 5)
				{
					object->mesh->objectColourRGBA = glm::vec4(1.f, 1.f, 0.f, 1.f);

					if (fire) fire->mesh->bIsVisible = true;

					isON = true;

				}
			}

		}
		

	}
};