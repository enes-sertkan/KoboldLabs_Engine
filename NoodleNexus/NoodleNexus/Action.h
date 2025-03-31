#pragma once
//#include "sObject.h"

class Object;


class Action
{
private:

public:

	Object* object = nullptr;
	virtual void Start()
	{}
	virtual void Update()
	{
		//printf("WOW, you are UPDATING\n");
	}

	virtual void OnDestroy()
	{
	
	}
};


