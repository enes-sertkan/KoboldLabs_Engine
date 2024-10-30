#pragma once
#pragma once

#include "Action.h";
#include <glm/vec3.hpp> 
#include "PhysicsManager.h"

class aAsteroidCollision : public Action
{
public:
	glm::vec3 speed;
	bool isMoving = true;
	PhysicsManager* PhysMan;

	void Update() override
	{
//CheckCollision
		//If yes do smt
	}
};