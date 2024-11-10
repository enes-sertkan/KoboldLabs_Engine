#pragma once

#include "Action.h";
#include <glm/vec4.hpp> 
#include "sObject.h"
#include <glm/glm.hpp>
#include "Scene.hpp"

class aChangeColorOverTime : public Action
{
public:
	glm::vec4 startColour;
	glm::vec4 endColour;
	float secondsToChange;
	float currentT=0;

	void Start() override
	{
		
	}

	void Update() override
	{

		if (currentT > 1.f) return;

		object->mesh->objectColourRGBA =  glm::mix(startColour, endColour, currentT);
		currentT += 1/secondsToChange * object->scene->deltaTime;

		
	}



};