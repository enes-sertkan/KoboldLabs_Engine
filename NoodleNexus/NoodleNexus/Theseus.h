#pragma once
#pragma once


#include "aBaseMazeCharacter.h"

class Object;


class TheseusChar : public BazeMazeCharacter
{
private:

public:
	//health here. They should be int!!!


	virtual void Start()
	{
		targetWorldPosition = maze->GridToWorld(mazePosition.x, mazePosition.y);
	}



	virtual void Update()
	{
		if (MoveToTargetPositionUpdate())
			DefaultWanderingTick();
	}

};


