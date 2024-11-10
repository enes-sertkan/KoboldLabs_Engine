
#pragma once

#include "Action.h";
#include <glm/vec3.hpp> 
#include "sObject.h"
#include "Laser.hpp"
#include "UsableItem.hpp"
#include "GranadeLauncher.hpp" 


class aPlayerItemsController : public Action
{
public:

	Laser* laser = nullptr;
	GranadeLauncher* grenade = nullptr;
	UsableItem* currentWeapon = nullptr;


	void Start()
	{
		laser = new Laser();
		laser->program = object->scene->programs[0];
		laser->scene = object->scene;

		// Initialize grenade launcher
		grenade = new GranadeLauncher();
		grenade->program = object->scene->programs[0];
		grenade->scene = object->scene;
	}

	void Update() override
	{


		if (glfwGetKey(object->scene->window, GLFW_KEY_E) == GLFW_PRESS)
		{
			laser->position = object->scene->fCamera->getEyeLocation();
			laser->position.y;// -20.f;
			laser->target = object->scene->fCamera->getTargetLocation();
			laser->Use();
		}

		if (glfwGetKey(object->scene->window, GLFW_KEY_Q) == GLFW_PRESS)
		{
			grenade->position = object->scene->fCamera->getEyeLocation();
			grenade->target = object->scene->fCamera->getTargetLocation();
			grenade->Use();  // Launch grenade
		}
	
		

	}
};