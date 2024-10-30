#include "sObject.h"
#include"Scene.hpp"


//TODO BAD CODE FIX
void Object::Destroy()
{
	for (Action* action : actions)
	{

		std::vector<Action*>::iterator it = std::find(actions.begin(), actions.end(), action);
		if (it != actions.end())
		{
			actions.erase(it); // Remove from vector
		}

	}

	scene->RemoveObject(this);
	enabled = false;
}