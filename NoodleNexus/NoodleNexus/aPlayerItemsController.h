
#pragma once

#include "Action.h";
#include <glm/vec3.hpp> 
#include "sObject.h"
#include "Laser.hpp"
#include "UsableItem.hpp"
#include "Gun.hpp"  


class aPlayerItemsController : public Action
{
public:

	Laser* laser = nullptr;
	Gun* gun = nullptr;

	void Start()
	{
		laser = new Laser();
		laser->program = object->scene->programs[0];
		laser->scene = object->scene;

		// Initialize gun
		gun = new Gun();
		gun->program = object->scene->programs[1];  // Assuming Gun uses a different shader program
		gun->scene = object->scene;
	}

	void Update() override
	{


		if ( glfwGetMouseButton(object->scene->window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
		{
			if (object->scene->isFlyCamera) {
				laser->position = object->mesh->positionXYZ; //object->scene->fCamera->getEyeLocation();

				laser->target = object->mesh->positionXYZ + glm::normalize(object->scene->fCamera->getTargetRelativeToCamera()) * 100.f;
			}
			else
			{
				laser->position = object->scene->fCamera->getEyeLocation(); //object->scene->fCamera->getEyeLocation();
				laser->position.y -= 5.f;

				laser->target = object->scene->fCamera->getEyeLocation() + glm::normalize(object->scene->fCamera->getTargetRelativeToCamera()) * 100.f;
			
			}
			
			laser->Use();
		}

		// Gun setup and use
		if (glfwGetKey(object->scene->window, GLFW_KEY_Q) == GLFW_PRESS)
		{
			glm::vec3 gunTarget;
			if (object->scene->isFlyCamera) {
				gun->position = object->mesh->positionXYZ;
				gunTarget = object->mesh->positionXYZ + glm::normalize(object->scene->fCamera->getTargetRelativeToCamera()) * 100.f;
			}
			else
			{
				gun->position = object->scene->fCamera->getEyeLocation();
				gun->position.y -= 5.f;
				gunTarget = object->scene->fCamera->getEyeLocation() + glm::normalize(object->scene->fCamera->getTargetRelativeToCamera()) * 100.f;
			}
			gun->Fire(gunTarget);  // Pass target directly to Fire
		}
	
		

	}
};