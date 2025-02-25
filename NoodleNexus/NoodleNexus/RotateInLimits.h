#pragma once
#include "sObject.h"

class Object;


class RotateInLimits: public Action
{
private:

public:

	float speed = 5.f;
	float minRot = 30;
	float maxRot = 100;

	
	virtual void Start()
	{}
	virtual void Update()
	{
		object->mesh->rotationEulerXYZ.y + speed;

		if (object->mesh->rotationEulerXYZ.y < minRot)
			if (speed < 0) speed = -speed;


		if (object->mesh->rotationEulerXYZ.y > maxRot)
			if (speed > 0) speed = -speed;


		//printf("WOW, you are UPDATING\n");
	}

};


