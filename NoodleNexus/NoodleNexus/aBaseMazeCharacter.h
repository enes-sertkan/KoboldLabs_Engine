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

	glm::vec3 targetWorldPosition;
	MazeGenerator* maze = nullptr;
	CharDirection curWanderingDirection;
	int health = 100;


	glm::vec3 GetMazePosition()
	{
		glm::vec2 mazePosition = maze->WorldToGrid(object->mesh->positionXYZ);
		
		return glm::vec3(mazePosition.x, mazePosition.y,0);
	}

	bool Move(CharDirection direction)
	{
		//std::cout << "MOVE()" << std::endl;
		int curX = GetMazePosition().x;
		int curY = GetMazePosition().y;
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
//		std::cout << "Trying to move to" << x << " " << y << " and tile is " << maze->GetMazePoint(x, y) << std::endl;


	//	if (maze->IsWall(x, y)) return false;



		/*if ((x == maze->thesChar->mazePosition.x) && (y == maze->thesChar->mazePosition.y)) {HandleEncounter(); return false;}
		if ((x == maze->minoChar->mazePosition.x) && (y == maze->minoChar->mazePosition.y)) { HandleEncounter(); return false; }*/


		//std::cout << "TRY MOVE()" << std::endl;
		//	mazePosition.x = x;
		//	mazePosition.y = y;
			targetWorldPosition = maze->GridToWorld(x, y);
			targetWorldPosition.y = object->mesh->positionXYZ.y;
			//DamageOnWalk();
		//	std::cout << "Current health: " << health << std::endl;
			return true;
	}
	//Moves to the target based on speed
	//returns if reaches position
	bool MoveToTargetPositionUpdate()
	{
		// Calculate direction and distance
		glm::vec3 direction = targetWorldPosition - object->mesh->positionXYZ;
		float distance = glm::length(direction);

		// Early exit if at target
		if (distance < 0.001f) {
			return true;
		}

		// Normalize direction and calculate movement
		direction = glm::normalize(direction);
		glm::vec3 movement = direction * speed * object->scene->deltaTime;

		// Update rotation to face direction (NEW CODE)
		if (distance > 0.1f) { // Only rotate when moving significantly
			// Calculate target yaw from movement direction
			float targetYaw = glm::degrees(glm::atan(-direction.x, -direction.z));

			// Get current rotation
			float currentYaw = object->mesh->rotationEulerXYZ.y;

			// Calculate shortest angle difference
			float deltaYaw = targetYaw - currentYaw;
			if (deltaYaw > 180) deltaYaw -= 360;
			if (deltaYaw < -180) deltaYaw += 360;

			// Smooth rotation interpolation
			float rotationSpeed = 200.0f; // Degrees per second
			float maxRotationStep = rotationSpeed * object->scene->deltaTime;
			float rotationStep = glm::clamp(deltaYaw, -maxRotationStep, maxRotationStep);

			// Apply rotation
			object->mesh->rotationEulerXYZ.y += rotationStep;
		}

		// Handle movement
		if (glm::length(movement) >= distance) {
			object->mesh->positionXYZ = targetWorldPosition;
			return true;
		}

		object->mesh->positionXYZ += movement;
		return false;
	}

	virtual void Start()
	{
		targetWorldPosition = maze->GridToWorld(GetMazePosition().x, GetMazePosition().y);
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

	void DamageOnWalk()
	{
		health--;
	}
};


