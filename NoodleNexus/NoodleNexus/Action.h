#pragma once
#include "sObject.h"


class Action
{
private:

public:

	Object* object = nullptr;
	virtual void Start()
	{}
	virtual void Update()
	{
		printf("WOW, you are UPDATING\n");
	}

};

class MoveForward : public Action
{
	void Update() override
	{
		object->mesh->positionXYZ.x++;
	}
};