#pragma once

#include "Action.h";
#include <glm/vec3.hpp> 

class aDEstroyOnPos : public Action
{
public:
	glm::vec3 speed;
	bool destroyed = false;

	void Update() override
	{
		if (destroyed) return;
		if (object->mesh->positionXYZ.x < -20000)
		{
			


			std::vector<Action*>::iterator it = std::find(object->actions.begin(), object->actions.end(), this);
			if (it != object->actions.end())
			{
				object->actions.erase(it); // Remove from vector
			}

			destroyed = true;
			object->Destroy();
		}
		//printf("WOW, you are UPDATING\n");
	}
};