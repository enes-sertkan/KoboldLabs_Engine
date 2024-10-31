#pragma once

#include "Action.h";
#include <glm/vec3.hpp> 
#include "Scene.hpp"

class aMoveXYZSpeed : public Action
{
public:
	glm::vec3 speed;
	bool isMoving = true;

	void Update() override
	{
		double deltaTime = object->scene->deltaTime;
		object->mesh->positionXYZ.x += speed.x* deltaTime;
		object->mesh->positionXYZ.y += speed.y * deltaTime;
		object->mesh->positionXYZ.z += speed.z * deltaTime;

		//printf("WOW, you are UPDATING\n");
	}
};