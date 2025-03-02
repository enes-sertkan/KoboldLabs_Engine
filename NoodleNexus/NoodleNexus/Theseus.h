#pragma once


#include "aBaseMazeCharacter.h"
#include <vector>
#include <limits>
#include "StupidPathFinder.h"

class Object;

//
//class MazeTile
//{
//public:
//	Object* floorTile;
//	int numberOfChecks;
//
//	void  AddCheck()
//	{
//		numberOfChecks++;
//
//	}
//	void  RemoveCheck()
//	{
//		numberOfChecks--;
//	}
//};


class TheseusChar : public BazeMazeCharacter
{
private:

public:

	StupidPathFinder* pathFinder = new StupidPathFinder();

	std::vector<glm::vec2> currentPath;
	int currentPathPoint = 0;
	bool onPath = false;

	virtual void Start()
	{
		targetWorldPosition = maze->GridToWorld(mazePosition.x, mazePosition.y);

	}



	virtual void Update()
	{
		speed = 180.f;

		object->mesh->bOverrideObjectColour = (1.f, health / 100, 0.f);

		HandleFindFood();
		HandleFindWater();

		//HandleFindFood();
		
		if (!MoveToTargetPositionUpdate()) return;

		std::cout << "Current health: " << health << std::endl;
		
		if (health < 75)
			SearchForFood();
		else
			DefaultWanderingTick();
			
	
	}	

	void SearchForFood()
	{
		if (onPath)
			MoveToNextPathPoint();
		else
			FindPathToFood();



		//check if on path
		//if no find one
	}

	void MoveToNextPathPoint()
	{
		currentPathPoint++;
		

		if (mazePosition.x < currentPath[currentPathPoint].x)
		{
			Move(CRIGHT);
		}	
		if (mazePosition.x > currentPath[currentPathPoint].x)
		{
			Move(CLEFT);
		}	
		
		if (mazePosition.y < currentPath[currentPathPoint].y)
		{
			Move(CUP);
		}
		
		if (mazePosition.y > currentPath[currentPathPoint].y)
		{
			Move(CDOWN);
		}

		if (currentPathPoint >= currentPath.size() )
		{
			currentPathPoint = 0;
			onPath = false;


		}

	}


	void FindPathToFood()
	{
		//Find closest food
		//

		pathFinder->maze = maze;

		std::vector<glm::vec2> path;
		path.clear();
		while (path.size() == 0 && (maze->waters.size() > 0 || maze->foods.size() > 0))
		{
			Object* target = FindClosestFoodOrWater(object->mesh->positionXYZ);
			glm::vec2 targetPos = maze->WorldToGrid(target->mesh->positionXYZ);  //maze->WorldToGrid(maze->foods[3]->mesh->positionXYZ); maze->minoChar->mazePosition;

			path = pathFinder->FindPath(mazePosition, targetPos);

			if (path.size() == 0)
			{
				maze->foods.erase(std::remove(maze->foods.begin(), maze->foods.end(), target), maze->foods.end());
				maze->waters.erase(std::remove(maze->waters.begin(), maze->waters.end(), target), maze->waters.end());
			}

		}

		if (path.size() > 0)
		{
			currentPath = path;

			onPath = true;
		}
	}

	Object* FindClosestFood(const glm::vec3& currentWorldPos) {
		Object* closestFood = nullptr;
		float smallestDistance = std::numeric_limits<float>::max();

		for (Object* food : maze->foods) {
			if (!food || !food->mesh) continue;  // Safety check

			// Get food position and convert to grid coordinates if needed
			glm::vec3 foodWorldPos = food->mesh->positionXYZ;

			// Calculate 2D distance (ignoring Y-axis)
			float distance = glm::distance(
				glm::vec2(currentWorldPos.x, currentWorldPos.z),
				glm::vec2(foodWorldPos.x, foodWorldPos.z)
			);

			if (distance < smallestDistance) {
				smallestDistance = distance;
				closestFood = food;
			}
		}

		return closestFood;
	}

	Object* FindClosestWater(const glm::vec3& currentWorldPos) {
		Object* closestWater = nullptr;
		float smallestDistance = std::numeric_limits<float>::max();

		for (Object* water : maze->waters) {
			if (!water || !water->mesh) continue;

			glm::vec3 waterWorldPos = water->mesh->positionXYZ;
			float distance = glm::distance(
				glm::vec2(currentWorldPos.x, currentWorldPos.z),
				glm::vec2(waterWorldPos.x, waterWorldPos.z)
			);

			if (distance < smallestDistance) {
				smallestDistance = distance;
				closestWater = water;
			}
		}
		return closestWater;
	}


	Object* FindClosestFoodOrWater(const glm::vec3& currentWorldPos) {
		Object* closestFood = FindClosestFood(currentWorldPos);
		Object* closestWater = FindClosestWater(currentWorldPos);

		// Handle cases where one or both are null
		if (!closestFood && !closestWater) return nullptr;
		if (!closestFood) return closestWater;
		if (!closestWater) return closestFood;

		// Calculate distances for comparison
		float foodDistance = glm::distance(
			glm::vec2(currentWorldPos.x, currentWorldPos.z),
			glm::vec2(closestFood->mesh->positionXYZ.x, closestFood->mesh->positionXYZ.z)
		);

		float waterDistance = glm::distance(
			glm::vec2(currentWorldPos.x, currentWorldPos.z),
			glm::vec2(closestWater->mesh->positionXYZ.x, closestWater->mesh->positionXYZ.z)
		);

		if ((foodDistance < waterDistance))
			std::cout << "Chose food on pos " << std::endl;
		else
			std::cout << "Chose water on pos " << std::endl;

		return (foodDistance < waterDistance) ? closestFood : closestWater;
	}

	void HandleFindFood()
	{
		for (Object* food : maze->foods) {
			if (!food || !food->mesh) continue;  // Safety check

			// Get food position and convert to grid coordinates if needed
			glm::vec3 foodWorldPos = food->mesh->positionXYZ;

			// Calculate 2D distance (ignoring Y-axis)
			float distance = glm::distance(maze->WorldToGrid(object->mesh->positionXYZ),
				maze->WorldToGrid(food->mesh->positionXYZ));

		

			if (distance < 1.5)
			{
				Heal(2);
				food->Destroy();
				maze->foods.erase(std::remove(maze->foods.begin(), maze->foods.end(), food), maze->foods.end());
			}
		}
	}


	void HandleFindWater()
	{
		for (Object* water : maze->waters) {
			if (!water || !water->mesh) continue;  // Safety check

			// Get food position and convert to grid coordinates if needed
			glm::vec3 waterWorldPos = water->mesh->positionXYZ;

			// Calculate 2D distance (ignoring Y-axis)
			float distance = glm::distance(maze->WorldToGrid(object->mesh->positionXYZ),
				maze->WorldToGrid(water->mesh->positionXYZ));



			if (distance < 1.5)
			{
				Heal(1);
				water->Destroy();
				maze->waters.erase(std::remove(maze->waters.begin(), maze->waters.end(), water), maze->waters.end());
			}
		}
	}
	

	void Heal(int healAmmount)
	{
		health += healAmmount;
	}
};


