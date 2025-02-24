#pragma once

#include "Action.h";
#include <glm/vec3.hpp> 
#include "sObject.h"

//#include "sObject.h"

class Object;


class CameraToTexture : public Action
{
private:

public:

	Object* object = nullptr;
	virtual void Start()
	{
	//Create and allocate texture

	
	}
	virtual void Update()
	{
		//printf("WOW, you are UPDATING\n");
	}

};


