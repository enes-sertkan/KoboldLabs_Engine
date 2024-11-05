#pragma once

#include "Action.h";
#include "sObject.h"
#include <glm/vec3.hpp> 

class aMoveXYZSpeed : public Action
{
public:
	glm::vec3 speed;
	bool isMoving = true;

	void Update() override
	{
		object->mesh->positionXYZ.x += speed.x;
		object->mesh->positionXYZ.y += speed.y;
		object->mesh->positionXYZ.z += speed.z;

		//printf("WOW, you are UPDATING\n");
	}
};