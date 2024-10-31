#pragma once
#include "Action.h"
#include "Scene.hpp"

class aExplosion : public Action
{
	void Update() override
	{
		object->mesh->uniformScale *= (1+10*object->scene->deltaTime);
		if (object->mesh->uniformScale > 1000)
			object->Destroy();
	}
};
