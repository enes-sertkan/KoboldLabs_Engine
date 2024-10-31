#pragma once

#include "Action.h"
#include "sharedThings.h"



class aRandomRotation : public Action
{
public:
	glm::vec3 rotationSpeed;
	bool isMoving = true;

	float getRandomFloat(float a, float b) {
		float random = ((float)rand()) / (float)RAND_MAX;
		float diff = b - a;
		float r = random * diff;
		return a + r;
	}


	void Start() override
	{
	

	}

	void Update() override
	{
		object->mesh->rotationEulerXYZ.x += rotationSpeed.x;
		object->mesh->rotationEulerXYZ.y += rotationSpeed.y;
		object->mesh->rotationEulerXYZ.z += rotationSpeed.z;

	}
};