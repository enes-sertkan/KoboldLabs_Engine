
#pragma once

#include "Action.h";
#include <glm/vec3.hpp> 
#include "sObject.h"
#include "Laser.hpp"
#include "UsableItem.hpp"


class aPlayerItemsController : public Action
{
public:

	Laser* laser = nullptr;

	void Start()
	{
		laser = new Laser();
		laser->program = object->scene->programs[0];
		laser->scene = object->scene;
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

		if (glfwGetKey(object->scene->window, GLFW_KEY_E) == GLFW_PRESS)
		{
		///GUN setup and use
		}
	
		

	}
};