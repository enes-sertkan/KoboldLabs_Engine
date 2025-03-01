#pragma once


#include "aBaseMazeCharacter.h"

class Object;


class MinotaurChar : public BazeMazeCharacter
{
private:

public:


	virtual void Start()
	{
		targetWorldPosition = maze->GridToWorld(mazePosition.x, mazePosition.y);
	}



	virtual void Update()
	{

		if (MoveToTargetPositionUpdate())
		{

			//if (maze.theChar is closeenough)
			//do RageTick()


			DefaultWanderingTick();

		}



	}


	void RageTick()
	{
		//Decide what direction to go
		//Use Move(direction) direction is CharDIrection
	}
};


