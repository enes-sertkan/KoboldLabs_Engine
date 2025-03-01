#pragma once
#include "sObject.h"
#include "MazeGenerator.hpp"

class Object;

enum CharDirection {
	CUP,
	CDOWN,
	CLEFT,
	CRIGHT
};

class BazeMazeCharacter : public Action
{
private:

public:
	float speed = 250;
	glm::vec3 mazePosition;
	glm::vec3 targetWorldPosition;
	MazeGenerator* maze = nullptr;
	CharDirection curWanderingDirection;
	int health = 100;



	bool Move(CharDirection direction)
	{
		std::cout << "MOVE()" << std::endl;
		int curX = mazePosition.x;
		int curY = mazePosition.y;
		switch (direction)
		{
		case CUP: object->mesh->rotationEulerXYZ.y = 0; return TryMovingToMazePoint(curX, curY + 1); break;
		case CDOWN: object->mesh->rotationEulerXYZ.y = 180; return TryMovingToMazePoint(curX, curY-1); break;
		case CLEFT:  object->mesh->rotationEulerXYZ.y = 270;  return TryMovingToMazePoint(curX-1, curY); break;
		case CRIGHT: object->mesh->rotationEulerXYZ.y = 90; return TryMovingToMazePoint(curX+1, curY); break;

		default:
		
			break;
			return false;
		}
	}

	bool TryMovingToMazePoint(int x, int y)
	{
		//std::cout << "Trying to move to" << mazePosition.x << " " << mazePosition.y << " and tile is" << maze->GetMazePoint(mazePosition.x, mazePosition.y) << std::endl;


		if (maze->IsWall(y, x)) return false;



		if ((x == maze->thesChar->mazePosition.x) && (y == maze->thesChar->mazePosition.y)) {HandleEncounter(); return false;}
		if ((x == maze->minoChar->mazePosition.x) && (y == maze->minoChar->mazePosition.y)) { HandleEncounter(); return false; }


		//std::cout << "TRY MOVE()" << std::endl;
			mazePosition.x = x;
			mazePosition.y = y;
			targetWorldPosition = maze->GridToWorld(x, y);
			DamageOnWalk();
			std::cout << "Current health: " << health << std::endl;
			return true;
	}
	//Moves to the target based on speed
	//returns if reaches position
	bool MoveToTargetPositionUpdate()
	{
	//	std::cout << "START Moving to point" << mazePosition.x <<" "<< mazePosition.y<<std::endl;
		// Calculate the direction vector
		glm::vec3 direction = targetWorldPosition - object->mesh->positionXYZ;

		// Calculate the distance to the target
		float distance = length(direction);

		// If the distance is very small, return the target position directly
		if (distance < 0.001f) {
			return true;
		}

		// Normalize the direction vector
		direction = normalize(direction);

		// Calculate the movement vector
		glm::vec3 movement = direction * speed*object->scene->deltaTime;

		// If the movement would overshoot the target, clamp it to the target position
		if (length(movement) >= distance) {
			return true;
		}

	//	std::cout << "Moving to point" << targetWorldPosition.x << " " << targetWorldPosition.z << std::endl;
		
		object->mesh->positionXYZ += movement;
		return false;
	}

	virtual void Start()
	{
		targetWorldPosition = maze->GridToWorld(mazePosition.x, mazePosition.y);
	}
	

	
	virtual void Update()
	{
		
		if (MoveToTargetPositionUpdate())
			DefaultWanderingTick();
		


	}

	void HandleControls()
	{
		if (glfwGetKey(object->scene->window, GLFW_KEY_LEFT) == GLFW_PRESS)
		{
			Move(CLEFT);
		}

		if (glfwGetKey(object->scene->window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		{
			Move(CRIGHT);
		}

		if (glfwGetKey(object->scene->window, GLFW_KEY_UP) == GLFW_PRESS)
		{
			Move(CUP);
		}

		if (glfwGetKey(object->scene->window, GLFW_KEY_DOWN) == GLFW_PRESS)
		{
			Move(CDOWN);
		}
	}
	void PickNewDirection()
	{
		int pickDir = rand() % 4;
		curWanderingDirection = static_cast<CharDirection>(pickDir);
	}

	void DefaultWanderingTick()
	{
		speed = 250.f;
		if (!Move(curWanderingDirection))
			PickNewDirection();
		else
		{
			
		}
	}

	void HandleEncounter()
	{
		//Do logic for when they meet.
		if (maze->thesChar->health > 50)
			maze->minoChar->object->Destroy();
		else
			maze->thesChar->object->Destroy();
;
		//object->Destroy();

	}

	void DamageOnWalk()
	{
		health--;
	}
};


