#pragma once

#include "Action.h";
#include "sObject.h"
#include <glm/vec3.hpp> 
#include <vector>
#include <string>
#include "Scene.hpp"


class aModelsFramesAnimator : public Action
{
private:
	std::vector<std::string> frameModelNames;


	int CalculateCurrentFrame()
	{

		currentTime += object->scene->deltaTime * animationSpeed;

		if (currentTime >= maxFrame)
		{
			currentTime = 0;
		}

		lastFrame = currentFrame;
		currentFrame = (int)currentTime;


		return currentFrame;

	}

	bool IsFrameChanged()
	{
		if (currentFrame != lastFrame) return true;

		return false;
	}

	void UpdateModelWithCurrentFrame()
	{
		object->mesh->modelFileName = frameModelNames[currentFrame];

		

	}


public:
	
	std::vector <std::vector<std::string>> animations;
	float animationSpeed=1;
	int currentFrame;
	float currentTime=0;
	int lastFrame;
	int maxFrame;
	bool lookingRight = true;
	
	void Start() override
	{
		frameModelNames = animations[0];
		maxFrame = frameModelNames.size();

	}

	void Update() override
	{


		if (IsFrameChanged())
			UpdateModelWithCurrentFrame();


		CalculateCurrentFrame();


		if (lookingRight)
			object->mesh->rotationEulerXYZ.y = 270;
		else
			object->mesh->rotationEulerXYZ.y = 90;
	}


	void ChangeAnimation(int newAnimationId)
	{
		if (newAnimationId >= animations.size())
		{
			std::cout << "ERROR: Trying to chage to animation that doesn't exist" << std::endl;
			return;
		}

		if (animations[newAnimationId] == frameModelNames)
		{
			//std::cout << "SAME ANIM" << std::endl;
			return;
		}

		lastFrame = -1;
		currentFrame = 0;
		currentTime = 0;
		frameModelNames = animations[newAnimationId];
		maxFrame = frameModelNames.size();
	}

	
};