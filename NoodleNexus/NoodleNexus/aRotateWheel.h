#pragma once
#include "sObject.h"
#include "Scene.hpp"

class Object;


class aRotateWheel :public Action
{
private:

public:

	virtual void Start()
	{
	}
	virtual void Update()
	{
		object->mesh->rotationEulerXYZ.x -= 180 * object->scene->deltaTime;
	}

};



