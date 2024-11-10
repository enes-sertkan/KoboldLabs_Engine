#pragma once

#include "Action.h";
#include <glm/vec4.hpp> 
#include "sObject.h"
#include <glm/glm.hpp>
#include "Scene.hpp"
#include <vector>

class aSizeUpThenDown : public Action
{
public:
	unsigned int state = 0;
	std::vector<float> scales;
	std::vector<float>  secondsToChange;
	float currentT=0;

	void Start() override
	{
		
	}

	void Update() override
	{

		if (currentT >= 1.f)
		{
			state++;
			currentT = 0;
		}

		if (state > scales.size() - 2) return;


	
		currentT += 1/secondsToChange[state% secondsToChange.size()] * object->scene->deltaTime;
		if (currentT > 1) currentT = 1;

		object->mesh->uniformScale = glm::mix(scales[state], scales[state + 1], currentT);
		
	}



};

