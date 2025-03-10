#pragma once
#include "sObject.h"
#include "Scene.hpp"

class Object;


class aRotate:public Action
{
private:

public:

	virtual void Start()
	{
	} 
	virtual void Update()
	{
		object->mesh->rotationEulerXYZ.y += 100 * object->scene->deltaTime;
	}

};
