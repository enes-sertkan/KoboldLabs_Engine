#pragma once

#include "Action.h";
#include <glm/vec3.hpp> 
#include "sObject.h"

class aDEstroyOnPos : public Action
{
public:
	glm::vec3 speed;
	

	void Update() override
	{
		object->mesh->positionXYZ.x += speed.x;
		object->mesh->positionXYZ.y += speed.y;
		object->mesh->positionXYZ.z += speed.z;

		//printf("WOW, you are UPDATING\n");
	}
};