#pragma once


#include "aBaseMazeCharacter.h"

class Object;


class MinotaurChar : public BazeMazeCharacter
{
private:

public:


	virtual void Start()
	{
		//targetWorldPosition = maze->GridToWorld(mazePosition.x, mazePosition.y);
	}



	virtual void Update()
	{
	
	//	RageTick();
		/*speed = 300.f;
	
		if (MoveToTargetPositionUpdate())
		{
			if (glm::distance(maze->minoChar->mazePosition, maze->thesChar->mazePosition) <= 10)
			{
				object->mesh->objectColourRGBA = glm::vec4(0.9, 0.0, 0.0, 1);
				RageTick();
			}
			else
			{
				object->mesh->objectColourRGBA = glm::vec4(1, 1, 1, 1);
			 DefaultWanderingTick();
			}
		}*/



	}


	void RageTick()
	{	
		PickNewDirectionRage();
		speed = 500.f;
		if (!Move(curWanderingDirection))
			PickNewDirectionRage();
	}

	void PickNewDirectionRage()
	{
	//	glm::vec2 minoPos = maze->minoChar->mazePosition;
	//	glm::vec2 theoPos = maze->thesChar->mazePosition;

	//	//if distance on x bigger then distance
	//	//Mega stupid solution with * to remove minuses
	//	if ((minoPos.x - theoPos.x)* (minoPos.x - theoPos.x) > (minoPos.y - theoPos.y)* (minoPos.y - theoPos.y))
	//	{
	//		if (minoPos.x < theoPos.x) 
	//		{
	//			curWanderingDirection = CRIGHT;
	//		}

	//		if (minoPos.x > theoPos.x)
	//		{
	//			curWanderingDirection = CLEFT;
	//		}
	//	}

	//	else

	//	{
	//		if (minoPos.y < theoPos.y)
	//		{
	//			curWanderingDirection = CUP;
	//		}

	//		if (minoPos.y > theoPos.y)
	//		{
	//			curWanderingDirection = CDOWN;
	//		}

	//	}
	}
};


